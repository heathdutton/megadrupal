<?php

/**
 * Implements hook_field_views_data().
 */
function hms_field_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  // Probably not needed.
  if ($field['type'] !== 'hms_field') {
    return $data;
  }
  // Add a handler for countries.
  foreach ($field['storage']['details']['sql'] as $type => $tables) {
    foreach ($tables as $table_name => $columns) {
      $column_real_name = $columns['value'];
      if (!isset($data[$table_name][$column_real_name]['filter'])) {
        continue;
      }
      $data[$table_name][$column_real_name]['filter']['handler'] = 'hms_field_views_handler_filter_hms';
    }
  }
  return $data;
}
