<?php

/**
 * @file
 * Views integration for Nepali calendar module.
 */

/**
 * Implements hook_views_data_alter().
 */
function nepali_calendar_views_data_alter(&$data) {
  foreach ($data as $table => $config) {
    foreach ($config as $item => $item_config) {
      if (isset($item_config['field']) && !empty($item_config['field']['handler'])) {
        if ($item_config['field']['handler'] == 'views_handler_field_date') {
          $data[$table][$item]['field']['handler'] = 'nepali_calendar_views_handler_field_date';
        }
      }
    }
  }

  return $data;
}
