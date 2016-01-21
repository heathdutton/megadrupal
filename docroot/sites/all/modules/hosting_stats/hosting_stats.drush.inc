<?php

/**
 * @file hosting_stats.drush.inc
 * Drush functions.
 */

include_once dirname(__FILE__) . '/hosting_stats.report.inc';
include_once dirname(__FILE__) . '/hosting_stats.summary.inc';

/**
 * Implements hook_drush_command().
 */
function hosting_stats_drush_command() {
  $items = array();

  // Enable performance logging.
  $items['hd-enable'] = array(
    'callback' => '_hosting_stats_logging_enable',
    'description' => 'Enable Hosting Diagnostics performance logging.',
    // 'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
  );

  // Disable performance logging.
  $items['hd-disable'] = array(
    'callback' => '_hosting_stats_logging_disable',
    'description' => 'Disable Hosting Diagnostics performance logging.',
    // 'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
  );

  // Calculate statistics
  $items['hd-calculate'] = array(
    'callback' => '_hosting_stats_calculate',
    'arguments' => array(
      'type' => 'Which statistics group to calculate',
    ),
    'description' => 'Run calculations of Hosting Diagnostics parameters and logs.',
  );

  // Clear performance log
  $items['hd-clearlog'] = array(
    'callback' => '_hosting_stats_drush_clearlog',
    'description' => 'Clear the Hosting Diagnostics performance log to save database space.',
  );

  // Show/email/export statistics
  $items['hd-stats'] = array(
    'callback' => '_hosting_stats_display',
    'arguments' => array(
      'format' => 'How to format output of Hosting Diagnostics.',
    ),
    'description' => 'Return collected data as text table, CSV or via email.',
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function hosting_stats_drush_help($section) {
  switch ($section) {
    case 'drush:hd-enable':
      return dt("Enables logging of performance data (total requests, requests per second, anonymous vs. authenticated users). Note that this will consume database space.");
    case 'drush:hd-disable':
      return dt("Disable data logging.");
    case 'drush:hd-calculate':
      return dt("Run statistics calculations. Note that some caclulations may take longer than others, and could potentially cause performance degradations, depending on the size of your database and filesystem.");
    case 'drush:hd-clearlog':
      return dt("Purge Hosting Diagnostics data log.");
    case 'drush:hd-stats':
      return dt("Export or email data in varying formats.");
  }
}


/**
 * Enable performance logging.
 */
function _hosting_stats_logging_enable() {
  variable_set('hosting_stats_logging', 1);
  drush_print(t('Enabled performance logging.'));
}

/**
 * Disable performance logging.
 */
function _hosting_stats_logging_disable() {
  variable_set('hosting_stats_logging', 0);
  drush_print(t('Disabled performance logging.'));
}

/**
 * Callback to clear performance log.
 */
function _hosting_stats_drush_clearlog() {
  if (call_user_func('hosting_stats_clearlog')) {
    drush_log(dt("Performance log cleared"), 'success');
  }
  else {
    drush_log(dt("Failed to clear performance log"), 'error');
  }
}

/**
 * Run calculation functions to populate results variables.
 */
function _hosting_stats_calculate($type = NULL) {
  $types = _hosting_stats_stats_types();
  if ($type) {
    drush_op('call_user_func', $types[$type]);
    drush_log(dt("'!name' statistics were calculated", array('!name' => $type)), 'success');
  }
  else {
    $choice = drush_choice($types, 'Enter a number to choose which statistics to calculate.', '!key');
    if ($choice !== FALSE) {
      call_user_func($types[$choice]);
      drush_log(dt("'!name' statistics were calculated", array('!name' => $choice)), 'success');
    }
  }
}

/**
 * Types of data to calculate/compile.
 */
function _hosting_stats_stats_types() {
  $types = array(
    'all' => '_hosting_stats_all',
    'perf' => 'hosting_stats_perf_stats',
    'db' => 'hosting_stats_db_stats',
    'file' => 'hosting_stats_file_stats',
  );
  return $types;
}

/**
 * Clear all stats
 */
function _hosting_stats_all() {
  call_user_func('hosting_stats_perf_stats');
  call_user_func('hosting_stats_db_stats');
  call_user_func('hosting_stats_file_stats');
}

/**
 * Display/email/export data.
 */
function _hosting_stats_display($format = NULL) {
  $formats = _hosting_stats_display_formats();
  if ($format) {
    drush_op('call_user_func', $formats[$format]);
  }
  else {
    $choice = drush_choice($formats, 'Enter a number to choose how to deliver data. Note that you should run "drush hd-calculate" first!', '!key');
    if ($choice !== FALSE) {
      call_user_func($formats[$choice]);
    }
  }
}

/**
 * Report formats.
 */
function _hosting_stats_display_formats() {
  $formats = array(
    'text' => '_hosting_stats_display_text',
    'csv' => '_hosting_stats_display_csv',
    'e-mail' => '_hosting_stats_display_email',
  );
  return $formats;
}

/**
 * Return a table of stats.
 */
function _hosting_stats_display_text() {
  drush_print_table(array_merge(array(array(dt("Environment"))), unserialize(variable_get('hosting_stats_env', 'a:0:{}'))), TRUE, array(50, 30));
  drush_print("\n");
  drush_print_table(array_merge(array(array(dt("Performance"))), unserialize(variable_get('hosting_stats_perf', 'a:0:{}'))), TRUE, array(50, 30));
  drush_print("\n");
  drush_print_table(array_merge(array(array(dt("MySQL"))), unserialize(variable_get('hosting_stats_db', 'a:0:{}'))), TRUE, array(50, 30));
  drush_print("\n");
  drush_print_table(array_merge(array(array(dt("Filesystem"))), unserialize(variable_get('hosting_stats_file', 'a:0:{}'))), TRUE, array(50, 30));
  drush_print("\n");

  // Add php info
  $ini = ini_get_all();
  $initable = array();
  foreach ($ini as $key => $value) {
    array_push($initable, array($key, $value['global_value'], $value['local_value']));
  }
  $header = array(dt("PHP Variable"), dt("Global"), dt("Local"));
  array_unshift($initable, $header);
  drush_print_table($initable, TRUE);
}

function _hosting_stats_display_csv() {
  drush_print(dt('"Environment"'));
  drush_print(_hosting_stats_tocsv(unserialize(variable_get('hosting_stats_env', 'a:0:{}'))));
  drush_print(dt('"Performance"'));
  drush_print(_hosting_stats_tocsv(unserialize(variable_get('hosting_stats_perf', 'a:0:{}'))));
  drush_print(dt('"MySQL"'));
  drush_print(_hosting_stats_tocsv(unserialize(variable_get('hosting_stats_db', 'a:0:{}'))));
  drush_print(dt('"Filesystem"'));
  drush_print(_hosting_stats_tocsv(unserialize(variable_get('hosting_stats_file', 'a:0:{}'))));

  // Add php info
  $ini = ini_get_all();
  $initable = array();
  foreach ($ini as $key => $value) {
    array_push($initable, array($key, $value['global_value'], $value['local_value']));
  }
  $header = array(dt("Variable"), dt("Global"), dt("Local"));
  array_unshift($initable, $header);
  drush_print(dt('"PHP Variables"'));
  drush_print(_hosting_stats_tocsv($initable));
}

/**
 * Convert array to CSV string.
 */
function _hosting_stats_tocsv($arr) {
  $str = '';
  for ($i = 0; $i < sizeof($arr); $i++) {
    $str .= '"' . implode('","', $arr[$i]) . "\"\n";
  }
  return $str;
}

/**
 * Send report via email.
 */
function _hosting_stats_display_email() {
  call_user_func('hosting_stats_send');
}
