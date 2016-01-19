<?php

/* Maestro HOOK to modules to return info on any new task handlers - ie functions for tasks */
function maestro_webform_task_maestro_handler_options() {
  $handlers = array(
    'MaestroTaskTypeInteractiveFunction' => array(
      'maestro_reviewWebForm' => t('Review with Approve/Reject - provides a link to review a submitted web form. Will ask the task owner if they accept this document and their answer can be tested for by an IF task. <p>Requires that you use a variable called "instance_name". It can be Process Variable or a Task Variable (defined under the edit task dialog - Optional tab) that is set to the instance name for the web form previously defined in the workflow that this function will review.</p>'),
      'maestro_viewWebForm' => t('Review task with a link to review a submitted web form.<p>Requires that you use a variable called "instance_name". It can be Process Variable or a Task Variable (defined under the edit task dialog - Optional tab) that is set to the instance name for the web form previously defined in the workflow that this function will review.</p>'),
      ),
    );
    return $handlers;
}

/* Review submitted Web Form results - with Accept/Reject question */
function maestro_reviewWebForm($op,&$task,$parms) {
  global $base_url;

  $retval = new stdClass();
  $data = unserialize($task->_properties->task_data);

  /* Determine the web form instance_id to retrieve the web form submission result to review
   * First check if the task has defined a local variable
   * if not then check for a process variable
  */
  if(isset($data['optional_parm']['instance_name'])) {
    $instance_name = $data['optional_parm']['instance_name'];
  } else {
    $maestro = Maestro::createMaestroObject(1);
    $instance_name = $maestro->engine()->getProcessVariable('instance_name', $task->_properties->process_id);
  }

  $content_type = 'webform';
  $tracking_id = db_select('maestro_process')
  ->fields('maestro_process', array('tracking_id'))
  ->condition('id', $task->_properties->process_id, '=')
  ->execute()->fetchField();

  $query = db_select('maestro_project_content', 'a');
  $query->addField('a','nid', 'nid');
  $query->addField('a','instance', 'sid');
  $query->condition('a.tracking_id', $tracking_id, '=');
  $query->condition('a.content_type', $content_type, '=');
  if (!empty($instance_name)) {
    $query->condition('a.instance_name', $instance_name, '=');
  }
  $result = $query->execute();
  $numrows = $query->countQuery()->execute()->fetchField();
  if ($numrows > 0 ) {
    $rec = $result->fetchObject();
  }

  $submission = FALSE;
  if ($rec->nid > 0 AND $rec->sid > 0) {
      module_load_include('inc', 'webform' , 'includes/webform.submissions');
      $node = node_load($rec->nid);
      $submission = webform_get_submission($rec->nid, $rec->sid);
  }

  if ($submission === FALSE) {
    $retval->retcode = TRUE;
    $retval->html = t('Invalid WebForm Result: Content possibly has been deleted.');
    return $retval;
  }

  $retval->html = '';
  $retval->retcode = FALSE;
  $retval->engineop = '';  // Optionally set the engineop value if you want to complete the task

  switch ($op) {
    case 'display':
      $data = $task->getTempData();
      $retval->html = drupal_render(drupal_get_form('maestro_webform_review_approve_form',array(
        'sid' => $rec->sid,
        'queue_id' => $task->_properties->queue_id,
        'tracking_id' => $tracking_id,
        'title' => $node->title,
        'mode' => 'approve',
        'data' => $data)));
      $retval->retcode = TRUE;
      $retval->engineop = '';
      break;
    case 'complete':
      $data = $task->getTempData();
      $tracking_id = maestro_getTaskTrackingId($task->_properties->queue_id);
      if (empty($data['reviewstatus']) AND !isset($_POST['reviewstatus'])) {
        $retval->retcode = FALSE;

      } elseif ($data['reviewstatus'] == 'accept' OR $_POST['reviewstatus'] == 'yes') {
        $data['reviewstatus'] = 'accept';
        $task->saveTempData($data);
        $retval->retcode = TRUE;
        $retval->status = MaestroTaskStatusCodes::STATUS_COMPLETE;
        $retval->engineop = 'completetask';
        // Flag the content record as accepted
        db_update('maestro_project_content')
          ->fields(array( 'status' => MaestroContentStatusCodes::STATUS_UNDER_REVIEW))
          ->condition('tracking_id', $tracking_id)
          ->condition('nid', $nid)
          ->execute();

      } elseif ($data['reviewstatus'] == 'reject' OR $_POST['reviewstatus'] == 'no') {
        $data['reviewstatus'] = 'reject';
        $task->saveTempData($data);
        $retval->retcode = TRUE;
        $retval->status = MaestroTaskStatusCodes::STATUS_IF_CONDITION_FALSE;
        $retval->engineop = 'completetask';
        // Flag the content record as rejected
        db_update('maestro_project_content')
          ->fields(array( 'status' => MaestroContentStatusCodes::STATUS_REJECTED))
          ->condition('tracking_id', $tracking_id)
          ->condition('nid', $nid)
          ->execute();

      } else {
        $retval->retcode = FALSE;
      }
      break;
    case 'update':
      if($_POST['reviewstatus'] == 'yes') {
        $data['reviewstatus'] = 'accept';
        $task->saveTempData($data);
      } elseif ($_POST['reviewstatus'] == 'no') {
        $data['reviewstatus'] = 'reject';
        $task->saveTempData($data);
      }
      $retval->retcode = TRUE;
      break;
  }

  return $retval;

}


