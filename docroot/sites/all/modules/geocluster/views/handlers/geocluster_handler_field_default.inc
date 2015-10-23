<?php

/**
 * @file
 * Geocluster field implementation.
 *
 * Based on Geofield, see geocluster_views_data_alter.
 */

class geocluster_handler_field_default extends views_handler_field {

  function render($values) {
    $field_name = $this->field;
    if (isset($values->$field_name)) {
      return $values->$field_name;
    }
    return "";
  }

  function query() {
  }

}
