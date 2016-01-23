<?php




/**
 * Maestro Task Engine Class
 */

include_once './' . drupal_get_path('module', 'maestro') . '/maestro_tasks.class.php';

class MaestroTaskTypeEntityform extends MaestroTask {

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
     * and the taskdata will contain the entityform_id and the instance_name
     * so we can determine the entityform submission (sid) record in the
     * maestro_project_content table to create the URL to either
     * launch a new entityform or edit/view an existing result.
     */

    $taskdata = unserialize($this->_properties->task_data);
    if (empty($taskdata['entityform_id'])) {
      $maestro = Maestro::createMaestroObject(1);
      $taskdata['entityform_id'] = $maestro->engine()->getProcessVariable('entityform_id', $this->_properties->process_id);
      /* Now that we have the entityform for this instance of the workflow
       * Required for the code in the HOOK_form_alter to work correctly.
       * We need to register the entityform being used and since
       * taskdata is not set, we can assume there is no saved entityform data
       * Save the content type in the task data
      */
      $entityform_id = intval($taskdata['entityform_id']);
      $instance_name = '';
      $data = serialize(array('entityform_id' => $entityform_id, 'instance_name' => ''));
      db_update('maestro_queue')
        ->fields(array('task_data' => $data))
        ->condition('id', $this->_properties->queue_id, '=')
        ->execute();
    } else {
      $entityform_id = $taskdata['entityform_id'];
      $instance_name = (isset($taskdata['instance_name'])) ? $taskdata['instance_name'] : '';
    }

    if (empty($entityform_id)) {
      watchdog('maestro', "Error: Invalid entityform id for task in getTaskConsoleURL method");
      return '#';
    }

    $maestro_entityforms = cache_get('maestro_entityforms');
    if ($maestro_entityforms === FALSE) {
      $entityforms = array();
      $query = db_select('maestro_template_data', 'template_data');
      $query->addField('template_data', 'id');
      $query->addField('template_data', 'task_data', 'task_data');
      $query->condition('task_class_name', 'MaestroTaskTypeEntityform', '=');
      $res = $query->execute();
      foreach ($res as $rec) {
        $data = unserialize($rec->task_data);
        if (!empty($data['entityform_id']) AND !in_array("{$data['entityform_id']}_entityform_edit_form", $entityforms)) {
          $entityforms[] = "{$data['entityform_id']}_entityform_edit_form";
        }
      }
      cache_set('maestro_entityforms', $entityforms);
      $maestro_entityforms = $entityforms;
    } else {
      $maestro_entityforms = $maestro_entityforms->data;
    }

    if (!in_array("{$entityform_id}_entityform_edit_form", $maestro_entityforms)) {
      $maestro_entityforms[] = "{$entityform_id}_entityform_edit_form";
      cache_set('maestro_entityforms', $maestro_entityforms);
    }


    // We need to track the entityform definition (nid) and the instance of the entityform submission (sid)
    $content_type = 'entityform';
    $tracking_id = db_select('maestro_process')
    ->fields('maestro_process', array('tracking_id'))
    ->condition('id', $this->_properties->process_id, '=')
    ->execute()->fetchField();

    // Check and see if the submission id is valid or if not set retrieve it and save it for future use in the taskdata
    $submission_id = 0;
    if ($tracking_id > 0) {
      /* Verify the submission record exists in the project content table.
       * If there is no instance name then we have to assume only one instance of this entityform is being used in the workflow
       * Unfortunately the field 'instance' already existed and used here for the submission id but if the same form
       * is used more then once, the entityform_id will be the same value - we will need to use the instance_name
       * which is optionally defined in the task editor for this task - null if only 1 instance of the entityform used.
       */
      $query = db_select('maestro_project_content', 'a');
      $query->addField('a','nid', 'submission_id');
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
      $url = url("entityform/{$submission_id}/edit/maestro/edit/{$this->_properties->queue_id}/completeonsubmit");
    } else {
        $url = url("eform/submit/{$entityform_id}/maestro/{$this->_properties->queue_id}");
    }

    return $url;
  }


  /* Method called by custom entityform submit handler this module added
   * to handle tracking of the entityform submission for this workflow instance
  */
  function processContent($taskid, $op, $data) {
    $rec = db_select('maestro_queue')
    ->fields('maestro_queue', array('process_id','template_data_id', 'task_data'))
    ->condition('id', $taskid, '=')
    ->execute()->fetchObject();

    $taskdata = @unserialize($rec->task_data);

    /* @TODO: Add logic to handle draft state of entityforms */
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
    'nid' => $data['id'],
    'tracking_id' => $tracking_id,
    'task_id' => $taskid,
    'content_type' => 'entityform',
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
    $query->addField('content','nid', 'submission_id');
    $query->addField('content','status');
    $query->condition('content.tracking_id',$tracking_id,'=');
    $query->condition('content.task_id',$task_id,'=');
    $res = $query->execute();
    foreach ($res as $record) {
      $entity = entityform_load($record->submission_id);
      $entityform_type = entityform_get_types($entity->type);
      $variables['content_records'][$record->submission_id] = $entityform_type->label;
      $retval .= '<div>' . l($entityform_type->label, "maestro/entityform_viewdialog/{$record->submission_id}/nojs",
        array('attributes' => array('class' => array('ctools-modal-maestro-viewcontent-modal-style', 'ctools-use-modal'))));

      $retval .= '<span style="padding-left:10px;">' . t('Status') . ': ';
      $retval .= t(MaestroContentStatusCodes::getStatusLabel($record->status));
      $retval .= '</span></div>';
    }
    return $retval;
  }

}

