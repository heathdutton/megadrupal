<?php

/*
 * @file
 * Admin menu callbacks for cd_sunlight.
 */

/**
 * Form builder for admin settings form.
 *
 * @ingroup forms
 *
 * @see cd_sunlight_settings_form_validate()
 */
function cd_sunlight_settings_form($form, &$form_state) {

  $form['cd_sunlight_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Sunlight API Key'),
    '#description' => t('A key can be obtained from <a href="!url">Sunlight Labs</a>.',
      array('!url' => 'http://services.sunlightlabs.com/api/')),
    '#default_value' => variable_get('cd_sunlight_api_key', ''),
    '#required' => TRUE,
  );
  $form['cd_sunlight_cron_contact_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Cron contact limit'),
    '#description' => t('The maximum number of contacts to process during cron.'),
    '#default_value' => variable_get('cd_sunlight_cron_contact_limit', 200),
    '#required' => TRUE,
  );
  $form['cd_sunlight_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debugging'),
    '#description' => t('Extra log entries will be made to aid in debugging.'),
    '#default_value' => variable_get('cd_sunlight_debug', FALSE),
  );
  if (function_exists('sys_getloadavg')) {
    $form['cd_sunlight_cron_load_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Cron server load limit'),
      '#description' => t('Do not run Cron CD lookups if the system load is higher than this value. '
        . 'Only applicable to *nix based servers (Linux, OS X, Solaris, BSD, etc.). '),
      '#default_value' => variable_get('cd_sunlight_cron_load_limit', 6),
    );
  }

  $form = system_settings_form($form);
  return $form;
}

/**
 * Form validator for admin settings form.
 *
 * @ingroup forms
 *
 * @see cd_sunlight_settings_form()
 */
function cd_sunlight_settings_form_validate($form, &$form_state) {

  // Temporarily replace the existing API key.
  $key_existing = variable_get('cd_sunlight_api_key', '');
  variable_set('cd_sunlight_api_key', $form_state['values']['cd_sunlight_api_key']);

  // Test the new one.
  if (!_cd_sunlight_connection_test()) {
    form_set_error('cd_sunlight_api_key',
      t('Either the Sunlight key is not valid, or the Sunlight API is down. ' .
      'See watchdog for more detailed information.'));
  }
  else {
    drupal_set_message(t('The Sunlight API key works.'));
  }

  // Revert.
  variable_set('cd_sunlight_api_key', $key_existing);

  // Are the contact limit and load limit numeric?
  if (!empty($form_state['values']['cd_sunlight_cron_contact_limit'])) {
    $contact_limit = $form_state['values']['cd_sunlight_cron_contact_limit'];
    if (!is_numeric($contact_limit) || (int) $contact_limit != $contact_limit) {
      form_set_error('cd_sunlight_cron_contact_limit',
        t('The contact limit must be an integer.'));
    }
  }
  if (!empty($form_state['values']['cd_sunlight_cron_load_limit'])) {
    $cron_limit = $form_state['values']['cd_sunlight_cron_load_limit'];
    if (!is_numeric($cron_limit) || $cron_limit < 0) {
      form_set_error('cd_sunlight_cron_load_limit',
        t('The cron limit must be a positive numeric value.'));
    }
  }
}

/**
 * Confirm that we will batch process all contacts.
 *
 * @ingroup forms
 */
function cd_sunlight_batch_confirm($form, &$form_state) {
  return confirm_form(array(),
    t('Process all contacts'),
    'admin/config/services/cd_sunlight',
    t('Are you sure you want to process all contacts, queuing those that require a Congressional ' .
      'District? This action iterates through all contacts, and may be a lengthy process.'),
    t('Process all contacts'),
    t('Cancel')
  );
}

/**
 * Handler for batch confirmation.
 *
 * @see cd_sunlight_batch_finished()
 */
function cd_sunlight_batch_confirm_submit($form, &$form_state) {
  $batch = array(
    'finished' => 'cd_sunlight_batch_finished',
    'title' => t('Processing contacts.'),
    'init_message' => t('Starting.'),
    'error_message' => t('An error occurred and some contacts have not been processed.'),
    'file' => drupal_get_path('module', 'cd_sunlight') . '/cd_sunlight.admin.inc',
    'operations' => array(),
  );
  if (function_exists('_cd_civicrm_batch')) {
    $batch['operations'][] = array('_cd_civicrm_batch', array());
  }
  batch_set($batch);
}

/**
 * Batch finished callback.
 *
 * @see cd_sunlight_batch_confirm_submit()
 */
function cd_sunlight_batch_finished($success, $results, $operations) {
  if ($success) {
    if ($results) {
      $message = format_plural($results, 'One contact processed.', '@count contacts processed.');
    }
    else {
      $message = t('No contacts to process.');
    }
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);
}

/**
 * Menu Callback to display {cd_sunlight_cron}.
 */
