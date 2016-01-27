<?php

/**
 * @file
 * Views filter handler to decide whether items with parent should be shown
 */
class views_handler_filter_boolean_operator_ref_parent extends views_handler_filter_boolean_operator {
  function construct() {
    parent::construct();
    $this->accept_null = TRUE;
  }
}