<?php

/**
 * @file
 * Views definitions for Hijri module.
 */

/**
 * Implements hook_views_data_alter().
 */
function hijri_views_data_alter(&$data) {
  foreach ($data as $table => $config) {
    foreach ($config as $item => $item_config) {
      if (isset($item_config['field']['handler']) && $item_config['field']['handler'] == 'views_handler_field_date') {
        $data[$table][$item]['field']['handler'] = 'hijri_handler_hijri_created';
      }
    }
  }

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function hijri_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'hijri') . '/views',
    ),
    'handlers' => array(
      'hijri_handler_hijri_created' => array(
        'parent' => 'views_handler_field_date',
      ),
    ),
  );
}
