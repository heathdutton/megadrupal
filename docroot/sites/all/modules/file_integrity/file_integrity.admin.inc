<?php 
/**
 * @file
 * Checks all files below site root (except uploaded files) for
 * changes.
 */


/**
 * Form constructor for the form that does a quick check
 *
 * @see file_integrity_checksum_submit()
 * @ingroup forms
 */
function file_integrity_checksum_form($form) {
  $form['file_integrity_quick'] = array(
    '#type' => 'fieldset',
    '#title' => t('Instant file checksum check'),
    '#description' => t('This let you check the integrity of your files by recomputing the file content checksum to see if it still matches the stored value. However, this computation may time out on a large site.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $storedcs = variable_get('file_integrity_checksum', 0);
  $form['file_integrity_quick']['quick'] = array(
    '#type' => 'submit',
    '#value' => t('Check checksum'),
    '#submit' => array('file_integrity_checksum_submit'),
    '#disabled' => !$storedcs,
  );
  if (!$storedcs) {
    $form['file_integrity_quick']['markup'] = array(
      '#markup' => t('(The checksum does not exist. Fingerprint the site to create it.)'),
    );
  }
  return $form;
}

/**
 * Form submission handler for file_integrity_checksum_form().
 * 
 */
function file_integrity_checksum_submit() {
  global $base_url;
  $checksum = _fileintegrity_compute_checksum();
  $storedcs = variable_get('file_integrity_checksum', 0);
  if ($checksum == $storedcs) {
    drupal_set_message(t('Files passed checksum test.'));
  }
  else {
    $path = base_path() . 'admin/config/system/file_integrity';
    drupal_set_message(t('Checksum test failed (stored: !storedcs vs. computed: !checksum). Visit the <a the href="@path">file integrity configuration page</a> and compare fingerprints to see details.', array('!storedcs' => $storedcs, '!checksum' => $checksum, '@path' => $path)), 'error');
  }
}


/**
 * Form constructor for the form that clears out the log.
 *
 * @see file_integrity_clear_log_submit()
 * @ingroup forms
 */
function file_integrity_clear_log_form($form) {
  $form['file_integrity_clear'] = array(
    '#type' => 'fieldset',
    '#title' => t('Clear log messages'),
    '#description' => t('This will permanently remove the log messages from the database.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['file_integrity_clear']['clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear log messages'),
    '#submit' => array('file_integrity_clear_log_submit'),
  );

  return $form;
}

/**
 * Form submission handler for file_integrity_clear_log_form().
 */
function file_integrity_clear_log_submit() {
  variable_set('file_integrity_last_fid', 0);
  db_truncate('file_integrity_log')->execute();
  drupal_set_message(t('File integrity log cleared.'));
}


/**
 * Page callback: Displays a listing of database log messages.
 *
 * This borrows a lot from dblog.
 *
 * @see file_integrity_clear_log_form()
 * @see file_integrity_event()
 * @see file_integrity_filter_form()
 * @see file_integrity_menu()
 *
 * @ingroup logging_severity_levels
 */
function file_integrity_overview() {
  $classes = array(
    WATCHDOG_DEBUG     => 'dblog-debug',
    WATCHDOG_INFO      => 'dblog-info',
    WATCHDOG_NOTICE    => 'dblog-notice',
    WATCHDOG_WARNING   => 'dblog-warning',
    WATCHDOG_ERROR     => 'dblog-error',
    WATCHDOG_CRITICAL  => 'dblog-critical',
    WATCHDOG_ALERT     => 'dblog-alert',
    WATCHDOG_EMERGENCY => 'dblog-emerg',
  );

  $rows = array();
  $build['file_integrity_checksum_form'] = drupal_get_form('file_integrity_checksum_form');
  $build['file_integrity_clear_log_form'] = drupal_get_form('file_integrity_clear_log_form');
  $header = array(
    '', // Icon column.
    array('data' => t('Lid'), 'field' => 'f.lid', 'sort' => 'desc'),
    array('data' => t('Timestamp'), 'field' => 'f.timestamp'),
    t('Message'),
  );

  $query = db_select('file_integrity_log', 'f')->extend('PagerDefault')->extend('TableSort');
  $query = $query
    ->fields('f', array('lid', 'msg', 'severity', 'timestamp'))
    ->limit(50)
    ->orderByHeader($header);
  $result = $query->execute();

  foreach ($result as $dblog) {
    $rows[] = array('data' =>
      array(
      // Cells
        array('class' => 'icon'),
	$dblog->lid,
	format_date($dblog->timestamp,'short'),
	$dblog->msg,
      ),
      // Attributes for tr
      'class' => array($classes[$dblog->severity]),
    );
  }

  $build['file_integrity_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'admin-dblog'),
    '#empty' => t('No log messages available.'),
  );
  $build['file_integrity_pager'] = array('#theme' => 'pager');

  return $build;
}
