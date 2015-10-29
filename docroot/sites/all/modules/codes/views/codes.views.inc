<?php
/**
 * @file
 * Provide views data and handlers for codes.module
 */


/**
 * Implements hook_field_views_data().
 */
function codes_field_views_data($field) {
  // Different filter handler for codes. This allows to select values
  // from the code list.
  $data = field_views_field_default_views_data($field);
  foreach ($data as $table_name => $table_data) {
    foreach ($table_data as $field => $field_data) {
      if (isset($field_data['filter'])) {
        $data[$table_name][$field]['filter']['handler'] = 'codes_handler_filter_field_codes';
      }
    }
  }
  return $data;
}
