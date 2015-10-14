<?php
// $Id $

define('DBA_STATPACK_INTERVAL', 30);

/**
 * Display database report.
 */
function dba_statpack_page() {
  $form = array();
  if (isset($_SESSION['dba_statpack_reports']) && is_array($_SESSION['dba_statpack_reports'])) {
    $reports = array();
    foreach ($_SESSION['dba_statpack_reports'] as $report) {
      $reports[] = $report;
    }
    unset($_SESSION['dba_statpack_reports']);
    if (isset($_SESSION['dba_statpack_interval'])) {
      $interval = $_SESSION['dba_statpack_interval'];
      unset($_SESSION['dba_statpack_interval']);
    }
    $form['view_report'] = array(
      '#type' => 'fieldset',
      '#title' => t('Database statistics report'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $name = dba_invoke_driver_specific('name', NULL, 'report');
    $uptime = dba_invoke_driver_specific('uptime', $reports[0], 'statpack');
    $output = dba_statpack_header($uptime, $interval);
    $output = array_merge($output, dba_invoke_driver_specific('output', $reports, 'statpack'));

    $display = '' ;
    foreach ($output as $line) {
      if (substr($line, 0, 1) == '_') {
        $display .= "\n" ;
      }
      $display .= $line . "\n" ;
    }

    $form['view_report']['report'] = array(
      '#type' => 'markup',
      '#markup' => "<pre>$display</pre>",
    );
  }
  $form['run_report'] = array(
    '#type' => 'fieldset',
    '#title' => t('Generate database statistics report'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['run_report']['interval'] = array(
    '#title' => t('Snapshot interval'),
    '#description' => t('Specify how long an interval between snapshots.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(5, 15, 30, 45, 60, 120, 180, 240, 300, 600, 900, 1800, 3600), 'format_interval'),
  );
  $form['run_report']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Generate report'),
  );
  return $form;
}

/**
 * Batch process statpack report.
 */
function dba_statpack_page_submit($form, &$form_state) {
  dba_statpack_batch_setup($form_state['values']['interval']);
}

/**
 *
 */
function dba_statpack_batch_setup($interval) {
  $batch = array(
    'title' => t('Collecting database statistics'),
    'operations' => array(
      array('dba_statpack_batch_snapshot', array($interval)),
    ),
    'finished' => 'dba_statpack_batch_complete',
    'init_message' => t('Taking initial database statistics snapshot.'),
    'progress_message' => '',
    'error_message' => t('Whoops! We ran into an unexpected error.'),
    'file' => drupal_get_path('module', 'dba') . '/dba.statpack.inc',
  );
  batch_set($batch);
}

function dba_statpack_batch_snapshot($interval, &$context) {
  // Prepare to loop until it's time to take another snapshot.
  if (empty($context['sandbox'])) {
    // re-use code from dba_report
    $context['results'][] = dba_invoke_driver_specific('status', NULL, 'report');
    $context['sandbox']['time'] = time();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $interval;
  }
  sleep(1);
  $context['sandbox']['progress']++;
  $current_time = time();
  $remaining = $interval - ($current_time - $context['sandbox']['time']);
  $context['message'] = t('Taking next snapshot in %time seconds', array('%time' => $remaining));
  $context['finished'] = ($current_time - $context['sandbox']['time']) / $interval;

  if ($remaining <= 0) {
    $context['results'][] = dba_invoke_driver_specific('status', NULL, 'report');
    $_SESSION['dba_statpack_interval'] = $interval;
  }
}

function dba_statpack_batch_complete($success, $results, $operations) {
  if ($success) {
    $_SESSION['dba_statpack_reports'] = $results;
  }
  else {
    // Cleanup if necessary
    if (isset($_SESSION['dba_statpack_reports'])) {
      unset($_SESSION['dba_statpack_reports']);
    }
    if (isset($_SESSION['dba_statpack_interval'])) {
      unset($_SESSION['dba_statpack_interval']);
    }
    drupal_set_message(t('Unexpected error; failed to generate report.'));
  }
}

/**
 * Add a line of === like standard statpack output.
 */
function dba_statpack_line() {
  return str_repeat('=', 100);
}

/**
 * Center text like standard statpack output.
 */
function dba_statpack_center($text, $length = 100) {
  $space = ($length - strlen($text)) / 2;
  return str_repeat(' ', $space) . $text;
}

/**
 * Display statpack columns.
 */
function dba_statpack_columns($columns = array()) {
  $output = '';
  for ($col = 0; $col < 4; $col++) {
    switch ($col) {
      case 0:
        $output .= sprintf('%28s', $columns[$col]);
        break;
      case 1:
        $output .= sprintf('%17s', $columns[$col]);
        break;
      case 2:
      case 3:
        $output .= sprintf('%25s', $columns[$col]);
        break;
    }
  }
  return $output;
}

/**
 * Format number.
 */
function dba_statpack_short($number, $kb = FALSE, $d = 2) {
  $n = 0;

  $format= array();

  if ($kb) {
    $format = array('b','Kb','Mb','Gb','Tb');
    while ($number > 1023) {
      $number /= 1024;
      $n++;
    }
  }
  else {
    $format = array('','K','M','G','T') ;
    while ($number > 999) {
      $number /= 1000;
      $n++;
    }
  }

  $short = sprintf("%.${d}f%s", $number, $format[$n]) ;

  if (preg_match('/^(.+)\.00$/', $short, $matches)) {
    return $matches[1] ; // Convert 12.00 to 12, but not 12.00kb to 12kb
  }

  return $short;
}

/**
 * Return value as percentage.
 */
function dba_statpack_to_percentage($fraction, $integer = FALSE) {
  if ($integer) {
    return (int)$fraction * 100 ;
  }
  else {
    return floatval($fraction * 100) ;
  }
}

/**
 * Display report header.
 */
function dba_statpack_header($uptime, $interval) {
  $output = array();
  $output[] = dba_statpack_line();
  $text = t('Uptime: ') . $uptime . t(' Snapshot Period: !interval second interval', array('!interval' => $interval));
  $output[] = dba_statpack_center($text);
  $output[] = dba_statpack_line();
  $output[] = dba_statpack_columns(array(t('Variable'), t('Delta/Percentage'), t('Per Second'), t('Total')));
  $output[] = dba_statpack_line();
  return $output;
}
