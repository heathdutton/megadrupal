<?php

/**
 * @file
 * Page callbacks and utility functions for IP anonymize module.
 */

/**
 * Anonymize IP addresses which have exceeded the retention period. 
 */
function ip_anon_scrub() {
  foreach (ip_anon_tables() as $table => $columns) {
    db_update($table)
      ->fields(array($columns['hostname'] => 0))
      ->condition($columns['timestamp'], REQUEST_TIME - variable_get('ip_anon_period_'. $table, 21600), '<')
      ->execute();
  }
}

/**
 * Configuration options for IP anonymize.
 */
function ip_anon_settings() {
  $form = array();
  $form['ip_anon_policy'] = array(
    '#type' => 'radios',
    '#title' => t('Retention policy'),
    '#options' => array(0 => t('Preserve IP addresses'), 1 => t('Anonymize IP addresses')),
    '#description' => t('This setting may be used to temporarily disable IP anonymization.'),
    '#default_value' => variable_get('ip_anon_policy', 0),
  );
  $form['ip_anon_period'] = array(
    '#type' => 'fieldset',
    '#title' => t('Retention period'),
    '#description' => t('IP addresses older than the retention period will be anonymized.'),
  );
  $options = drupal_map_assoc(array(0, 30, 60, 120, 180, 300, 600, 900, 1800, 2700, 3600, 5400, 7200, 10800, 21600, 32400, 43200, 64800, 86400, 172800, 259200, 345600, 604800, 1209600, 2419200, 4838400, 9676800, 31536000), 'format_interval');
  foreach (ip_anon_tables() as $table => $columns) {
    $schema = drupal_get_schema($table);
    $schema = drupal_get_schema_unprocessed($schema['module'], $table);
    $form['ip_anon_period']['ip_anon_period_' . $table] = array(
      '#type' => 'select',
      '#title' => t('@table table', array('@table' => $table)),
      '#options' => $options,
      '#default_value' => variable_get('ip_anon_period_' . $table, 21600),
      '#description' => t($schema['description']),
    );
  }
  return system_settings_form($form);
}


/**
 * Default array of hostname and timestamp columns.
 */
function ip_anon_columns() {
  return drupal_map_assoc(array('hostname', 'timestamp'));
}

/**
 * Array of tables and columns which store hostnames and timestamps.
 *
 * Modules may add tables by implementing hook_ip_anon_alter().
 */
function ip_anon_tables() {
  $tables = array('sessions' => ip_anon_columns());
  drupal_alter('ip_anon', $tables);
  return $tables;
}

/**
 * Implements hook_ip_anon_alter() for comment module.
 */
function comment_ip_anon_alter(&$tables) {
  $tables['comment'] = array(
    'hostname' => 'hostname',
    'timestamp' => 'changed',
  );
}

/**
 * Implements hook_ip_anon_alter() for dblog module.
 */
function dblog_ip_anon_alter(&$tables) {
  $tables['watchdog'] = ip_anon_columns();
}

/**
 * Implements hook_ip_anon_alter() for statistics module.
 */
function statistics_ip_anon_alter(&$tables) {
  $tables['accesslog'] = ip_anon_columns();
}

/**
 * Implements hook_ip_anon_alter() for Voting API module.
 */
function votingapi_ip_anon_alter(&$tables) {
  $tables['votingapi_vote'] = array(
    'hostname' => 'vote_source',
    'timestamp' => 'timestamp',
  );
}

/**
 * Implements hook_ip_anon_alter() for webform module.
 */
function webform_ip_anon_alter(&$tables) {
  $tables['webform_submissions'] = array(
    'hostname' => 'remote_addr',
    'timestamp' => 'submitted',
  );
}
