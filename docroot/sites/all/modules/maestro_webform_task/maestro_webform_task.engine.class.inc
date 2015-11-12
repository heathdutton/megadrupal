<?php


/**
 * Maestro Task Engine Class
 */


include_once './' . drupal_get_path('module', 'maestro') . '/maestro_tasks.class.php';

class MaestroTaskTypeWebform extends MaestroTask {

  function execute() {
    // Check to see if the current status has been set to 1.
    // If so, completion status is set to true to complete the task.

    if($this->_properties->status == 1) {
      $this->completionStatus = MaestroTaskStatusCodes::STATUS_COMPLETE;  //just complete it!
    }
    else {
      $this->completionStatus = FALSE;
      $this->setMessage( 'Conent Type task -- status is 0.  Will not complete this task yet.');
    }
    $this->executionStatus = TRUE;
    $this->setRunOnceFlag($this->_properties->id);
    $this->setTaskStartedDate($this->_properties->id);
    return $this;
  }

  function prepareTask() {
    $serializedData = db_query("SELECT task_data FROM {maestro_template_data} WHERE id = :tid",
    array(':tid' => $this->_properties->taskid))->fetchField();
    $taskdata = @unserialize($serializedData);
    return array('serialized_data' => $serializedData);
  }

  function getTaskConsoleURL(){
    global $base_url;

    /* The tracking_id provides us with the unique instance of the workflow
     * and the taskdata will contain the webform_nid and the instance_name
     * so we can determine the webform submission (sid) record in the
     * maestro_project_content table to create the URL to either
     * launch a new webform or edit/view an existing result.
     */

    $taskdata = unserialize($this->_properties->task_data);
    if (empty($taskdata['webform_nid'])) {
      $maestro = Maestro::createMaestroObject(1);
      $taskdata['webform_nid'] = $maestro->engine()->getProcessVariable('webform_nid', $this->_properties->process_id);
      /* Now that we have the webform for this instance of the workflow
       * Required for the code in the HOOK_form_alter to work correctly.
       * We need to register the webform being used and since
       * taskdata is not set, we can assume there is no saved webform data
       * Save the content type in the task data
      */
      $webform_nid = intval($taskdata['webform_nid']);
      $instance_name = '';
      $data = serialize(array('webform_nid' => $webform_nid, 'instance_name' => ''));
      db_update('maestro_queue')
        ->fields(array('task_data' => $data))
        ->condition('id', $this->_properties->queue_id, '=')
        ->execute();
    } else {
      $webform_nid = intval($taskdata['webform_nid']);
      $instance_name = (isset($taskdata['instance_name'])) ? $taskdata['instance_name'] : '';
    }

    if (empty($webform_nid)) {
      watchdog('maestro', "Error: Invalid webform id for task in getTaskConsoleURL method");
      return '#';
    }

    $maestro_webforms = cache_get('maestro_webforms');
    if ($maestro_webforms === FALSE) {
      $webforms = array();
    } else {
      $webforms = $maestro_webforms->data;
    }
    $webform_form_id = "webform_client_form_{$webform_nid}";
    if (!in_array($webform_form_id, $webforms)) {
      $webforms[] = $webform_form_id;
      cache_set('maestro_webforms', $webforms);
    }


    // Each webform is of node type 'webform' and each node record (nid) is a separate webform definition
    // We need to track the webform definition (nid) and the instance of the webform submission (sid)
    $content_type = 'webform';
    $tracking_id = db_select('maestro_process')
    ->fields('maestro_process', array('tracking_id'))
    ->condition('id', $this->_properties->process_id, '=')
    ->execute()->fetchField();

    // Check and see if the submission id is valid or if not set retrieve it and save it for future use in the taskdata
    $submission_id = 0;
    if ($tracking_id > 0) {
      /* Verify the submission record exists in the project content table.
       * If there is no instance name then we have to assume only one instance of this webform is being used in the workflow
       * Unfortunately the field 'instance' already existed and used here for the submission id but if the same form
       * is used more then once, the webform_nid will be the same value - we will need to use the instance_name
       * which is optionally defined in the task editor for this task - null if only 1 instance of the webform used.
       */
      $query = db_select('maestro_project_content', 'a');
      $query->addField('a','instance', 'submission_id');
      $query->condition('a.nid', $webform_nid, '=');
      $query->condition('a.tracking_id', $tracking_id, '=');
      $query->condition('a.content_type', $content_type, '=');
      if (!empty($instance_name)) {
        $query->condition('a.instance_name', $instance_name, '=');
      }
      $result = $query->execute();
      $numrows = $query->countQuery()->execute()->fetchField();
      if ($numrows > 0 ) {
        $rec = $result->fetchObject();
        $submission_id = $rec->submission_id;
      }
    }

    if ($submission_id > 0) {
      $url = url("node/{$webform_nid}/submission/{$submission_id}/edit/maestro/edit/{$this->_properties->queue_id}/completeonsubmit");
    } else {
        $url = url("node/{$webform_nid}/maestro/{$this->_properties->queue_id}");
    }

    $url .= '/maestro_hidelocaltasks';
    return $url;
  }


  /* Method called by custom webform submit handler this module added
   * to handle tracking of the webform submission for this workflow instance
  */
  function processContent($taskid, $op, $data) {
    $rec = db_select('maestro_queue')
    ->fields('maestro_queue', array('process_id','template_data_id', 'task_data'))
    ->condition('id', $taskid, '=')
    ->execute()->fetchObject();

    $taskdata = @unserialize($rec->task_data);

    /* @TODO: Add logic to handle draft state of webforms */
    $status = MaestroContentStatusCodes::STATUS_PUBLISHED; // For now - assume published
    /*
    if ($node->status == 1) {
      $status = MaestroContentStatusCodes::STATUS_PUBLISHED;
    } else {
      $status = 0;
    }
    */

    $tracking_id = db_select('maestro_process')
    ->fields('maestro_process', array('tracking_id'))
    ->condition('id', $rec->process_id, '=')
    ->execute()->fetchField();

    db_insert('maestro_project_content')
    ->fields(array(
    'nid' => $data['nid'],
    'tracking_id' => $tracking_id,
    'task_id' => $taskid,
    'content_type' => 'webform',
    'instance' => $data['sid'],
    'instance_name' => $taskdata['instance_name'],
    'status'  => $status
    ))
    ->execute();

    // Initiate the mestro workflow engine and complete the task
    // Complete task is an engine method
    $maestro = Maestro::createMaestroObject(1);
    $maestro->engine()->completeTask($taskid);

  }

  // Method to return HTML formatted content to include in the project detail area
  function showContentDetail($tracking_id,$task_id) {

    $retval = '';
    /* Format any content records */
    $query = db_select('maestro_project_content','content');
    $query->addField('content','nid');
    $query->addField('content','instance','sid');
    $query->addField('content','status');
    $query->condition('content.tracking_id',$tracking_id,'=');
    $query->condition('content.task_id',$task_id,'=');
    $res = $query->execute();
    foreach ($res as $record) {
      $node = node_load($record->nid);
      $variables['content_records'][$record->nid] = $node->title;
      $retval .= '<div>' . l($node->title, "maestro/webform_viewdialog/{$record->nid}/{$record->sid}/nojs",
        array('attributes' => array('class' => array('ctools-modal-maestro-viewcontent-modal-style', 'ctools-use-modal'))));

      $retval .= '<span style="padding-left:10px;">' . t('Status') . ': ';
      $retval .= t(MaestroContentStatusCodes::getStatusLabel($record->status));
      $retval .= '</span></div>';
    }
    return $retval;
  }

}

