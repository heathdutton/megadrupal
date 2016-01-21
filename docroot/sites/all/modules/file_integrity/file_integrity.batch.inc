<?php 
/**
 * @file
 * Batch processing for file integrity.
 */

/**
 * Create and save MD5 fingerprint for a single file.
 */
function file_integrity_create_md5($file, &$context) {
  // Optional message displayed under the progressbar.
  $context['message'] = t('Fingerprinting file "@file"', array('@file' => $file));
  $tainted = FALSE;
  if (is_readable ($file)) {
    $md5 = md5_file($file);
  }
  else {
    $message = t('File: “@file” is not readable by the web server.', array('@file' => $file));
    $tainted = TRUE;
    $md5 = '';
  }
  $updated = filemtime($file);
  db_insert('file_integrity_md5')
  ->fields(array(
    'path' => $file,
    'md5hash' =>  $md5,
    'updated' =>  $updated,
  ))
  ->execute();
  if (is_writable($file)) {
    $message = t('File: “@file” is writeable by the web server.', array('@file' => $file));
    $tainted = TRUE;
  }
  if ($tainted) {
    $context['results']['error'][] = $message;
  }
  else {
    $context['results'][] = 'Fingerprinted ' . check_plain($file);
  }
}

/**
 * Callback.
 */
function file_integrity_create_md5_finished($success, $results, $operations) {
  if ($success) {
    $datetime = format_date(REQUEST_TIME, 'custom', 'H:i:s');
    drupal_set_message(t('!datetime: !count files fingerprinted.', array('!datetime' => $datetime, '!count' => count($results))), 'status');
    if (isset($results['error'])) {
      drupal_set_message(t('There were !count irregularities:', array(
         '!count' => count($results['error']),
      )), 'warning');
      foreach ($results['error'] as $error) {
        drupal_set_message($error, 'warning');
      }
      drupal_set_message(t('Please fix those and create a new fingerprint.'), 'warning');
    }
    else {
      drupal_set_message(t('There were no errors.'));
    }
    variable_set('file_integrity_last', REQUEST_TIME);
  } else {
    // A fatal error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('A fatal error occurred while processing @operation with arguments : @args', array('@operation' => $error_operation[0], '@args' => print_r($error_operation[0], TRUE)));
    variable_set('file_integrity_last', 0);
    drupal_set_message(t($message, 'error'));
  }
}

/**
 * Compare MD5 fingerprint for a single file.
 */
function file_integrity_compare_md5($file, &$context) {
  // Optional message displayed under the progressbar.
  $context['message'] = t('Checking fingerprint of file "@file"', array('@file' => $file));
  list($tainted, $message) = _file_integrity_compare_md5($file, TRUE);
  if ($tainted) {
    $context['results']['error'][] = $message;
  }
  else {
    $context['results'][] = 'Is equal: ' . check_plain($file);
  }
}

/**
 * Callback.
 */
function file_integrity_compare_md5_finished($success, $results, $operations) {
  // Check for missing files.
  $result = db_query("SELECT path FROM {file_integrity_md5} WHERE bfound = 0");
  foreach ($result as $record) {
    $message = t('File: “@file” is missing.', array('@file' => $record->path));
    $results['error'][] = $message;
    $fid = db_insert('file_integrity_log')
    ->fields(array(
      'msg' => $message,
      'severity' =>  WATCHDOG_WARNING,
      'timestamp' => REQUEST_TIME,
    ))
    ->execute();
  }
  $datetime = format_date(REQUEST_TIME, 'custom', 'H:i:s');
  $startedfp = variable_get('file_integrity_startedfp', 0);
  drupal_set_message(t('!datetime: Finished comparing (!duration seconds).', array('!datetime' => $datetime, '!duration' => REQUEST_TIME - $startedfp)), 'status');
  $computedcs = _fileintegrity_compute_checksum();
  db_insert('file_integrity_log')
  ->fields(array(
    'msg' => t('Manual scan ended. Checksum: !computedcs.', array('!computedcs' => $computedcs)),
    'severity' =>  WATCHDOG_INFO,
    'timestamp' => REQUEST_TIME,
  ))
  ->execute();
  if ($success) {
    if (isset($results['error'])) {
      drupal_set_message(t('There were !count non-fatal error(s):', array(
         '!count' => count($results['error']),
      )), 'warning');
      foreach ($results['error'] as $error) {
        drupal_set_message($error, 'warning');
      }
    }
    else {
      drupal_set_message(t('!datetime: !count files checked. No anomalies detected.', array('!datetime' => $datetime, '!count' => count($results))), 'status');
    }
  } else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    drupal_set_message(t('!datetime: An error occurred while processing @operation with arguments: @args', array(
      '!datetime' => $datetime,
      '@operation' => $error_operation[0],
      '@args' => print_r($error_operation[0], TRUE),
    )), 'error');
  }
}

