<?php

class MaestroTaskInterfaceAutocompleteTask extends MaestroTaskInterfaceBatchFunction {
  function __construct($task_id = 0, $template_id = 0) {
    parent::__construct($task_id, $template_id);
    $this->_task_type = 'AutocompleteTask';
    $this->_task_edit_tabs = array('optional' => 1, 'notification' => 1);
  }
  
  function display() {
    return theme('maestro_initiator_autocomplete_task', array('tdid' => $this->_task_id, 'taskname' => $this->_taskname, 'ti' => $this));
  }
}
