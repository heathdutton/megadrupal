<?php
/**
 * @file
 * Administration settings for the simpletest_fixture module
 */


/**
 * Administration settings for the simpletest_fixture module
 * settings to avoid copying content for several tables
 * @return mixed
 */
function simpletest_fixture_admin_settings() {

  $form = array();

  // get te table names
  $tables = simpletest_fixture_get_table_names();

  $form['simpletest_fixture_exclude_tables'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exclude tables contents'),
    '#description' => t('Checked tables will be excluded in fixture generate.
      The table structure will be copied but the content is not.
    '),
    '#options' => $tables,
    '#default_value' => simpletest_fixture_admin_exclude_tables(),
  );

  return system_settings_form($form);
}

/**
 * Get the default settings for the tables that should be excluded
 * @return array
 */
function simpletest_fixture_admin_exclude_tables() {
  $tables = variable_get('simpletest_fixture_exclude_tables', array());
  if (!empty($tables)) {
    // Exclude tables defined by user
    return $tables;
  }

  // Exclude standard set of tables

  $all_tables = simpletest_fixture_get_table_names();
  if (empty($all_tables)) {
    return $tables;
  }

  foreach ($all_tables as $table_name) {
    // cache tables
    if (strpos($table_name, 'cache') !== FALSE) {
      $tables[$table_name] = $table_name;
    }
    // watchdog
    if (strpos($table_name, 'watchdog') !== FALSE) {
      $tables[$table_name] = $table_name;
    }
    // blocked_ips
    if (strpos($table_name, 'blocked_ips') !== FALSE) {
      $tables[$table_name] = $table_name;
    }
    // flood
    if (strpos($table_name, 'flood') !== FALSE) {
      $tables[$table_name] = $table_name;
    }
    // queue
    if (strpos($table_name, 'queue') !== FALSE) {
      $tables[$table_name] = $table_name;
    }
    // sessions
    if (strpos($table_name, 'sessions') !== FALSE) {
      $tables[$table_name] = $table_name;
    }
  }

  return $tables;
}

/**
 * Get all tables of the Drupal schema
 * @return array $tables
 */
function simpletest_fixture_get_table_names() {

  // static caching
  $tables = drupal_static(__FUNCTION__);

  if (!isset($tables)) {
    $schemas = drupal_get_complete_schema($rebuild = FALSE);
    // sort the table schemas for a better overview
    ksort($schemas);
    foreach ($schemas as $table_name => $schema) {
      $tables[$table_name] = $table_name;
    }
  }
  return $tables;
}
