<?php
/**
 * @file
 * Integration with Views.
 */

/**
 * Implementation of hook_views_data_alter()
 */
function raty_input_views_data_alter(&$data) {
  foreach ($data as $table_name => $table_info) {
    foreach ($table_info as $field_name => $field_info) {
      if (!empty($field_info['filter']['handler'])
        && $field_info['filter']['handler'] == 'views_handler_filter_numeric'
        && $field_info['filter']['handler'] != 'views_handler_filter_raty') {

        $raty_field = $field_info;
        $raty_field['title'] = (!empty($field_info['filter']['title']) ? $field_info['filter']['title'] : $field_info['title']) . ' (Raty stars)';
        $raty_field['filter']['handler'] = 'views_handler_filter_raty';
        $raty_field['filter']['field'] = $field_name;
        unset($raty_field['argument'], $raty_field['field'], $raty_field['relationship'], $raty_field['sort'], $raty_field['filter']['title']);

        $data[$table_name][$field_name . '_raty'] = $raty_field;

      }
    }
  }
}

/**
 * Implementation of hook_views_handlers()
 */
function raty_input_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'raty_input') . '/views',
    ),
    'handlers' => array(
      'views_handler_filter_raty' => array(
        'parent' => 'views_handler_filter_numeric',
      ),
    ),
  );
}
