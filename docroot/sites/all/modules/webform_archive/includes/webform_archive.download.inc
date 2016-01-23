<?php
/**
 * @file
 * Archive Report Download.
 */

module_load_include('inc', 'webform', 'includes/webform.components');

/**
 * Page Callback function.
 */
function webform_archive_download_reports($webform_node) {
  $nid = $webform_node->nid;
  $is_archvie = webform_archive_is_webform_archived($nid);
  if ($is_archvie) {
    $download_report_form = drupal_get_form('webform_archive_download_reports_form', $webform_node);
    return $download_report_form;
  }
  else {
    $message = t('This webform is not archived, So no reports are available for download.');
    return $message;
  }
}

/**
 * Check Wheater webform is archived or not.
 *
 * @param int $nid
 *   Webfrom nid.
 *
 * @return bool
 *   Returns True if webform is archived else false.
 */
function webform_archive_is_webform_archived($nid) {
  $is_archvie = db_select('archive_webform', 'aw')
  ->fields('aw')
  ->condition('aw.nid', $nid)
  ->execute();
  $is_archvie = $is_archvie->rowCount();
  if ($is_archvie > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Form builder for download report form.
 */
function webform_archive_download_reports_form($form, &$form_state, $webform_node) {
  $nid = $webform_node->nid;
  // Get First and last webfrom submission time, so that we can give a user time
  // range b/w this time for reports download.
  $first_submission_timestamp = webform_archive_get_first_submission_timestamp($nid);
  $last_submission_timestamp = webform_archive_get_last_submission_timestamp($nid);
  $last_submission_timestamp = $last_submission_timestamp['last_submitted'];
  $first_submission_timestamp = $first_submission_timestamp['first_submitted'];
  $current_year = date('Y');
  $last_submission_year = date('Y', $last_submission_timestamp);
  $first_submission_year = date('Y', $first_submission_timestamp);
  $start_range = $first_submission_year - $current_year;
  $end_range = $last_submission_year - $current_year;
  $year_range = $start_range . ':' . $end_range;

  // Various Report download options.
  $options = array(
    'yearly' => t('Yearly'),
    'monthly' => t('Monthly'),
    'half-yearly' => t('Half Yearly'),
    'quarterly' => t('Quarterly'),
  );

  $form_state['storage']['webform_node'] = $webform_node;

  $form['webform_archive_report_options'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#title' => t('Frequency'),
    '#default_value' => 'yearly',
    '#required' => TRUE,
  );

  $form['webform_archive_report_start_time'] = array(
    '#type' => module_exists('date') ? 'date_select' : 'date',
    '#date_format' => 'Y-m',
    '#required' => TRUE,
    '#title' => t('Select Duration'),
    '#date_year_range' => $year_range,
    '#date_label_position' => 'within',
  );

  $form['webform_archive_show_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show submission count only'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Downlaod Report'),
    '#name' => 'report_download_submit',
  );

  return $form;
}

/**
 * Get first webform submission time stamp.
 *
 * @param int $nid
 *   Webform Nid.
 *
 * @return array
 *   First submission fetched time.
 */
function webform_archive_get_first_submission_timestamp($nid) {
  $submission_timestamps = db_select('archive_webform_submissions', 'aws')
  ->condition('aws.nid', $nid);
  $submission_timestamps->addExpression('MIN(submitted)', 'first_submitted');
  $submission_timestamps = $submission_timestamps->execute();
  $submission_timestamps = $submission_timestamps->fetchAssoc();
  return $submission_timestamps;
}

/**
 * Get Last webform submission time stamp.
 *
 * @param int $nid
 *   Webform Nid.
 *
 * @return array
 *   Last submission fetched time.
 */
function webform_archive_get_last_submission_timestamp($nid) {
  $submission_timestamps = db_select('archive_webform_submissions', 'aws')
  ->condition('aws.nid', $nid);
  $submission_timestamps->addExpression('MAX(submitted)', 'last_submitted');
  $submission_timestamps = $submission_timestamps->execute();
  $submission_timestamps = $submission_timestamps->fetchAssoc();
  return $submission_timestamps;
}

/**
 * Submit Callback for download form, Initiates batch process to save a report.
 */
function webform_archive_download_reports_form_submit($form, &$form_state) {
  $options = array();
  // Load the archive webform export file and pass data into it, to initiate
  // the batch process.
  module_load_include('inc', 'webform_archive', 'includes/webform_archive.export');
  $webform_node = $form_state['storage']['webform_node'];
  $show_count = $form_state['values']['webform_archive_show_count'];
  $start_time = $form_state['values']['webform_archive_report_start_time'];
  $start_time = strtotime($start_time);
  $frequency = $form_state['values']['webform_archive_report_options'];
  switch ($frequency) {
    case 'yearly':
      $end_time = strtotime('+1 year', $start_time);
      break;

    case 'monthly':
      $end_time = strtotime('+1 month', $start_time);
      break;

    case 'quarterly':
      $end_time = strtotime('+3 month', $start_time);
      break;

    case 'half-yearly':
      $end_time = strtotime('+6 month', $start_time);
      break;

    default:
      break;
  }
  $options['start_time'] = $start_time;
  $options['end_time'] = $end_time;
  $options['frequency'] = $frequency;
  if ($show_count == 1) {
    module_load_include('inc', 'webform_archive', 'includes/webform_archive.batch');
    $count = webform_archive_archive_submissions_count($webform_node->nid, $start_time, $end_time);
    drupal_set_message(t('Total Download Count : @count'), array('@count' => $count));
  }
  else {
    // Calls the batch process.
    webform_archive_export_download($webform_node, $options);
  }
}

/**
 * Gives the months difference b/w two timestamps.
 *
 * @param int $start_time
 *   Start time stamp.
 * @param int $end_time
 *   End time stamp.
 *
 * @return int
 *   Difference of two timestamps.
 */
function webform_archive_date_difference($start_time, $end_time) {
  $diff = ((date('Y', $end_time) - date('Y', $start_time)) * 12) + (date('m', $end_time) - date('m', $start_time));
  return $diff;
}
