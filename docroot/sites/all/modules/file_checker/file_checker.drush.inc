<?php
/**
 * @file
 * Drush commands for File Checker
 */

/**
 * Implements hook_drush_command().
 */
function file_checker_drush_command() {
  $items['file-checker-process'] = array(
    'description' => 'Verify all files in the files table',
    'aliases' => array('fcp'),
    'options' => array(
      'batch-size' => 'The number of files that should be processed in a single batch job.',
    ),
  );

  return $items;
}

/**
 * Callback for file-checker-process.
 */
function drush_file_checker_process() {
  // Get option for controlling batch size
  $batch_size = drush_get_option('batch-size');
  // process batch with drush
  file_checker_process_verification(FALSE, $batch_size);
  drush_backend_batch_process();
}