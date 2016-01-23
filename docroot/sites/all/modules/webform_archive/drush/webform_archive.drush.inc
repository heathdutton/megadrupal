<?php

/**
 * @file
 * Drush integration for webform_archive.
 *
 * drush webform-archive
 */

/**
 * Implements hook_drush_help().
 */
function webform_archive_drush_help($section) {
  switch ($section) {
    case 'meta:webfrom_archive:title':
      return dt('Webfrom archive commands');

    case 'drush:ard-webforms-details':
      return dt('Show the list of archived webforms.');

    case 'drush:ar-active-webforms':
      $help = dt('Archive all the active webform submissions till today.');
      $help .= dt('If no webform is spacified all active webforms will be archived till today.');
      return $help;

    case 'drush:ar-closed-webforms':
      $help = dt('Archive all the closed webform submissions.');
      $help .= dt('If no webform is spacified all closed webforms will be archived.');
      return $help;
  }
}

/**
 * Implements hook_drush_command().
 */
function webform_archive_drush_command() {
  $items = array();
  $items['ard-webforms-details'] = array(
    'drupal dependencies' => array('webform_archive'),
    'callback' => 'webform_archive_archived_webform_details',
    'description' => 'Get the list of archived webforms',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('ardwfd'),
  );
  $items['ar-active-webform'] = array(
    'drupal dependencies' => array('webform_archive'),
    'callback' => 'webform_archive_archive_active_webform',
    'description' => 'Archive active webforms submission',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('aracwf'),
    'options' => array(
      'date' => 'Unix timestamp for archiving of webforms, till this date webfrom submissions will be archived.',
      'webform' => 'Webfrom nid in comma separated format',
    ),
    'examples' => array(
      'drush aracwf' => 'Archive all active webform submissions',
      'drush aracwf --date=3122342' => 'Archive all webforms till 3122342',
      'drush aracwf --date=3122342 --webform=1234,1235' => 'Archive 1234, 1235 webforms till date 3122342',
    ),
  );
  $items['ar-closed-webform'] = array(
    'drupal dependencies' => array('webform_archive'),
    'callback' => 'webform_archive_archive_closed_webform',
    'description' => 'Archive all closed webform submissions',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'aliases' => array('arcdwf'),
    'options' => array(
      'webform' => 'Webfrom nid in comma separated format',
    ),
    'examples' => array(
      'drush arcdwf' => 'Archive all active webform submissions',
      'drush arcdwf --webform=1234,1235' => 'Archive 1234, 1235 webforms',
    ),
  );
  return $items;
}

/**
 * List out all the archived webforms.
 */
function webform_archive_archived_webform_details() {
  $archived_webforms = webform_archive_get_archived_webforms();
  if (!empty($archived_webforms)) {
    // Set the table headers.
    $header = array(
      dt('NID'),
      dt('Archived till'),
    );
    $rows = array();
    foreach ($archived_webforms as $archived_webform) {
      $row = array();
      $row[] = $archived_webform->nid;
      $row[] = date('d-M-Y', $archived_webform->archived);
      $rows[] = $row;
    }
    array_unshift($rows, $header);
    drush_print_table($rows, TRUE);
  }
  else {
    drush_print('No Webforms Submissions are Archived.');
  }
}

/**
 * Archive all the active webform submissions.
 */
function webform_archive_archive_active_webform() {
  // Get the date argument.
  $archived_date_timepstamp = drush_get_option_list('date');

  // Get the webform arguments.
  $webform_nids = drush_get_option_list('webform');
  $text = '';
  // Confirm user action for different cases.
  if (empty($archived_date_timepstamp) && empty($webform_nids)) {
    $text = dt('All the webform submissions will be archived till date');
  }
  else {
    if (empty($archived_date_timepstamp)) {
      $webform_nids_str = implode(',', $webform_nids);
      $text = dt('Submissions of given webforms (@nids) will be Archived till date', array('@nids' => $webform_nids_str));
      $archived_date_timepstamp = REQUEST_TIME;
    }
    else {
      if (empty($webform_nids)) {
        $archived_date = date('d M Y, H : i', array_pop($archived_date_timepstamp));
        $text = dt('All webform submissions will be archived till given @date', array('@date' => $archived_date));
      }
    }
  }

  $archive = TRUE;
  if (!empty($text)) {
    if (!drush_confirm($text)) {
      $archive = FALSE;
    }
  }

  if ($archive) {
    $webform_nids = webform_archive_get_valid_webforms($webform_nids, 1);
    webform_archive_start_archive_after_confirm($webform_nids, $archived_date_timepstamp);
  }
}

