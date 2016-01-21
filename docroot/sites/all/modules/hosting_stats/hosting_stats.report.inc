<?php
/**
 * @file
 * UI and collation functions for hosting statistics and parameters.
 */

/**
 * Provide admin settings form
 */
function hosting_stats_reporting($form, &$form_state) {
  $form = array();

  if (user_access('administer hosting stats')) {
    if (variable_get('hosting_stats_logging', 0)) {
      drupal_set_message(t('Performance logging is currently running. <a href="@link">Disable here</a>.', array('@link' => url('admin/config/hosting_stats'))), 'error', FALSE);
    }
    else {
      drupal_set_message(t('Performance logging is disabled. <a href="@link">Enable here</a>.', array('@link' => url('admin/config/hosting_stats'))), 'status', FALSE);
    }
  }

  // Get environmental information.
  $env = array(
    array('Drupal version', VERSION),
    array('MySQL version', Database::getConnection()->version()),
    array('PHP version', phpversion()),
    array('Drupal caching', variable_get('cache', FALSE) ? t('Enabled') : t('Disabled')),
    array('Preprocess CSS', variable_get('preprocess_css', 0) ? "On" : "Off"),
    array('Preprocess JavaScript', variable_get('preprocess_js', 0) ? "On" : "Off"),
  );
  $form['env'] = array(
    '#type' => 'fieldset',
    '#title' => t('Environment'),
  );
  variable_set('hosting_stats_env', serialize($env));

  if (!module_exists('systeminfo')) {
    $form['env']['#description'] = t('For more detailed information about Drupal, MySQL and PHP configuration, please install and enable the <a href="http://drupal.org/project/systeminfo">System Info</a> module.');
  }
  else {
    $form['env']['#description'] = t('For more detailed information about Drupal, MySQL and PHP configuration, view the <a href="/@url">System Info</a> module output.', array('@url' => 'admin/reports/systeminfo'));
  }

  $form['env']['report'] = array(
    '#markup' => theme('table', array('rows' => $env)),
  );

  $form['env']['ini'] = array(
    '#type' => 'fieldset',
    '#title' => t('PHP Configuration'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $ini = ini_get_all();
  $initable = array();
  foreach ($ini as $key => $value) {
    // Link each key to its documentation on php.net.
    $link = l($key, "http://php.net/{$key}", array('external' => TRUE));
    array_push($initable, array($link, $value['global_value'], $value['local_value']));
  }

  $form['env']['ini']['ini'] = array(
    '#markup' => theme('table', array('header' => array('', 'Global Value', 'Local Value'), 'rows' => $initable)),
  );

  // Run/display performance metrics.
  $form['perf'] = array(
    '#type' => 'fieldset',
    '#title' => t('Performance'),
    '#description' => t('Be aware that calculation of statistics will take some time. It is advisable to run during off-peak traffic hours.'),
  );

  $form['perf']['report'] = array(
    '#markup' => hosting_stats_report_markup(unserialize(variable_get('hosting_stats_perf', 'a:0:{}'))),
    '#suffix' => t('<div class="description"><strong>*</strong> Minimum and maximum time values of collected data, but not necessarily contiguous if recording was stopped and started.</div>'),
  );
  if (user_access('administer hosting stats')) {
    $form['perf']['hosting_stats_update'] = array(
      '#type' => 'submit',
      '#value' => t('Calculate performance statistics'),
      '#submit' => array('hosting_stats_perf_stats'),
    );
  }

  // Run/display database metrics.
  $form['db'] = array(
    '#type' => 'fieldset',
    '#title' => t('Database'),
    '#description' => t('Running calculations on very busy databases may cause this page to fail. Run at off-peak hours.'),
  );
  $form['db']['report'] = array(
    '#markup' => hosting_stats_report_markup(unserialize(variable_get('hosting_stats_db', 'a:0:{}'))),
  );
  if (user_access('administer hosting stats')) {
    $form['db']['hosting_stats_update'] = array(
      '#type' => 'submit',
      '#value' => t('Calculate database statistics'),
      '#submit' => array('hosting_stats_db_stats'),
    );
  }

  // Run/display filesystem metrics.
  $form['file'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filesystem'),
    '#description' => t('Running calculations on very large file systems may cause this page to fail. Run at off-peak hours.'),
  );
  $form['file']['report'] = array(
    '#markup' => hosting_stats_report_markup(unserialize(variable_get('hosting_stats_file', 'a:0:{}'))),
  );

  // AHAH wrapper.
  $form['hosting_stats_result'] = array(
    '#prefix' => '<div id="hosting-stats-emailed">',
    '#suffix' => '</div>',
  );

  if (user_access('administer hosting stats')) {
    $form['file']['hosting_stats_update'] = array(
      '#type' => 'submit',
      '#value' => t('Calculate filesystem statistics'),
      '#submit' => array('hosting_stats_file_stats'),
    );

    // Run all calculations.
    $form['hosting_stats_allstats'] = array(
      '#type' => 'submit',
      '#value' => t('Calculate all statistics'),
      '#submit' => array('hosting_stats_perf_stats', 'hosting_stats_db_stats', 'hosting_stats_file_stats'),
    );

    // Clear hosting_stats_log table.
    $form['hosting_stats_clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear performance log'),
      '#submit' => array('hosting_stats_clearlog'),
    );

    // Send aggregated report via email.
    // @todo Move into fieldset or actions.
    $form['hosting_stats_send'] = array(
      '#type' => 'button',
      '#prefix' => '<div id="hosting-stats-send-wrapper">',
      '#value' => t('Send data to host'),
      '#suffix' => '<div class="description">' . t('Sends report to email specified in !link.', array('!link' => l(t('module configuration'), 'admin/config/hosting-stats'))) . '</div></div>',
      '#ajax' => array(
        // 'event' => 'click',
        //'path' => 'admin/reports/hosting_stats/summary',
        'callback' => 'hosting_stats_reporting_email_ajax_callback',
        'method' => 'replace',
        'wrapper' => 'hosting-stats-emailed',
        'progress' => array(
          'type' => 'bar',
          'message' => t('Please wait...'),
        ),
      ),
    );

    $form['hosting_stats_send']['#attached']['css'] = array(drupal_get_path('module', 'hosting_stats') . '/hosting_stats.css');
  }

  return $form;
}

/**
 * Provide a default message if stats have not yet been generated.
 *
 * @param $aggregated_data
 *   An array of data, generated by one of the _perf_stats() functions.
 *
 * @todo Refactor this so it is handled in the theme.
 */
function hosting_stats_report_markup($aggregated_data) {
  if ($aggregated_data) {
    return theme('table', array('rows' => $aggregated_data));
  }
  return t('Statistics have not yet been calculated.');
}

/**
 * AJAX submit callback for emailing of reports.
 *
 * @todo Make a theme function for the status message.
 */
function hosting_stats_reporting_email_ajax_callback($form, $form_state) {
  module_load_include('inc', 'hosting_stats', 'hosting_stats.summary');
  $element = $form['hosting_stats_result'];
  $element['#markup'] = '<div class="messages status">';
  $element['#markup'] .= hosting_stats_send();
  $element['#markup'] .= '</div>';
  return $element;
}

/**
 * Calculate performance statistics and save to a variable.
 */
function hosting_stats_perf_stats() {
  // Total requests recorded.
  $total_requests = db_query("SELECT count(*) from {hosting_stats_log}")->fetchField();

  $requests = _hosting_stats_perf_stats_per_timestamp();
  $user_counts = _hosting_stats_perf_stats_per_usertype();

  $start_time = db_query("SELECT MIN(timestamp) from {hosting_stats_log}")->fetchField();
  $end_time = db_query("SELECT MAX(timestamp) from {hosting_stats_log}")->fetchField();

  $stats = array();
  array_push(
    $stats,
    array('Logging window *', date("D, j M, Y, G:i:s", $start_time) . ' - ' . date("D, j M, Y, G:i:s", $end_time)),
    array('Total logging time', round(sizeof($requests) / 60 / 60, 2) . " hours"),
    array('Logged requests', $total_requests)
  );
  if (isset($user_counts[0])) {
    array_push(
      $stats,
      array('Anonymous requests', $user_counts[0] . " (" . round($user_counts[0] / $total_requests * 100, 2) . "%)")
    );
  }
  if (isset($user_counts[1])) {
    array_push(
      $stats,
      array('Authenticated requests', $user_counts[1] . " (" . round($user_counts[1] / $total_requests * 100, 2) . "%)")
    );
  }
  if ($requests) {
    array_push(
      $stats,
      array('Maximum load', max($requests) . " req/sec"),
      array('Minimum load', min($requests) . " req/sec"),
      array('Average load', round(array_sum($requests) / sizeof($requests), 1) . " req/sec")
    );
  }

  variable_set("hosting_stats_perf", serialize($stats));
  drupal_set_message(t('Performance statistics report generated.'));
}

/**
 * Collates statistics on a per-second basis.
 *
 * This is used solely to find min and max values. Each item in the list is the
 * count for a single second.
 *
 * @return array
 *   A list of timestamps per second.
 */
function _hosting_stats_perf_stats_per_timestamp() {
  return db_query("SELECT COUNT(*) AS req FROM {hosting_stats_log} GROUP BY timestamp")->fetchCol();
}

/**
 * Collates statistics on a per-usertype basis (anonymous, authenticated).
 *
 * @return array
 *   An array of request counts, keyed to 0 for anon requests and 1 for auth.
 */
function _hosting_stats_perf_stats_per_usertype() {
  return db_query("SELECT usertype, COUNT(*) AS req FROM {hosting_stats_log} GROUP BY usertype")->fetchAllKeyed();
}

/**
 * Calculate DB statistics and save to variable
 */
function hosting_stats_db_stats() {
  // Node & user counts.
  $node_count = db_query("SELECT COUNT(*) FROM {node}")->fetchField();
  $user_count = db_query("SELECT COUNT(*) FROM {users}")->fetchField();
  // Database size and missing indexes.
  $noindex = array();
  $db_size = 0;
  $result = db_query("SHOW TABLE STATUS");
  foreach ($result as $row) {
    if ($row->Name != 'hosting_stats_log') {
      $db_size += $row->Index_length;
      $db_size += $row->Data_length;
      // Check that each table has indices.
      // Note that you cannot paramaterize tablenames with PDO, so we build the
      // query manually.
      $query = sprintf("SHOW INDEX FROM %s", $row->Name);
      if (!db_query($query)->fetchField()) {
       array_push($noindex, $row->Name);
      }
    }
  }

  // Stats array to return.
  $stats = array();

  $result = db_query("SHOW STATUS");
  foreach ($result as $row) {
    if (in_array($row->Variable_name, array('Slow_queries', 'Threads_running', 'Queries', 'Uptime', 'Questions', 'Open_tables'))) {
      array_push($stats, array(str_replace("_", " ", $row->Variable_name), $row->Value));
    }
  }

  array_push(
    $stats,
    array('Total nodes', $node_count),
    array('Total users', $user_count),
    array('Database size (excluding hosting stats table)', round($db_size / 1024 / 1024, 2) . " MB"),
    array('Tables without index', implode(", ", $noindex))
  );
  variable_set("hosting_stats_db", serialize($stats));
  drupal_set_message(t('Database statistics report generated.'));
}

/**
 * Calculate filesystem statistics and save to variable.
 */
function hosting_stats_file_stats() {
  // File size and count, supporting drush too.
  $files = file_scan_directory(file_default_scheme() . '://', "/.*/");
  $filesize = 0;
  $filecount = 0;
  foreach ($files as $file) {
    $filesize += filesize($file->filename);
    $filecount++;
  }
  $stats = array();
  array_push(
    $stats,
    array('Total Files', $filecount),
    array('Total File Size', round($filesize / 1024 / 1024, 2) . " MB")
  );
  variable_set("hosting_stats_file", serialize($stats));
  drupal_set_message(t('File statistics report generated.'));
}

/**
 * Clear contents of log table.
 */
function hosting_stats_clearlog() {
  db_truncate('hosting_stats_log')->execute();
}
