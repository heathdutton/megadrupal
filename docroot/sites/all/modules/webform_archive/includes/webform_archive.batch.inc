<?php
/**
 * @file
 * Contains batch processes for webform_archive module.
 */

module_load_include('inc', 'webform', 'includes/webform.submissions');

/**
 * Batch process for archiving webforms.
 */
function webform_archive_batch_process_start($webforms, $archive_date_timestamp, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_submission'] = 0;
    $count = webform_archive_submissions_count($webforms, $archive_date_timestamp);
    $context['sandbox']['max'] = ($count != 0) ? $count : 1;
  }
  // 10 submissions at a time without a timeout.
  $limit = variable_get('webform_archive_batch_limit', WEBFORM_ARCHIVE_BATCH_LIMIT);
  // With each pass through the callback, retrieve the next group of results.
  $submissions = webform_archive_get_submissions($webforms, $archive_date_timestamp, $context['sandbox']['current_submission'], $limit);
  if (!empty($submissions)) {
    // Here we actually perform our processing on the current submission.
    foreach ($submissions as $sid => $submission) {
      if ($submission != NULL) {
        $submitted_data = $submission->data;
        unset($submission->data);
        $nid = $submission->nid;
        drupal_write_record('archive_webform_submissions', $submission);
        webform_archive_archive_submitted_data($submitted_data, $nid, $sid);
        // Update our progress information.
        $context['sandbox']['progress']++;
        $context['results'][] = $sid;
        $context['results']['webforms'][$nid] = $nid;
        $context['results']['archived'] = $archive_date_timestamp;
        $context['sandbox']['current_submission'] = $sid;
        $context['message'] = t('Now processing %sid', array('%sid' => $sid));
      }
    }
    $context['results']['last_sid'] = $context['sandbox']['current_submission'];
  }
  else {
    $context['sandbox']['progress'] = $context['sandbox']['max'] + 1;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Batch 'finished' callback.
 */
function webform_archive_batch_finished($success, $results, $operations) {
  if ($success) {
    $webforms = isset($results['webforms']) ? $results['webforms'] : array();
    $archived = isset($results['archived']) ? $results['archived'] : 0;
    $last_sid = isset($results['last_sid']) ? $results['last_sid'] : 0;
    unset($results['webforms']);
    unset($results['archived']);
    unset($results['last_sid']);
    if (isset($webforms) && !empty($webforms)) {
      foreach ($webforms as $nid) {
        // Inserting the last archived time for webform.
        $archived = ($archived != 0) ? $archived : REQUEST_TIME;
        webform_archive_insert_archive_webform_data($nid, $archived);
        // Deleting submissions form the webform tables.
        db_delete('webform_submissions')->condition('nid', $nid)->condition('sid', $last_sid, '<=')->execute();
        db_delete('webform_submitted_data')->condition('nid', $nid)->condition('sid', $last_sid, '<=')->execute();
      }
    }
    $count = count($results);
    $message = t('@count Webforms submissions are Archived successfully', array('@count' => $count));
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with
      arguments: @arguments', array(
        '%error_operation' => $error_operation[0],
        '@arguments' => print_r($error_operation[1], TRUE))
      );
    if (module_exists('dblog')) {
      watchdog('webform_archive', $message, array(), WATCHDOG_CRITICAL);
      drupal_set_message(t('Some error occured while archiving webforms'), 'error');
    }
    else {
      drupal_set_message($message, 'error');
    }
  }
}

/**
 * Returns the submissions which needs to archive.
 *
 * @param array $webforms
 *   Webforms whose submissions are to arhcived.
 * @param int $archive_date_timestamp
 *   Unix timestamp for the date.
 * @param int $current_submission_sid
 *   SID, all submissions whoses sid is greater then this current submission.
 * @param int $limit
 *   No. of submissions to return.
 *
 * @return array
 *   Webform Submissions and their data.
 */
