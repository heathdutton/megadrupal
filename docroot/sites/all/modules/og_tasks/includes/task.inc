<?php
/**
 * @file
 * Include file containing the og_task class.
 */

class og_task {
  public $title = '';
  public $description = '';
  public $required = FALSE;
  public $completed = FALSE;
  public $callback = NULL;
  public $file = NULL;

  public function __construct($options = array()) {
    foreach ($options as $key => $value) {
      if (property_exists($this, $key)) {
        $this->$key = $value;
      }
    }
  }

  public function get_status() {
    $status = ($this->completed ? OG_TASKS_COMPLETED : ($this->required ? OG_TASKS_REQUIRED : OG_TASKS_OPTIONAL));
    return $status;
  }

  public function get_callback_string() {
    if (is_array($this->callback)) {
      return $this->callback[0] .'::'. $this->callback[1];
    }
    return $this->callback;
  }

  public function execute($entity, &$context) {
    if ($this->completed) {
      return FALSE;
    }

    if (!empty($this->file)) {
      require_once($this->file);
    }

    if (is_callable($this->callback)) {
      call_user_func($this->callback, $entity, $this, $context);
    }
    else {
      watchdog('og_tasks', 'Invalid callback: the callback (%callback) specified by the %title task is not callable.', 
        array('%callback' => $this->get_callback_string(), '%title' => $this->title), WATCHDOG_ERROR);
    }
  }
}
