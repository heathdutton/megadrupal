<?php

/**
 * Maestro Admin Interface Class
 */

include_once './' . drupal_get_path('module', 'maestro') . '/maestro_task_interface.class.php';

class MaestroTaskInterfaceInlineFormAPI extends MaestroTaskInterface {
  function __construct($task_id=0, $template_id=0) {
    $this->_task_type = 'InlineFormAPI';
    $this->_is_interactive = MaestroInteractiveFlag::IS_INTERACTIVE;

    parent::__construct($task_id, $template_id);

    $this->_task_edit_tabs = array('assignment' => 1, 'notification' => 1);
  }

  function display() {
    return theme('maestro_task_inline_form_api', array('tdid' => $this->_task_id, 'taskname' => $this->_taskname, 'ti' => $this));
  }

  function getEditFormContent() {
    $this->_fetchTaskInformation();
    if (!is_array(@($this->_task_data->task_data)) || !array_key_exists('form_api_code', $this->_task_data->task_data)) {
      $this->_task_data->task_data['form_api_code'] = '';
    }

    return theme('maestro_task_inline_form_api_edit', array('tdid' => $this->_task_id, 'td_rec' => $this->_task_data));
  }

  function save() {
    $rec = new stdClass();
    $rec->id = $_POST['template_data_id'];
    $rec->task_data = serialize(array('form_api_code' => filter_xss($_POST['form_api_code']), 'content_type' => check_plain($_POST['content_type']) ));

    drupal_write_record('maestro_template_data', $rec, array('id'));

    return parent::save();
  }
}


