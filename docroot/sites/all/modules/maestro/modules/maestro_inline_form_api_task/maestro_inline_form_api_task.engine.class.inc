<?php

/**
 * Maestro Task Engine Class
 */

include_once './' . drupal_get_path('module', 'maestro') . '/maestro_tasks.class.php';

class MaestroTaskTypeInlineFormAPI extends MaestroTask {

  function execute() {
    $this->setRunOnceFlag($this->_properties->id);
    $this->completionStatus = FALSE;
    $this->executionStatus = TRUE;
    return $this;
  }

  function prepareTask() {
    $serializedData = db_query("SELECT task_data FROM {maestro_template_data} WHERE id = :tid",
    array(':tid' => $this->_properties->taskid))->fetchField();
    $taskdata = @unserialize($serializedData);

    return array('form_api_code' => $taskdata['form_api_code'], 'serialized_data' => $serializedData);
  }

  function showInteractiveTask() {
    $retval = '';
    $serializedData = db_query("SELECT task_data FROM {maestro_queue} WHERE id = :id",
    array(':id' => $this->_properties->queue_id))->fetchField();
    $taskdata = @unserialize($serializedData);

    $form = array();

    eval(html_entity_decode($taskdata['form_api_code']));

    $tracking_id = db_select('maestro_process')
      ->fields('maestro_process', array('tracking_id'))
      ->condition('id', $this->_properties->process_id, '=')
      ->execute()->fetchField();
    $default_data = $this->getProjectData($tracking_id, $taskdata['content_type']);

    if ($default_data !== FALSE) {
      $this->updateDefaultFormData($form, $default_data);
    }

    $form['queue_id'] = array(
      '#type' => 'hidden',
      '#default_value' => $this->_properties->queue_id,
    );
    $form['save'] = array(
      '#type'           => 'submit',
      '#default_value'  => t('Save'),
    );
    $form['complete'] = array(
      '#type'           => 'submit',
      '#default_value'  => t('Complete Task'),
      '#attributes'=> array('maestro'=>array('completetask')),
    );

    $form['#submit'][] = 'maestro_inline_form_api_task_form_submit';

    return drupal_render(drupal_get_form('maestro_inline_form_api_task_form', $form));
  }

  function updateDefaultFormData(&$form, &$data, $parent='') {
    $value_handled = FALSE;
    foreach ($form as $key => $val) {
      if ($key[0] == '#') {
        if ($parent != '') {
          if ($form['#type'] != 'fieldset') {
            if (!$value_handled) {
              $form['#default_value'] = $data;
              $value_handled = TRUE;
            }
          }
        }
      }
      else {  //recurse into the next level
        $this->updateDefaultFormData($form[$key], $data[$key], $key);
      }
    }
  }

  function processInteractiveTask($taskid, $taskop = '') {
    $ret = new stdClass();
    $ret->retcode = TRUE;
    $ret->engineop = $taskop;

    $serializedData = db_query("SELECT task_data FROM {maestro_queue} WHERE id = :id", array(':id' => $taskid))->fetchField();
    $taskdata = @unserialize($serializedData);

    $tracking_id = db_select('maestro_process')
      ->fields('maestro_process', array('tracking_id'))
      ->condition('id', $this->_properties->process_id, '=')
      ->execute()->fetchField();

    unset($_POST['form_build_id']);
    unset($_POST['form_token']);
    unset($_POST['sec_token']);
    unset($_POST['op']);

    $this->saveProjectData($_POST, $tracking_id, 'inlineform');

    return $ret;
  }

}

