<?php
/**
 * @file
 * sf-active migrate configuration.
 */

/**
 * Builds the sf-active migrate settings form.
 */
function sfactive_migrate_settings() {
  global $databases;
  $form['sfactive_migrate_database'] = array(
    '#default_value' => variable_get('sfactive_migrate_database', $databases['default']['default']['database']),
    '#description' => t('The database containing the sf-active site. Must be accessible via the same MySQL username and password as the Drupal database. It is recommended to use separate databases because both sf-active and Drupal have a role table, and to make it easier to cleanup the sf-active database tables later.'),
    '#title' => t('sf-active database'),
    '#type' => 'textfield',
  );
  $form['sfactive_migrate_files'] = array(
    '#default_value' => variable_get('sfactive_migrate_files', DRUPAL_ROOT),
    '#description' => t('The file path containing the sf-active uploads and images directories.'),
    '#title' => t('sf-active files path'),
    '#type' => 'textfield',
  );
  $form['sfactive_migrate_encoding'] = array(
    '#default_value' => variable_get('sfactive_migrate_encoding', 'Windows-1252'),
    '#description' => t('The encoding used when converting invalid UTF-8 data. The default Windows-1252 is recommended for most sites.'),
    '#title' => t('sf-active character encoding'),
    '#type' => 'textfield',
  );
  $form['sfactive_migrate_timezone'] = array(
    '#default_value' => variable_get('sfactive_migrate_timezone', variable_get('date_default_timezone', @date_default_timezone_get())),
    '#description' => t('The time zone applied to creation, modification and event dates in the sf-active database.'),
    '#options' => system_time_zones(),
    '#title' => t('sf-active time zone'),
    '#type' => 'select',
  );
  return system_settings_form($form);
}
