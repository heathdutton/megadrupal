<?php
/**
 * @file
 * API for Settings.php generator Drush commands.
 */

/**
 * Modify settings before generating settings.php contents.
 *
 * @param $settings
 *  Array of settings
 *
 * @see drupal_rewrite_settings()
 */
function hook_settingsphp_generate_alter(&$settings) {
  $settings['update_free_access'] = array(
    'value' => TRUE,
  );
  // Pass 'required' => TRUE because $base_url variable
  // is not declared in default.settings.php -- it's commented out.
  // $base_url will be added at the end of the file.
  $settings['base_url'] = array(
    'value' => 'http://example.com',
    'required' => TRUE,
  );
}

