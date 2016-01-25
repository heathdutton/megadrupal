<?php

class XtoolsAssert {

  public $message = '';

  public $valid = FALSE;

  function __construct($valid, $message) {
    $this->caller = _drupal_get_last_caller(debug_backtrace());
    $this->message = $message;
    $this->valid = $valid;
  }
}