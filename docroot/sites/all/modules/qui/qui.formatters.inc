<?php
/**
 * @file
 * Contains various formatters provided by this module.
 *
 * @see qui_qui_formatters_info()
 */

/**
 * Displays the results in an Table table.
 */
function qui_formatter_table($vars) {
  $results = $vars['results'];
  // Format rows with 'data'. This is important because $results can
  // contain a column called 'data' which will mess with the theme_table call.
  $rows = array();
  foreach ($results as $row) {
    // Avoid XSS attacks from dangerous data.
    $row = array_map('check_plain', (array) $row);
    $rows[] = array(
      'data' => $row,
    );
  }
  // Allow other modules to modify the rows.
  drupal_alter('qui_formatter_table_alter_rows', $rows);
  return theme('table', array('header' => array_keys($rows[0]['data']), 'rows' => $rows));
}

/**
 * Displays the results in JSON format.
 */
function qui_formatter_json($vars) {
  $results = $vars['results'];
  return drupal_json_encode($results);
}

/**
 * Displays the results in PHP serialized format.
 */
function qui_formatter_php($vars) {
  $results = $vars['results'];
  return serialize($results);
}

/**
 * Displays the results in CSV format.
 */
function qui_formatter_csv($vars) {
  $results = $vars['results'];
  $handle = fopen('php://temp/maxmemory:' . (5 * 1024 * 1024), 'r+');
  fputcsv($handle, array_keys((array) $results[0]), ',');
  array_walk($results, function (&$v, $k) use ($handle) {
    $v = (array) $v;
    fputcsv($handle, $v, ',', '"');
  });
  rewind($handle);
  $output = stream_get_contents($handle);
  fclose($handle);
  return $output;
}

/**
 * Displays the results in TSV format.
 */
function qui_formatter_tsv($vars) {
  $results = $vars['results'];
  $handle = fopen('php://temp/maxmemory:' . (5 * 1024 * 1024), 'r+');
  fputcsv($handle, array_keys((array) $results[0]), "\t");
  array_walk($results, function (&$v, $k) use ($handle) {
    $v = (array) $v;
    fputcsv($handle, $v, "\t", '"');
  });
  rewind($handle);
  $output = stream_get_contents($handle);
  fclose($handle);
  return $output;
}

/**
 * Displays the results in Pipe delimited format.
 */
function qui_formatter_pipe($vars) {
  $results = $vars['results'];
  $handle = fopen('php://temp/maxmemory:' . (5 * 1024 * 1024), 'r+');
  fputcsv($handle, array_keys((array) $results[0]), '|');
  array_walk($results, function (&$v, $k) use ($handle) {
    $v = (array) $v;
    fputcsv($handle, $v, '|', '"');
  });
  rewind($handle);
  $output = stream_get_contents($handle);
  fclose($handle);
  return $output;
}
