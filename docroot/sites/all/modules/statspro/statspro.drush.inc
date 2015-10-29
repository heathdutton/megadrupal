<?php
// $Id: statspro.drush.inc,v 1.2.2.2.2.7 2010/10/29 12:19:34 rsevero Exp $

/**
 * @file
 *   Drush include file for statspro
 *
 */


/**
 * Implements hook_drush_help().
 */
function statspro_drush_help($section) {
  switch ($section) {
    case 'drush:statspro':
      return dt('Usage: drush [options] statspro statspro_options

        statspro_options - one of the following: alert, aliases, comments,
          critical, emergency, error, modules, nodes, node_types,
          path_aggregated, pi, sessions, terms, ualert, ucritical, uemergency,
          uerror, upi, users, uwarning or warning

        for options alert, comments, critical, emergency, error, nodes,
          path_aggregated, pi, sessions, ualert, ucritical, uemergency, uerror,
          upi, users, uwarning and warning one extra parameter is recognized for
          period definition. It should be one of the following options: today,
          yesterday, week_current, week_last, week_last2, month_current,
          month_last, month_last3, month_last6, quarter_current, quarter_last,
          year_current, year_last or custom_days

        for custom_days another extra parameter is recognized. It should be an
          integer meanning the number of days to include in the report.');
  }
}

/**
 * Implements hook_drush_command().
 *
 * @return array
 */
function statspro_drush_command() {
  $items = array();

  $items['statspro'] = array(
    'callback' => 'statspro_drush_run',
    'description' => 'statspro drush commands',
  );

  return $items;
}

/**
 * Run drush command for specified mode
 *
 */
function statspro_drush_run() {
  $modes = func_get_args();

  if (empty($modes)) {
    drush_die('Missing mode: use drush help statspro for more information');
  }

  drush_print(t('Statistics Pro information'));

  if ($modes[0] == 'path_aggregated') {
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro_path_aggregated.inc';
    $period = isset($modes[1]) ? $modes[1] : variable_get('statspro_period', 'today');
    $number_of_days = isset($modes[2]) ? (int) $modes[2] : variable_get('statspro_custom_number_days', 30);
    $period_info = statspro_get_period_info($period, $number_of_days);
    if ($period_info['period'] === NULL) {
      drush_print(t('Unknown period %period.', array('%period' => $period)));
      return;
    }
    $table_data = statspro_path_aggregated_get_data_for_configurable_period_report($period_info['period']);
    if (!is_array($table_data) || !isset($table_data['rows']) || (count($table_data['rows']) == 0)) {
      drush_print(t('No path aggregated data available for period %period_name.', array('%period_name' => $period_info['period_name'])));
      return;
    }
    drush_print(t('Path aggregated report for period %period_name.', array('%period_name' => $period_info['period_name'])));
    $lines = statspro_get_array_as_fixed_space_fields($table_data['headers'], $table_data['rows']);
    drush_print(implode("\n", $lines));
  }
  else {
    require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'statspro') . '/statspro.inc';
    $statspro = new StatsPro();
    $rc = $statspro->get_aggregate_stat($modes[0]);
    if (is_array($rc)) {
      drush_print($rc['subject'] . ': ' . $rc['amount']);
    }
    else {
      drush_die('Unknown mode');
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function statspro_get_array_as_fixed_space_fields($headers, $rows) {
  $column_widths = array();
  foreach ($headers as $header) {
    $column_widths[] = drupal_strlen($header);
  }
  foreach ($rows as $row) {
    $index = 0;
    foreach ($row as $cell) {
      $width = drupal_strlen($cell);
      if (!isset($column_widths[$index]) || ($width > $column_widths[$index])) {
        $column_widths[$index] = $width;
      }
      $index++;
    }
  }

  $lines = array();
  $lines[] = _statspro_get_array_as_fixed_space_fields_line($column_widths, $headers);
  foreach ($rows as $row) {
    $lines[] = _statspro_get_array_as_fixed_space_fields_line($column_widths, $row);
  }

  return $lines;
}

function _statspro_get_array_as_fixed_space_fields_line($widths, $row) {
  $line = '';
  $index = 0;
  foreach ($row as $cell) {
    $line .= '|' . $cell . str_repeat(' ', $widths[$index] - drupal_strlen($cell));
    $index++;
  }
  if ($line) {
    $line .= '|';
  }
  return $line;
}
