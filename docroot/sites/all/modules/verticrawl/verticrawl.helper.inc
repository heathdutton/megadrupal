<?php
/**
 * @file
 * Helper class to manage configuration data.
 */

/**
 * Verticrawl Helper Class.
 *
 * @category Class
 */
class VerticrawlHelper {

  /**
   * Save configuration in 'verticrawl_configs' table.
   */
  public static function saveConfig($data) {
    $table = 'verticrawl_configs';
    $primary_key = 'config_id';

    $entry_exists = db_select($table, 't')
      ->fields('t', array($primary_key))
      ->condition($primary_key, $data[$primary_key], '=')
      ->execute()
      ->rowCount();

    if ($entry_exists) {
      drupal_write_record($table, $data, $primary_key);
    }
    else {
      drupal_write_record($table, $data);
    }
  }

  /**
   * Get configuration value from 'verticrawl_configs' table.
   */
  public static function getConfigValue($primary_key_value, $default_value = NULL) {
    $config_value = db_select('verticrawl_configs', 'v')
      ->fields('v', array('config_value'))
      ->condition('config_id', $primary_key_value, '=')
      ->execute()
      ->fetchAssoc();

    $config_value = is_array($config_value) && array_key_exists('config_value', $config_value) ? $config_value['config_value'] : $default_value;

    return $config_value;
  }

}
