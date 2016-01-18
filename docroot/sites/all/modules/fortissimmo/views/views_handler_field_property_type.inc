<?php

/**
 * @file
 * Fortissimmo pand "Type" field handler
 */

class views_handler_field_property_type extends views_handler_field {
  function construct() {
    parent::construct();
    $types = fortissimmo_get_types();
    $this->type_mapping = $types;
  }

  function render($values) {
    return $this->type_mapping[$values->{$this->field_alias}];
  }
}