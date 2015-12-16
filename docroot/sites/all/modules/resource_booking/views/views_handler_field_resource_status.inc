<?php
/**
 * @file
 * Provide handler to show resources.
 */

class views_handler_field_resource_status extends views_handler_field  {
  /**
   * Get the value that's supposed to be rendered.
   *
   * @param $values
   *   An object containing all retrieved values.
   * @param $field
   *   Optional name of the field where the value is stored.
   */
  function pre_render(&$values) {
    $status = _resource_booking_field_get_rb_status();
    foreach ($values as $row => $data) {
      $item = (array) $data;
      foreach ($item as $field_name => $value) {
        if (strpos($field_name, 'rb_status') !== FALSE) {
          $data->$field_name = $status[$value];
          break;
        }
      }
    }
  }
}