/* Simple view submitted Web Form results - link to view node */
function maestro_viewWebForm($op,&$task,$parms) {
  global $base_url;

  $retval = new stdClass();
  $data = unserialize($task->_properties->task_data);

  /* Determine the web form instance_id to retrieve the web form submission result to review
   * First check if the task has defined a local variable
   * if not then check for a process variable
  */
  if(isset($data['optional_parm']['instance_name'])) {
    $instance_name = $data['optional_parm']['instance_name'];
  } else {
    $maestro = Maestro::createMaestroObject(1);
    $instance_name = $maestro->engine()->getProcessVariable('instance_name', $task->_properties->process_id);
  }

  $content_type = 'webform';
  $tracking_id = db_select('maestro_process')
  ->fields('maestro_process', array('tracking_id'))
  ->condition('id', $task->_properties->process_id, '=')
  ->execute()->fetchField();

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
  }

  $entity = entityform_load($rec->submission_id);
  if ($entity === FALSE) {
    $retval->retcode = TRUE;
    $retval->html = t('Invalid Entity Form Result: Content possibly has been deleted.');
    return $retval;
  }
  $entityform_type = entityform_get_types($entity->type);
  $retval->html = '';
  $retval->retcode = FALSE;
  $retval->engineop = '';  // Optionally set the engineop value if you want to complete the task

  switch ($op) {
    case 'display':
      $data = $task->getTempData();
      $retval->html = drupal_render(drupal_get_form('maestro_entityform_review_approve_form',array(
        'sid' => $rec->submission_id,
        'queue_id' => $task->_properties->queue_id,
        'tracking_id' => $tracking_id,
        'title' => $entityform_type->label,
        'mode' => 'review',
        'data' => $data)));
      $retval->retcode = TRUE;
      $retval->engineop = '';
      break;
    case 'complete':
        $retval->retcode = TRUE;
        $retval->status = MaestroTaskStatusCodes::STATUS_COMPLETE;
        $retval->engineop = 'completetask';
      break;
  }

  return $retval;

}


function maestro_webform_review_approve_form($form, $form_state, $parms) {

  if ($parms['data']['reviewstatus'] == 'accept') {
    $default_value = 'yes';
  } elseif($parms['data']['reviewstatus'] == 'reject') {
    $default_value = 'no';
  } else {
    $default_value = '';
  }

  $content_type = 'webform';
  $query = db_select('maestro_project_content', 'a');
  $query->addField('a','nid', 'nid');
  $query->addField('a','status', 'status');
  $query->condition('a.tracking_id', $parms['tracking_id'], '=');
  $query->condition('a.instance', $parms['sid'], '=');
  $query->condition('a.content_type', $content_type, '=');
  if (!empty($instance_name)) {
    $query->condition('a.instance_name', $instance_name, '=');
  }
  $result = $query->execute();
  $numrows = $query->countQuery()->execute()->fetchField();
  if ($numrows > 0 ) {
    $rec = $result->fetchObject();
  }

  $submission = FALSE;
  if ($rec->nid > 0 AND $parms['sid'] > 0) {
      module_load_include('inc', 'webform' , 'includes/webform.submissions');
      $node = node_load($rec->nid);
      $submission = webform_get_submission($rec->nid, $parms['sid']);
  }


  if ($submission !== FALSE) {
    $edit_content_link = l($node->title, "node/{$rec->nid}/submission/{$parms['sid']}/edit/maestro/edit/{$parms['queue_id']}");
  } else {
    $edit_content_link = '';
  }

  $form = array();
  $form['review_item'] = array(
    '#markup' =>  t('You have a task to Review and Edit the following form submission:') . '<span style="padding-left:5px;">' . $edit_content_link . '</span><div>' . t('Status: ') . t(MaestroContentStatusCodes::getStatusLabel($rec->status)) . '</div>',
  );

  if ($parms['mode'] == 'approve') {
    $form['reviewstatus'] = array(
      '#type' => 'radios',
      '#title' => t('Do you accept this document'),
      '#options' => array('yes' => t('Yes'), 'no' => t('No')),
      '#default_value' => $default_value,
    );
    $form['actions']['update_task'] = array(
      '#type' => 'button',
      '#default_value' => t('Update'),
      '#attributes' => array('maestro' => array('update')),
    );
  }

  $form['actions']['complete_task'] = array(
    '#type' => 'button',
    '#default_value' => t('Complete Task'),
    '#attributes' => array('maestro' => array('complete')),
  );

  return $form;

}
