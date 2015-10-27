<?php
/**
 * @file
 * Contains the functionality to run the translations receive batch.
 */

/**
 * Initiate and run a receive batch based on a the given Translator.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the batch for.
 * @param string $destination
 *   Path to where the user needs to be redirected once the batch has finished.
 */
function tmgmt_xconnect_batch_receive_run(TMGMTTranslator $translator, $destination) {
  // Get the files ready for translation.
  try {
    $files_ready = tmgmt_xconnect_import_get_remote_files($translator);
  }
  catch (Exception $e) {
    drupal_set_message(
      t(
        'Could not scan for translated jobs: %message',
        array('%message' => $e->getMessage())
      ),
      'warning'
    );
    drupal_goto($destination);
  }

  // Check if there are files to be imported.
  if (!$files_ready) {
    drupal_set_message(
      t('There are no processed translations to import.'),
      'warning'
    );
    drupal_goto($destination);
  }

  // Create the batch operations, one batch per file to avoid time-out issues.
  $operations = array();
  foreach ($files_ready as $file_ready) {
    $operations[] = array(
      'tmgmt_xconnect_batch_receive_file',
      array($translator, $file_ready),
    );
  }

  // Setup & process the batch.
  $batch = array(
    'operations' => $operations,
    'finished' => 'tmgmt_xconnect_batch_receive_finished',
    'title' => t('Processing translated jobs'),
    'init_message' => t('Starting processing translated files.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Translated files processing has encountered an error.'),
    'file' => drupal_get_path('module', 'tmgmt_xconnect') . '/includes/batch_receive.inc',
  );

  batch_set($batch);
  batch_process($destination);
}

/**
 * Batch process a single translated file.
 *
 * @param TMGMTTranslator $translator
 *   The translator service to perform the action for.
 * @param string $file_ready
 *   The file that is ready to be processed.
 * @param array $context
 *   The Batch context.
 */
function tmgmt_xconnect_batch_receive_file(TMGMTTranslator $translator, $file_ready, &$context) {
  // Init the message array if not initiated before.
  if (empty($context['results'])) {
    $context['results'] = array(
      'success' => array(),
      'error' => array(),
    );
  }

  // Process the file.
  $result = tmgmt_xconnect_import_remote_file($translator, $file_ready);

  // Add the result of the process to the results array.
  $type = ($result instanceof TmgmtXConnectImportResultSuccess)
    ? 'success'
    : 'error';
  // Exclude this code from the code style inspections as we are translating a
  // variable.
  // @codingStandardsIgnoreStart
  $context['results'][$type][] = t(
    $result->getMessage(),
    $result->getMessageArguments()
  );
  // @codingStandardsIgnoreEnd

  // One file per operation.
  $context['finished'] = 1;
}

/**
 * Receive Batch 'finished' callback.
 *
 * @param bool $success
 *   Was the batch a success.
 * @param array $results
 *   Array containing lists of successfully and failed processed translations.
 * @param array $operations
 *   Array containing all operations that (should) have run.
 */
function tmgmt_xconnect_batch_receive_finished($success, $results, $operations) {
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

  // Add the info about the successfully imported files.
  if (count($results['success'])) {
    $message = t(
      '@count translations successfully processed:',
      array('@count' => count($results['success']))
    );
    $message .= theme(
      'item_list',
      array('items' => $results['success'])
    );
    drupal_set_message($message, 'status');
  }

  // Add the info about the failed file imports.
  if (count($results['error'])) {
    $message = t(
      '@count translations could not be processed:',
      array('@count' => count($results['error']))
    );
    $message .= theme(
      'item_list',
      array('items' => $results['error'])
    );
    drupal_set_message($message, 'error');
  }
}
