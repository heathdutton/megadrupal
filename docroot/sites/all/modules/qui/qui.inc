<?php
/**
 * @file
 * Includes misc functionality for DQUI.
 */

/**
 * Gets the display friendly status of the report.
 *
 * @param QuiReport $report
 *   QuiReport to get the status of.
 *
 * @return string
 *   Warning message if one needs to show.
 */
function _qui_report_status(QuiReport $report) {
  switch ($report->status) {
    case ENTITY_OVERRIDDEN:
      return 'Overridden';
    case ENTITY_IN_CODE:
      return 'In Code';
    default:
      return 'In Database';
  }
}

/**
 * Display a report table.
 *
 * @param QuiReport $report
 *   DQUI Report.
 *
 * @return string
 *   Returns HTML output.
 */
function qui_view_report_callback(QuiReport $report) {
  return entity_get_controller('qui_report')->view(array($report), 'table');
}

/**
 * Callback for viewing a report.
 */
function qui_view_formatted_report_callback(QuiReport $report, $formatter) {
  $cache_name = __FUNCTION__ . '_' . md5(request_uri());

  $output = '';

  if (isset($formatter['headers'])) {
    foreach ((array) $formatter['headers'] as $header => $value) {
      drupal_add_http_header($header, $value);
    }
  }

  if (!isset($_GET['clear-cache']) && $cache = cache_get($cache_name)) {
    if ($cache->expire > REQUEST_TIME) {
      $cache_expire = $cache->expire - REQUEST_TIME;
      drupal_add_http_header('Cache-Control', "max-age={$cache_expire}, must-revalidate");
      drupal_add_http_header('Expires', gmdate('D, d M Y H:i:s \G\M\T', $cache->expire));
      $output = $cache->data;
    }
  }

  // Get the output if we didn't pull it from the cache.
  if (empty($output)) {
    $output = entity_view('qui_report', array($report), $formatter['key']);

    // If plain, don't "render" the output. Rendering wraps things in HTML.
    if ($formatter['plain']) {
      $output = $output['qui_report'][$report->identifier()]['formatted']['#markup'];
    }
    else {
      $output = render($output);
    }

    // Only re-cache if the user isn't bypassing cache on purpose.
    if (!isset($_GET['clear-cache'])) {
      cache_set($cache_name, $output, 'cache', $report->settings->cache_time + REQUEST_TIME);
    }
  }

  $gzip = variable_get('qui_gzip_format_output', TRUE);
  if ($gzip && $formatter['plain']) {
    header('Content-Encoding: gzip');
    header('Accept-Encoding: gzip, deflate');
    ob_start('ob_gzhandler');
  }

  if ($formatter['plain']) {
    // Page will be plain, not inside the site.
    print $output;
  }
  else {
    return $output;
  }

  if ($gzip && $formatter['plain']) {
    ob_end_flush();
  }
}

/**
 * Download a formatted report using the proper headers.
 *
 * @param QuiReport $report
 *   DQUI Report.
 * @param array $formatter
 *   Formatter.
 */
function qui_download_formatted_report_callback(QuiReport $report, array $formatter) {
  if (empty($formatter['download headers'])) {
    // Set default headers if none exist.
    $formatter['download headers'] = array('Content-Type' => 'text/plain');
  }
  foreach ((array) $formatter['download headers'] as $header => $value) {
    drupal_add_http_header($header, $value);
  }
  $ext = isset($formatter['download extension']) ? $formatter['download extension'] : 'txt';
  $filename = $report->machine_name . '_' . date('Y-m-d') . '.' . $ext;
  header("Content-Disposition: attachment; filename={$filename}");

  $output = entity_view('qui_report', array($report), $formatter['key']);
  $output = $output['qui_report'][$report->identifier()]['formatted']['#markup'];

  print $output;
}

/**
 * Check if this user can access this report.
 *
 * @param QuiReport $report
 *   DQUI Report.
 *
 * @return bool
 *   Returns TRUE if user can access the page.
 */
function qui_view_formatted_report_access(QuiReport $report) {
  global $user;

  if ($user->uid == 1) {
    return TRUE;
  }

  // Check if anonymous user is passing an access token.
  if ($user->uid == 0 &&
    $report->settings->access_token &&
    isset($_GET['access_token'])
  ) {
    if (check_plain($_GET['access_token']) === $report->settings->access_token) {
      // Correct Access Token passed to view the report.
      return TRUE;
    }
  }

  return qui_report_access('view', $report, $user);
}

/**
 * Formats a report.
 *
 * @param QuiReport $report
 *   DQUI Report.
 * @param string $format
 *   Formatter.
 *
 * @return string
 *   Formatted report output.
 */
function qui_get_formatted_report(QuiReport $report, $format) {
  $results = $report->runQuery();
  return qui_get_formatted_results($report, $results, $format);
}

/**
 * Formats results from a report.
 *
 * @param QuiReport $report
 * @param array $results
 * @param $format
 * @return string
 */
function qui_get_formatted_results(QuiReport $report, array $results = array(), $format) {
  return theme('qui_formatter_' . $format, array('results' => $results, 'report' => $report));
}

/**
 * Loads a specific formatter.
 *
 * @param string $format
 *   Format key.
 *
 * @return array
 *   Formatter.
 */
function qui_formatter_load($format) {
  $formatters = qui_get_formatters();
  $formatters[$format]['key'] = $format;
  return $formatters[$format];
}


/**
 * Get all reports from the system.
 *
 * @return array
 *   Array of QUI Reports.
 */
function _qui_get_all_reports() {
  $reports = entity_load('qui_report');
  return $reports;
}

/**
 * Replaces the query token if there is one present.
 *
 * @param string $param
 *   Conditional value.
 *
 * @return string
 *   Param value, replaced with the $_GET param if there is one.
 */
function _qui_format_get_param($param) {
  // @todo: Need to figure out how this should work.
  return $param;
}

/**
 * Filters tables based on QUI settings.
 */
function _qui_filter_tables($var) {
  $show_revision_tables = variable_get('qui_show_revision_tables', FALSE);
  $show_cache_tables = variable_get('qui_show_cache_tables', FALSE);
  if (!$show_revision_tables && strpos($var, 'revision') !== FALSE) {
    return FALSE;
  }
  if (!$show_cache_tables && strpos($var, 'cache') !== FALSE) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Get conditions for SQL.
 */
function _qui_get_conditions() {
  return array(
    'lt' => '<',
    'lte' => '<=',
    'e' => '=',
    'gt' => '>',
    'gte' => '>=',
    'ne' => '!=',
    'in' => 'IN',
    'not_in' => 'NOT IN',
    'like' => 'LIKE',
    'not_like' => 'NOT LIKE',
    'is_null' => 'IS NULL',
    'is_not_null' => 'IS NOT NULL',
  );
}

/**
 * Load a report by it's machine name.
 */
function qui_report_load_by_machine_name($machine_name) {
  $reports = entity_load_multiple_by_name('qui_report', array($machine_name));
  return reset($reports);
}

/**
 * Ensure this machine name is available.
 */
function _qui_machine_name_exists($machine_name) {
  if (qui_report_load_by_machine_name($machine_name)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Sort by weight on sub fields.
 */
function _qui_weight_sort($a, $b) {
  if ($a['weight'] == $b['weight']) {
    return 0;
  }
  return ($a['weight'] < $b['weight']) ? -1 : 1;
}