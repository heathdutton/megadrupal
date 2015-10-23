<?php

/**
 * @file
 * Rijksoverheid Cookie Opt-in Log menu callback functions.
 */

/**
 * Log report menu callback function.
 */
function ro_cookie_opt_in_log_report() {
  if (arg(3) == 'export') {
    _ro_cookie_opt_in_log_export();
  }
  else {
    $build = array();

    // If cookie consent logging is not enabled, show a warning.
    if (!variable_get('ro_cookie_opt_in_log_enabled', TRUE)) {
      drupal_set_message(t('You have not enabled the cookie consent logging functionality. At the moment no cookie preferences are logged in the Drupal database. You can enable logging at the <a href="@url">module settings page</a>', array('@url' => url('admin/config/system/ro-cookie-opt-in'))), 'warning', FALSE);
    }

    // Get the header.
    $header = _ro_cookie_opt_in_log_header();

    // Get the log entries.
    $lines = _ro_cookie_opt_in_log_get_log_rows(50, $header);

    // Build output render array.
    // Add the filter form.
    $build['filter_form'] = drupal_get_form('ro_cookie_opt_in_log_filter_form');

    // Add the log entry table.
    $build['log'] = array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $lines,
      '#empty' => t('No log entries available.'),
    );

    // Add the pager.
    $build['pager'] = array(
      '#theme' => 'pager',
      '#tags' => array(),
    );

    $build['export'] = array(
      '#markup' => l(t('export as csv file'), 'admin/reports/cookielog/export', array(
        'query' => drupal_get_query_parameters(),
        'attributes' => array(
          'class' => array(
            'button',
          ),
        ),
      )),
    );

    return $build;
  }
}

/**
 * Function that generates a csv export of the log.
 */
function _ro_cookie_opt_in_log_export() {
  // Get the log entries.
  $rows = _ro_cookie_opt_in_log_get_log_rows(-1, _ro_cookie_opt_in_log_header());

  $filename = 'export-' . date('YmdHi') . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $filename);

  $header = array(
    'Number',
    'Timestamp',
    'Response',
    'Unique ID',
  );
  print implode(';', $header) . "\r\n";

  foreach ($rows as $row) {
    foreach ($row['data'] as $value) {
      if ($value == NULL) {
        $value = '-';
      }
      $values[] = '"' . str_replace('"', '""', decode_entities(strip_tags($value))) . '"';
    }
    print implode(';', $values) . "\r\n";
    unset($values);
  }
}

/**
 * Function to retrieve log rows.
 */
function _ro_cookie_opt_in_log_get_log_rows($number = -1, $header = array()) {
  $rows = array();

  // Create the select statement with pager and header sort capabilities.
  $query = db_select('ro_cookie_opt_in_log', 'log')
    ->fields('log')
    ->extend('TableSort')
    ->orderByHeader($header);

  if ($number > -1) {
    $query->extend('PagerDefault')
    ->limit($number);
  }

  // Add the filter conditions if any are set.
  if (!empty($_SESSION['ro_cookie_opt_in_log_filter']['response'])) {
    $query->condition('log.response', $_SESSION['ro_cookie_opt_in_log_filter']['response'], '=');
  }
  if (!empty($_SESSION['ro_cookie_opt_in_log_filter']['unique_id'])) {
    $query->condition('log.unique_id', '%' . $_SESSION['ro_cookie_opt_in_log_filter']['unique_id'] . '%', 'LIKE');
  }

  $result = $query->execute();

  // Loop through the result and add the entries to the rows array.
  foreach ($result as $entry) {
    $rows[] = array(
      'data' => array(
        $entry->resp_id,
        format_date($entry->timestamp, 'short'),
        $entry->response,
        $entry->unique_id,
      ),
    );
  }

  return $rows;
}

/**
 * Helper function to construct the header.
 */
function _ro_cookie_opt_in_log_header() {
  // Construct the header.
  $header = array(
    array(
      'data' => t('Number'),
      'field' => 'resp_id',
    ),
    array(
      'data' => t('Date and time'),
      'field' => 'timestamp',
      'sort' => 'desc',
    ),
    array(
      'data' => t('Response'),
      'field' => 'response',
    ),
    array(
      'data' => t('Unique ID'),
      'field' => 'unique_id',
    ),
  );

  return $header;
}

/**
 * Return form for cookielog filters.
 */
function ro_cookie_opt_in_log_filter_form($form) {
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter log'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($_SESSION['ro_cookie_opt_in_log_filter']),
  );

  // Add the response filter.
  $form['filters']['response'] = array(
    '#title' => t('Filter by Response'),
    '#type' => 'select',
    '#options' => array(
      variable_get('ro_cookie_opt_in_cookie_accept', 'yes') => variable_get('ro_cookie_opt_in_cookie_accept', 'yes'),
      variable_get('ro_cookie_opt_in_cookie_deny', 'no') => variable_get('ro_cookie_opt_in_cookie_deny', 'no'),
    ),
  );
  if (!empty($_SESSION['ro_cookie_opt_in_log_filter']['response'])) {
    $form['filters']['response']['#default_value'] = $_SESSION['ro_cookie_opt_in_log_filter']['response'];
  }

  // Add the response filter.
  $form['filters']['unique_id'] = array(
    '#title' => t('Filter by Unique ID'),
    '#type' => 'textfield',
    '#size' => 25,
    '#description' => t('Enter (a part of) the Unique ID you are looking for.'),
  );
  if (!empty($_SESSION['ro_cookie_opt_in_log_filter']['unique_id'])) {
    $form['filters']['unique_id']['#default_value'] = $_SESSION['ro_cookie_opt_in_log_filter']['unique_id'];
  }

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  if (!empty($_SESSION['ro_cookie_opt_in_log_filter'])) {
    $form['filters']['actions']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }

  return $form;
}

/**
 * Validate result from cookielog filter form.
 */
function ro_cookie_opt_in_log_filter_form_validate($form, &$form_state) {
  // Check if any filter conditions were submitted.
  if ($form_state['values']['op'] == t('Filter') && empty($form_state['values']['response']) && empty($form_state['values']['unique_id'])) {
    form_set_error('type', t('You must select something to filter by.'));
  }
}

/**
 * Process result from dblog administration filter form.
 */
function ro_cookie_opt_in_log_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  switch ($op) {
    case t('Filter'):
      // Put the response filter value in the session.
      if (isset($form_state['values']['response'])) {
        $_SESSION['ro_cookie_opt_in_log_filter']['response'] = $form_state['values']['response'];
      }
      if (isset($form_state['values']['unique_id'])) {
        $_SESSION['ro_cookie_opt_in_log_filter']['unique_id'] = $form_state['values']['unique_id'];
      }
      break;

    case t('Reset'):
      $_SESSION['ro_cookie_opt_in_log_filter'] = array();
      break;

  }

  return 'admin/reports/cookielog';
}
