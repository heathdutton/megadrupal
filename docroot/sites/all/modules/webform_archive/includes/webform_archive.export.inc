<?php

/**
 * @file
 * Provides several different handlers for exporting archive webform results.
 */

/**
 * Sets the header for the reports, batch for report generation.
 *
 * @param object $node
 *   Loaded webform node object.
 * @param array $options
 *   Options for report generation, array('start_time', 'end_time').
 */
function webform_archive_export_download($node, $options = NULL) {
  // Load webform component file as we need its functions.
  module_load_include('inc', 'webform', 'includes/webform.components');
  $file_path = webform_archive_get_report_file_path($options, $node->nid);
  // If file already exists, download the file and do not process further.
  if (file_exists($file_path)) {
    $file_url = file_create_url($file_path);
    drupal_goto($file_url);
  }
  // Basic Submission information.
  $submission_information = array(
    'nid' => t('Title'),
    'sid' => t('SID'),
    'submitted' => t('Submission Time'),
    'remote_addr' => t('IP Address'),
  );
  $options['file_name'] = $file_path;
  // Other components for the webform.
  $options['components'] = array_keys(webform_component_list($node, 'csv', TRUE));
  $component_header = array();
  // Compile header information for components.
  foreach ($options['components'] as $cid) {
    if (isset($node->webform['components'][$cid])) {
      $component = $node->webform['components'][$cid];
      $component_header[$cid] = $component['name'];
    }
  }
  ksort($component_header);
  // CSV report file header.
  $header = $submission_information + $component_header;
  $options['components'] = $header;
  // Batch process function for downloading the Reports.
  $operations = array(
    array('webform_archive_download_report_start', array($node, $options)),
  );
  $batch = array(
    'title' => t('Download Archive Report'),
    'operations' => $operations,
    'file' => drupal_get_path('module', 'webform_archive') . '/includes/webform_archive.batch.inc',
    'finished' => 'webform_archive_download_batch_finished',
    'init_message' => t('Report creation started'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Report processing has encountered an error.'),
  );
  batch_set($batch);
}

/**
 * Gives File path for the report according to time range.
 *
 * @param array $options
 *   Start Time and End time to calculate the file name.
 * @param int $nid
 *   Webform nid.
 *
 * @return string
 *   File name.
 */
function webform_archive_get_report_file_path($options, $nid) {
  $start_time = $options['start_time'];
  $frequency = $options['frequency'];
  $time = date('Y-M', $start_time);

  // File name is created in a manner.

  // Format = webform-archive- [nid] -[frequency] - [Year] - [Start Month].csv.

  $file_name = 'webform-archive-' . $nid . '-' . $frequency . '-' . $time;
  $file_name .= '.csv';
  $file_path = WEBFORM_ARCHIVE_DIRECTORY . '/' . $file_name;
  $file_path = file_build_uri($file_path);
  return $file_path;
}
