<?php
/**
 * @file
 * Contains the functionality to run the translations request batch.
 */

/**
 * Initiate and run a request batch based on a the given translator.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the batch for.
 * @param string $destination
 *   Path to where the user needs to be redirected once the batch has finished.
 */
function tmgmt_xconnect_batch_request_run(TMGMTTranslator $translator, $destination) {
  // Get all the open jobs for the given translator.
  $jobs = tmgmt_xconnect_load_job_all_by_translator(
    $translator,
    TMGMT_JOB_STATE_UNPROCESSED
  );

  if (!$jobs) {
    drupal_set_message(
      t('There are no jobs to send to the service.'),
      'warning'
    );
    drupal_goto($destination);
  }

  // Create the operations.
  $operations = array();
  foreach ($jobs as $job) {
    $operations[] = array('tmgmt_xconnect_batch_request_job', array($job));
  }

  $batch = array(
    'operations' => $operations,
    'finished' => 'tmgmt_xconnect_batch_request_finished',
    'title' => t('Sending translation jobs to the service'),
    'init_message' => t('Starting sending jobs.'),
    'progress_message' => t('Sending @current out of @total.'),
    'error_message' => t('Sending translation jobs has encountered an error.'),
    'file' => drupal_get_path('module', 'tmgmt_xconnect') . '/includes/batch_request.inc',
  );

  batch_set($batch);
  batch_process($destination);
}

/**
 * Batch process a single translation Job.
 *
 * @param TMGMTJob $job
 *   The Job to send the request for.
 * @param array $context
 *   The Batch context.
 */
function tmgmt_xconnect_batch_request_job(TMGMTJob $job, &$context) {
  // Init the message array if not initiated before.
  if (empty($context['results'])) {
    $context['results'] = array(
      'success' => array(),
      'error' => array(),
    );
  }

  $args = array(
    '@identifier' => $job->identifier(),
    '!url' => url('admin/tmgmt/jobs/' . $job->tjid),
  );

  // Send out the translation request.
  $job->requestTranslation();

  // Log result.
  if ($job->isState(TMGMT_JOB_STATE_ACTIVE)) {
    $context['results']['success'][] = t(
      'Send <a href="!url">Job @identifier</a>.',
      $args
    );
  }
  else {
    $context['results']['error'][] = t(
      'Could not send translation for <a href="!url">Job @identifier</a>',
      $args
    );
  }

  $context['finished'] = 1;
}

/**
 * Receive Batch 'finished' callback.
 *
 * @param bool $success
 *   Was the batch a success.
 * @param array $results
 *   Array containing lists of successfull and errored processed translations.
 * @param array $operations
 *   Array containing all operations that (should) have run.
 */
function tmgmt_xconnect_batch_request_finished($success, $results, $operations) {
  // An error occurred.
  if (!$success) {
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t(
      'An error occurred while processing %error_operation with arguments: @arguments',
      array(
        '%error_operation' => $error_operation[0],
        '@arguments' => print_r($error_operation[1], TRUE),
      )
    );
    drupal_set_message($message, 'error');
    return;
  }

  // Add the info about the successfully processed Jobs.
  if (count($results['success'])) {
    $message = t(
      '@count translations successfully send:',
      array('@count' => count($results['success']))
    );
    $message .= theme(
      'item_list',
      array('items' => $results['success'])
    );
    drupal_set_message($message, 'status');
  }

  // Add the info about the failed Jobs.
  if (count($results['error'])) {
    $message = t(
      '@count translations could not be send:',
      array('@count' => count($results['error']))
    );
    $message .= theme(
      'item_list',
      array('items' => $results['error'])
    );
    drupal_set_message($message, 'error');
  }
}
