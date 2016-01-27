<?php

/**
 * @file
 * Provides information about views_dow module to views.
 */

/**
 * Implements hook_views_data_alter().
 */
function views_dow_views_data_alter(&$data) {
  $field = variable_get('views_dow_field');
  if ($field) {
    $data['field_data_' . $field][$field . '_value']['filter']['handler'] = 'views_dow_handler_filter';
  }
}
