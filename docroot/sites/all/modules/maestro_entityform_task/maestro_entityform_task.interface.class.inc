<?php


/**
 * Maestro Admin Interface Class
 */

include_once './' . drupal_get_path('module', 'maestro') . '/maestro_task_interface.class.php';

class MaestroTaskInterfaceEntityform extends MaestroTaskInterface {
  function __construct($task_id=0, $template_id=0) {
    $this->_task_type = 'Entityform';
    $this->_is_interactive = MaestroInteractiveFlag::IS_INTERACTIVE;

    parent::__construct($task_id, $template_id);

    $this->_task_edit_tabs = array('assignment' => 1, 'notification' => 1);
  }

  function display() {
    return theme('maestro_task_entityform', array('tdid' => $this->_task_id, 'taskname' => $this->_taskname, 'ti' => $this));
  }

  function getEditFormContent() {
    $this->_fetchTaskInformation();
    $entityform_types = entity_load_multiple_by_name('entityform_type', FALSE);
    $entityforms = array();
    foreach ($entityform_types as $entity) {
      $entityforms[$entity->type] = $entity->label;
    }

    return theme('maestro_task_entityform_edit', array('tdid' => $this->_task_id, 'td_rec' => $this->_task_data, 'entityforms' => $entityforms));
  }

  function save() {
    if(!drupal_valid_token($this->_security_token,'maestro_admin')) {
      return array('message' => t('Illegal save attempt.'), 'success' => 0, 'task_id' => $this->_task_id);
    }
    $rec = new stdClass();
    $rec->id = intval($_POST['template_data_id']);
    $rec->task_data = serialize(array('entityform_id' => check_plain($_POST['entityform']), 'instance_name' => check_plain($_POST['instance_name'])));
    drupal_write_record('maestro_template_data', $rec, array('id'));
    $retval = parent::save();

    //clear the cache for maestro_content_types so on page load the new content type task will be added for sure
    cache_clear_all();

    return $retval;
  }
}