function webform_archive_get_submissions($webforms, $archive_date_timestamp, $current_submission_sid, $limit) {
  $submissions = array();
  $result = db_select('webform_submissions', 'ws')
  ->fields('ws', array('sid', 'nid'))
  ->orderBy('ws.sid', 'ASC')
  ->condition('nid', $webforms, 'IN')
  ->condition('sid', $current_submission_sid, '>')
  ->range(0, $limit);
  if ($archive_date_timestamp !== 0) {
    $result = $result->condition('submitted', $archive_date_timestamp, '<');
  }
  $result = $result->execute();
  if (isset($result) && !empty($result)) {
    while ($submission = $result->fetchAssoc()) {
      $sid = $submission['sid'];
      $nid = $submission['nid'];
      $submission_data = webform_get_submissions(array('nid' => $nid, 'sid' => $sid));
      $submissions[$submission['sid']] = isset($submission_data[$sid]) ? $submission_data[$sid] : NULL;
    }
  }
  return $submissions;
}

/**
 * Returns the total count of webform submissions.
 *
 * @param array $webforms
 *   Array of webform nid(s).
 * @param int $archive_date_timestamp
 *   Unix timestamp for archiving submissions.
 *
 * @return int
 *   Total Count of webform submissions.
 */
function webform_archive_submissions_count($webforms, $archive_date_timestamp) {
  $count = db_select('webform_submissions', 'ws')
  ->fields('ws', array('sid'))
  ->condition('ws.nid', $webforms, 'IN');
  if ($archive_date_timestamp !== 0) {
    $count = $count->condition('ws.submitted', $archive_date_timestamp, '<');
  }
  $count = $count->execute();
  $count = $count->rowCount();
  return $count;
}

/**
 * Archive the webform submitted data.
 *
 * @param array $data
 *   Webfrom submitted data.
 */
function webform_archive_archive_submitted_data($data, $nid, $sid) {
  foreach ($data as $cid => $values) {
    // This is done for supporting webform V - 4.x.
    if (isset($values['value'])) {
      $values = $values['value'];
    }
    foreach ($values as $delta => $value) {
      $data = array(
        'nid' => $nid,
        'sid' => $sid,
        'cid' => $cid,
        'no' => $delta,
        'data' => is_null($value) ? '' : $value,
      );
      drupal_write_record('archive_webform_submitted_data', $data);
    }
  }
}

/**
 * Batch Process for downloading archive webform submission reports.
 */
function webform_archive_download_report_start($node, $options, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_submission'] = 0;
    $count = webform_archive_archive_submissions_count($node->nid, $options['start_time'], $options['end_time']);
    $context['sandbox']['max'] = ($count == 0) ? $count + 1 : $count;
    $file_path = $options['file_name'];
    // Create the file.
    $handle = fopen($file_path, 'w+');
    drupal_chmod($file_path, 0666);
    // Add HTTP headers for CSV file download.
    $header = $options['components'];
    // Write the labels to the header row.
    fputcsv($handle, $header);
    fclose($handle);
    $context['sandbox']['file_name'] = $file_path;
  }
  $header = $options['components'];
  // 10 submissions at a time without a timeout.
  $limit = variable_get('webform_archive_batch_limit', WEBFORM_ARCHIVE_BATCH_LIMIT);
  // With each pass through the callback, retrieve the next group of results.
  $submissions = webform_archive_get_archive_submissions(array($node->nid), $options, $context['sandbox']['current_submission'], $limit);
  if (!empty($submissions)) {
    // Open the file for writing ('a' puts pointer at end of file).
    $handle = fopen($context['sandbox']['file_name'], 'a');
    foreach ($submissions as $sid => $submission) {
      $data = $submission['submission'];
      $data['nid'] = $node->title;
      $data['submitted'] = date('Y-m-d', $data['submitted']);
      fputcsv($handle, $data);
      // Update our progress information.
      $context['sandbox']['progress']++;
      $context['sandbox']['current_submission'] = $sid;
      $context['message'] = t('Now processing %sid', array('%sid' => $sid));
    }
    // Close the file.
    fclose($handle);
  }
  else {
    $context['sandbox']['progress'] = $context['sandbox']['max'] + 2;
  }
  $context['results']['file_path'] = $context['sandbox']['file_name'];
  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finished callback function for download report batch process.
 */
