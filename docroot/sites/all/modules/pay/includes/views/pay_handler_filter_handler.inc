<?php

class pay_handler_filter_handler extends views_handler_filter_in_operator {
  function construct() {
    parent::construct();
    $this->pay_type = $this->definition['pay_type'];
  }

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = array();
      // Provide a list of available handlers to select from.
      foreach (pay_handlers($this->pay_type) as $name => $info) {
        $this->value_options[$name] = $info['title'];
      }
    }
  }
}
