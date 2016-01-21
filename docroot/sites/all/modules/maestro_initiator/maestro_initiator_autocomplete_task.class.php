<?php

/**
 * Tasks of this type will autocomplete tasks
 */
class MaestroTaskTypeAutocompleteTask extends MaestroTaskTypeBatchFunction {
  
  function execute() {
    $success = FALSE;

    $function = $this->_properties->handler;
    if (function_exists($function)) {
      $this->setTaskStartedDate($this->_properties->id);
      $success = $function($this->_properties->id,$this->_properties->process_id);
    } else {
      watchdog('maestro',"MaestroTaskTypeBatchFunction - unable to find the function: {$this->_properties->handler}");
    }
    
    //Assumption made here that the $success variable is set by the batch task.
    if ($success) {
      $this->completionStatus = MaestroTaskStatusCodes::STATUS_COMPLETE;
    }
    else {
      $this->completionStatus = MaestroTaskStatusCodes::STATUS_ON_HOLD;
    }
    $this->setTaskStartedDate($this->_properties->id);
    $this->executionStatus = TRUE;
    return $this;
  }

  function prepareTask() {
    $serializedData = db_query("SELECT task_data FROM {maestro_template_data} WHERE id = :tid",
    array(':tid' => $this->_properties->taskid))->fetchField();
    $taskdata = @unserialize($serializedData);
    return array('handler' => $taskdata['handler'],'serialized_data' => $serializedData);
  }
}