function webform_archive_download_batch_finished($success, $results, $operations) {
  if ($success) {
    $file_path = $results['file_path'];
    $file_path = file_create_url($file_path);
    $file_download_link = l(t('Click here'), $file_path);
    $message = t('To download the Report !link', array('!link' => $file_download_link));
    $message = filter_xss($message);
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE)));
    if (module_exists('dblog')) {
      watchdog('webform_archive', $message, array(), WATCHDOG_CRITICAL);
      drupal_set_message(t('Some error occured while archiving webforms'), 'error');
    }
    else {
      drupal_set_message($message, 'error');
    }
  }
}

/**
 * Get webform submission from the archive table.
 *
 * @param int $nid
 *   Webform nid.
 * @param array $options
 *   Submission filter options. arry('start_time', 'end_time').
 * @param int $current_submission
 *   SID, greater then this sid will be fetched.
 * @param int $limit
 *   Limit, no. of submissions to be fetched.
 *
 * @return array
 *   Webform submission details.
 */
function webform_archive_get_archive_submissions($nid, $options, $current_submission = 0, $limit = 10) {
  $archive_submission = array();
  $submissions = db_select('archive_webform_submissions', 'aws')
  ->fields('aws', array('nid', 'sid', 'submitted', 'remote_addr'))
  ->condition('aws.nid', $nid)
  ->orderBy('aws.sid', 'ASC')
  ->condition('aws.submitted', $options['start_time'], '>');
  if (isset($options['end_time']) && ($options['end_time'] !== NULL || $options['end_time'] !== 0)) {
    $submissions->condition('aws.submitted', $options['end_time'], '<');
  }
  $submissions->range(0, $limit);
  $submissions->condition('aws.sid', $current_submission, '>');
  $submissions = $submissions->execute();
  while ($submission = $submissions->fetchAssoc()) {
    $sid = $submission['sid'];
    $archive_submitted_data = db_select('archive_webform_submitted_data', 'awsd')
    ->fields('awsd', array('cid', 'data'))
    ->condition('awsd.sid', $sid)
    ->orderBy('awsd.cid', 'ASC')
    ->execute();
    $archive_submission[$sid]['submission'] = $submission;
    while ($data = $archive_submitted_data->fetchAssoc()) {
      $archive_submission[$sid]['submission'][$data['cid']] = $data['data'];
    }
  }
  return $archive_submission;
}

/**
 * Get Count of webform submission.
 *
 * @param int $nid
 *   Webform NID.
 * @param int $start_time
 *   Submission start time for webform.
 * @param int $end_time
 *   Submission end time for webform.
 *
 * @return int
 *   Submission count.
 */
function webform_archive_archive_submissions_count($nid, $start_time, $end_time = NULL) {
  $count = db_select('archive_webform_submissions', 'aws')
    ->fields('aws', array('sid'))
    ->condition('aws.nid', $nid)
    ->condition('aws.submitted', $start_time, '>');
  if ($end_time !== 0 && $end_time !== NULL) {
    $count = $count->condition('aws.submitted', $end_time, '<');
  }
  $count = $count->execute();
  $count = $count->rowCount();
  return $count;
}

/**
 * Updates the webform archived time.
 *
 * @param int $nid
 *   Webform nid.
 * @param int $archived
 *   Time for archive.
 */
function webform_archive_insert_archive_webform_data($nid, $archived) {
  module_load_include('inc', 'webform_archive', 'includes/webform_archive.download');
  if (webform_archive_is_webform_archived($nid)) {
    db_update('archive_webform')
    ->fields(array('archived' => $archived))
    ->condition('nid', $nid)
    ->execute();
  }
  else {
    db_insert('archive_webform')
      ->fields(array('nid' => $nid, 'archived' => $archived))
      ->execute();
  }
}
