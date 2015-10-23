<?php
/**
 * @file
 * Functions for compatibility between dbee module and Views module
 */

/**
 * Implements hook_views_data_alter().
 */
function dbee_views_data_alter(&$data) {
  foreach ($data as $table => $config) {
    foreach ($config as $item => $item_config) {
      if (isset($item_config['field']['handler']) && $item_config['field']['handler'] == 'views_handler_field_user_mail') {
        $data[$table][$item]['field']['handler'] = 'views_handler_field_dbee_user_mail';
      }
    }
  }
}
