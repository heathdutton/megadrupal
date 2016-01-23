<?php

/**
 * @file
 * Contains Views functionality.
 */

/**
 * Implements hook_field_views_data().
 */
function country_field_field_views_data($field) {
  $data = field_views_field_default_views_data($field);
  foreach ($data as $table => $info) {
    $data[$table][$field['field_name'] . '_value']['filter'] = array(
      'handler' => 'views_handler_filter_country_field',
    );
  }

  return $data;
}

