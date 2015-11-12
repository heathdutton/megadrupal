<?php


/**
 * Maestro Admin Interface Class
 */

include_once './' . drupal_get_path('module', 'maestro') . '/maestro_task_interface.class.php';

class MaestroTaskInterfaceWebform extends MaestroTaskInterface {
  function __construct($task_id=0, $template_id=0) {
    $this->_task_type = 'Webform';
    $this->_is_interactive = MaestroInteractiveFlag::IS_INTERACTIVE;

    parent::__construct($task_id, $template_id);

    $this->_task_edit_tabs = array('assignment' => 1, 'notification' => 1);
  }

  function display() {
    return theme('maestro_task_webform', array('tdid' => $this->_task_id, 'taskname' => $this->_taskname, 'ti' => $this));
  }

  function getEditFormContent() {
    $this->_fetchTaskInformation();
    $query = new EntityFieldQuery();
    $entities = $query->entityCondition('entity_type', 'node', '=')
                      ->entityCondition('bundle', 'webform')
                      ->propertyCondition('status', 1, '=')
                      ->execute();
    $nodes = entity_load('node', array_keys($entities['node']));
    $webforms = array();
    foreach ($nodes as $node) {
      $webforms[$node->nid] = $node->title;
    }

    return theme('maestro_task_webform_edit', array('tdid' => $this->_task_id, 'td_rec' => $this->_task_data, 'webforms' => $webforms));
  }

  function save() {
    if(!drupal_valid_token($this->_security_token,'maestro_admin')) {
      return array('message' => t('Illegal save attempt.'), 'success' => 0, 'task_id' => $this->_task_id);
    }
    $rec = new stdClass();
    $rec->id = intval($_POST['template_data_id']);
    $rec->task_data = serialize(array('webform_nid' => check_plain($_POST['webform']), 'instance_name' => check_plain($_POST['instance_name'])));
    drupal_write_record('maestro_template_data', $rec, array('id'));
    $retval = parent::save();

    //clear the cache for maestro_content_types so on page load the new content type task will be added for sure
    cache_clear_all();

    return $retval;
  }
}