function cd_sunlight_log() {
  $rows = array();
  $header = array(
    array(
      'data' => t('Time'),
      'field' => 'insert_time',
      'sort' => 'DESC',
    ),
    array(
      'data' => t('Storage System'),
      'field' => 'storage_system',
    ),
    array(
      'data' => t('Contact'),
      'field' => 'contact_id',
    ),
    array(
      'data' => t('Processed'),
      'field' => 'processed',
    ),
    array(
      'data' => t('URI'),
      'field' => 'request_uri',
    ),
  );
  if (variable_get('cd_sunlight_debug', FALSE)) {
    $header[] = t('Backtrace');
  }

  // Build data.
  $query = db_select('cd_sunlight_cron', 'csc', array('target' => 'slave'))
    ->extend('PagerDefault')
    ->extend('TableSort');
  $query->fields('csc');
  $query->orderByHeader($header)
    ->limit(50);
  $records = $query->execute();

  foreach ($records as $record) {

    // Get a link to the contact.
    $contact_link = $record->contact_id;
    $function = 'cd_' . $record->storage_system . '_contact_link';
    if (function_exists($function)) {
      $contact_link = $function($record->contact_id, $record->contact_id);
    }

    // Build row.
    $row = array(
      format_date($record->insert_time),
      $record->storage_system,
      $contact_link,
      $record->processed,
      check_plain($record->request_uri),
    );
    if (variable_get('cd_sunlight_debug', FALSE)) {
      $row[] = '<pre>' . check_plain($record->backtrace) . '</pre>';
    }
    $rows[] = $row;
  }

  // Create output;
  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager', array('quantity' => 50));
  return $output;
}

/**
 * Menu Callback to display summary analysis of {cd_sunlight_cron}.
 */
function cd_sunlight_log_summary() {
  $output = '';

  // Totals.
  $query = db_select('cd_sunlight_cron');
  $query->addExpression('MIN(insert_time)');
  $first_record = format_date($query->execute()->fetchField());
  $query = db_select('cd_sunlight_cron');
  $query->addExpression('MAX(insert_time)');
  $last_record = format_date($query->execute()->fetchField());
  $output .= '<p>' . t('Log contains data from : %first_record to %last_record',
      array('%first_record' => $first_record, '%last_record' => $last_record)) . '</p>';
  $query = db_select('cd_sunlight_cron')->condition('processed', 0);
  $query->addExpression('COUNT(*)');
  $total_unprocessed = $query->execute()->fetchField();
  $query = db_select('cd_sunlight_cron')->condition('processed', 1);
  $query->addExpression('COUNT(*)');
  $total_processed = $query->execute()->fetchField();
  $output .= '<p>' . t('Total records in the queue: %count',
    array('%count' => number_format($total_processed + $total_unprocessed))) . '</p>';
  $output .= '<p>' . t('Total records that have been processed: %count',
    array('%count' => number_format($total_processed))) . '</p>';
  $output .= '<p>' . t('Total records that have not been processed: %count',
    array('%count' => number_format($total_unprocessed))) . '</p>';

  // Most active contacts.
  $header = array('Contact', 'Count');
  $rows = array();
  $query = db_select('cd_sunlight_cron', 'csc')
    ->fields('csc', array('contact_id', 'storage_system'));
  $count_alias = $query->addExpression('COUNT(*)', 'cnt');
  $records = $query->groupBy('contact_id')
    ->groupBy('storage_system')
    ->orderBy($count_alias, 'DESC')
    ->range(0, 10)
    ->execute();
  foreach ($records as $record) {
    $contact_link = $record->contact_id;
    $function = 'cd_' . $record->storage_system . '_contact_link';
    if (function_exists($function)) {
      $contact_link = $function($record->contact_id, $record->contact_id);
    }
    $rows[] = array($contact_link, $record->$count_alias);
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'caption' => t('Most active contacts')));

  // Most common URIs.
  $header = array('URI', 'Count');
  $rows = array();
  $query = db_select('cd_sunlight_cron', 'csc')
    ->fields('csc', array('request_uri'));
  $count_alias = $query->addExpression('COUNT(*)', 'cnt');
  $records = $query->groupBy('request_uri')
    ->orderBy($count_alias, 'DESC')
    ->range(0, 10)
    ->execute();
  foreach ($records as $record) {
    $rows[] = array($record->request_uri, $record->$count_alias);
  }
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'caption' => t('Most common URIs')));

  // Most active periods of time.
  // 1NN seconds => left trim length of unix timestamp
  $time_periods = array(
    '00' => 8,
    '000' => 7,
    '0000' => 6,
    '00000' => 5,
  );
  foreach ($time_periods as $seconds => $left_trim) {
    $header = array('Period', 'Queued contacts');
    $rows = array();
    $query = db_select('cd_sunlight_cron', 'csc');
    $period_alias = $query->addExpression('LEFT(insert_time, :left_trim)', 'period', array(':left_trim' => $left_trim));
    $count_alias = $query->addExpression('COUNT(*)', 'cnt');
    $records = $query->groupBy($period_alias)
      ->orderBy($count_alias, 'DESC')
      ->range(0, 10)
      ->execute();
    foreach ($records as $record) {
      $start = format_date($record->$period_alias . $seconds);
      $end = format_date(($record->$period_alias + 1) . $seconds);
      $row = array(
        $start . ' - ' . $end,
        $record->$count_alias,
      );
      $rows[] = $row;
    }
    $output .= theme('table', array('header' => $header, 'rows' => $rows,
      'caption' => t('Most active @seconds second period', array('@seconds' => number_format('1' . $seconds)))));
  }

  return $output;
}
