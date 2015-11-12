<?php

/**
 * @file
 * Views Fast Field views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_fast_field_views_data_alter(&$data) {
  foreach ($data as $table_name => $table_info) {
    foreach ($table_info as $field_name => $field_info) {
      if (!empty($field_info['field']['handler']) && $field_info['field']['handler'] === 'views_handler_field_field') {
        $new_field = $field_info;
        $new_field['title'] = (!empty($field_info['field']['title']) ? $field_info['field']['title'] : $field_info['title']) . ' (fast)';
        $new_field['field']['handler'] = 'views_fast_field_handler';
        $new_field['field']['add fields to query'] = TRUE;
        unset($new_field['argument'], $new_field['filter'], $new_field['relationship'], $new_field['sort'], $new_field['field']['title'], $new_field['aliases']);
        $data[$table_name]['fastfield_' . $field_name] = $new_field;
      }
    }
  }
}
