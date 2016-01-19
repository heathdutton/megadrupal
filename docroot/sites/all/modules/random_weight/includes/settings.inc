<?php
/**
 * @file
 * Provides a set of helper methods to access and update settings.
 */

/**
 * Get all settings for each content type.
 *
 * @return array
 *   Returns an array of settings keyed by content type.
 */
function _random_weight_get_all_settings() {

  $settings = db_select('random_weight', 'rw')
    ->fields('rw')
    ->execute()
    ->fetchAllAssoc('type', PDO::FETCH_ASSOC);

  return $settings;
}

/**
 * Returns all the available settings for this content type.
 */
function _random_weight_get_settings($type) {

  $settings = _random_weight_get_all_settings();

  return isset($settings[$type]) ? $settings[$type] : array();
}

/**
 * Returns all the frequencies that are available to this module.
 *
 * @see hook_random_weight_frequency_info()
 * @see hook_random_weight_frequency_info_alter()
 */
function _random_weight_get_frequencies() {
  $frequencies = array();

  // Hook to add frequencies.
  $hook = 'random_weight_frequency_info';

  // Allow other modules to add their own settings.
  foreach (module_implements($hook) as $module) {
    // Add in the frequencies.
    $frequencies += module_invoke($module, $hook);
  }

  // Hook to edit frequencies.
  $hook = 'random_weight_frequency_info_alter';

  // Allow other modules to alter any frequencies.
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    $function($frequencies);
  }

  return $frequencies;
}

/**
 * Delete all the settings for this content type.
 *
 * @param string $type
 *   Content type to delete the settings for.
 */
function _random_weight_delete_settings($type) {
  db_delete('random_weight')
    ->condition('type', $type, '=')
    ->execute();
}

/**
 * Insert or update all settings.
 *
 * @param array $settings
 *   An array of settings keyed by content type.
 */
function _random_weight_save_settings($settings) {

  foreach ($settings as $type => $setting) {
    // Insert fields. Always set last run to 0.
    $insert_fields = $setting;
    $insert_fields['last_run'] = 0;

    // Update fields.
    $update_fields = $setting;

    // Insert or Update the settings.
    db_merge('random_weight')
      ->key(array('type' => $type))
      ->insertFields($insert_fields)
      ->updateFields($update_fields)
      ->execute();

  }
}
