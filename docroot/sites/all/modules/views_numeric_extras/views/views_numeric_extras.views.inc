<?php

/**
 * @file
 *   Views API hooks
 */

/**
 * Implements hook_views_data_alter().
 */
function views_numeric_extras_views_data_alter(&$data) {
  // Iterate the Views data
  foreach ($data as &$table) {
    // Iterate each table field
    foreach ($table as &$field) {
      // Search for numeric field handlers and swap in our own extender
      // field handler.
      if (is_array($field)) {
        array_walk_recursive($field, '_views_numeric_extras_change_handler');
      }
    }
  }
}

/**
 * Helper function for changing registered numeric field handlers to our
 * custom, extended handler.
 */
function _views_numeric_extras_change_handler(&$value, $key) {
  if ($key === 'handler') {
    if ($value == 'views_handler_field_numeric') {
      $value = 'views_handler_field_numeric_extended';
    }
  }
}
