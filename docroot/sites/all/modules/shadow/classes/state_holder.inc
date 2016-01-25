<?php

class ShadowStateHolder {
  // Allow for easy access and readability in state_machine by
  // making these public.
  public $token_values;
  public $token_types;
  public $position;
  public $buffer;

  public function __construct() {
    $this->token_values = array();
    $this->token_types = array();
    $this->buffer = '';
    $this->position = 0;
  }
}
