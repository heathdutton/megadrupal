<?php

/**
 * @file
 * Provide views handler.
 */

/**
 * Implements hook_field_views_data().
 */
function rut_field_field_views_data($field) {

  $data = field_views_field_default_views_data($field);
  $field_name = $field['field_name'];
  foreach ($data as $table_name => $table_data) {
    if (isset($data[$table_name][$field_name])) {
      $data[$table_name][$field_name]['filter'] = array(
        'field' => $field_name,
        'field_name' => $field_name,
        'table' => $data[$table_name][$field_name]['field']['table'],
        'handler' => 'rut_field_handler_filter_rut',
        'allow empty' => 1,
        'additional fields' => $data[$table_name][$field_name]['field']['additional fields'],
      );
    }
  }

  return $data;

}