/**
 * Starts batch process in the background.
 *
 * @param array $webform_nids
 *   Nid(s) of webform to archive.
 * @param int $archived_date_timepstamp
 *   Unix timestamp for archiving.
 */
function webform_archive_start_archiving($webform_nids, $archived_date_timepstamp) {
  if (!empty($webform_nids)) {
    $operations = array(
      array('webform_archive_batch_process_start',
        array($webform_nids, $archived_date_timepstamp)),
    );
    $batch = array(
      'title' => t('Archive Webform Submissions'),
      'operations' => $operations,
      'file' => drupal_get_path('module', 'webform_archive') . '/includes/webform_archive.batch.inc',
      'finished' => 'webform_archive_batch_finished',
      'init_message' => t('Archive Batch is starting.'),
      'progress_message' => t('Processed @current out of @total.'),
      'error_message' => t('Archive Batch has encountered an error.'),
    );
    batch_set($batch);
    // Start processing the batch operations.
    drush_backend_batch_process();
  }
  else {
    drush_print(dt('No Webforms are archived.'));
  }
}

/**
 * Archive all closed webform submissions.
 */
function webform_archive_archive_closed_webform() {
  // Get the webform arguments.
  $webform_nids = drush_get_option_list('webform');
  $webforms = array();
  if (empty($webform_nids)) {
    // Get all webforms which are closed.
    $webforms = webform_archive_get_all_webforms(0);
    $webforms = array_keys($webforms);
  }
  else {
    // Check that all webforms gived are closed.
    $webforms = webform_archive_get_valid_webforms($webform_nids, 0);
  }

  if (!empty($webforms)) {
    webform_archive_start_archive_after_confirm($webforms, REQUEST_TIME);
  }
  else {
    drush_print(dt('No Webforms are archived.'));
  }
}

/**
 * Check webform exists or not.
 *
 * @param array $webform_nids
 *   Array of webform nids.
 *
 * @return array
 *   Webform nids which does not exists.
 */
function webform_archive_get_valid_webforms($webform_nids, $status) {
  $nid_not_exists = array();
  $wrong_webform = array();
  foreach ($webform_nids as $nid) {
    $node = node_load($nid);
    if (is_object($node) && $node->type == 'webform') {
      if ($node->webform['status'] != $status) {
        $wrong_webform[] = $nid;
      }
      continue;
    }
    $nid_not_exists[] = $nid;
  }

  $webform_nids = array_diff($webform_nids, $nid_not_exists, $wrong_webform);

  if (!empty($nid_not_exists)) {
    $nid_not_exists = implode(',', $nid_not_exists);
    drush_print(dt('Webform nid(s): (@nid) either does not exists or are not webforms, so these will not be archived', array('@nid' => $nid_not_exists)));
  }

  if (!empty($wrong_webform)) {
    $wrong_webform = implode(',', $wrong_webform);
    $webform_status = ($status == 1) ? 'active' : 'closed';
    drush_print(dt('Status of Webform nid(s): (@nid) is not @status, so these will not be archived', array(
      '@nid' => $wrong_webform,
      '@status' => $webform_status,
      )));
  }
  return $webform_nids;
}

/**
 * Ask user for the final input before archiving, if yes then archive.
 *
 * @param array $webforms
 *   Webform nids.
 * @param int $timestamp
 *   Unix timestamp.
 */
function webform_archive_start_archive_after_confirm($webforms, $timestamp) {
  if (!empty($webforms)) {
    if (drush_confirm(dt('Webforms with nid(s): (@nid) will be archived, Do you still want to continue', array('@nid' => implode(',', $webforms))))) {
      webform_archive_start_archiving($webforms, $timestamp);
    }
  }
}
