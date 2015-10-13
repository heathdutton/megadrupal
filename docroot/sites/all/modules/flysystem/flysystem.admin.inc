<?php

/**
 * @file
 * Configuration page callbacks for Flysystem.
 */

/**
 * Page callback for the Flysystem configuration form.
 */
function flysystem_config_form(array $form, array &$form_state) {
  if (!flysystem_dependencies_check()) {
    drupal_set_message(t('The Flysystem dependencies are not installed correctly.'));
    return $form;
  }

  $schemes = array_keys(variable_get('flysystem', array()));

  $form['sync_from'] = array(
    '#type' => 'select',
    '#options' => array_combine($schemes, $schemes),
    '#title' => t('Sync from'),
    '#required' => TRUE,
  );

  $form['sync_to'] = array(
    '#type' => 'select',
    '#options' => array_combine($schemes, $schemes),
    '#title' => t('Sync to'),
    '#required' => TRUE,
  );

  $form['force'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force'),
    '#description' => t('Normally, existing files will be ignored. Selecting this option will overwrite any existing files.'),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Sync'),
  );

  return $form;
}

/**
 * Validation callback for flysystem_config_form().
 */
function flysystem_config_form_validate(array &$form, array &$form_state) {
  if ($form_state['values']['sync_from'] === $form_state['values']['sync_to']) {
    form_error($form['sync_from'], t('"Sync from" and "Sync to" cannot be the same scheme.'));
    form_error($form['sync_to']);
  }
}

/**
 * Submit callback for flysystem_config_form().
 */
function flysystem_config_form_submit(array &$form, array &$form_state) {
  $scheme_from = $form_state['values']['sync_from'];
  $scheme_to = $form_state['values']['sync_to'];

  $from_files = _flysystem_config_form_get_file_list($scheme_from);

  $to_files = array();
  if (!$form_state['values']['force']) {
    $to_files = _flysystem_config_form_get_file_list($scheme_to);
  }

  $batch = array(
    'operations' => array(),
    'finished' => '_flysystem_config_form_finish_batch',
    'title' => t('Synchronizing file systems'),
    'init_message' => t('Starting file system synchronization.'),
    'progress_message' => t('Completed @current step of @total.'),
    'error_message' => t('File system synchronization has encountered an error.'),
    'file' => drupal_get_path('module', 'flysystem') . '/flysystem.admin.inc',
  );

  // @todo We shouldn't do all files in one go, but rather add files and
  // directories and recurse in a batch callback.
  foreach (array_diff($from_files, $to_files) as $filepath) {
    $batch['operations'][] = array('_flysystem_config_form_copy_file', array($scheme_from, $scheme_to, $filepath));
  }

  if (empty($batch['operations'])) {
    drupal_set_message(t('There are no files to copy.'), 'warning');
    return;
  }

  batch_set($batch);
}

/**
 * Copies a single file.
 *
 * @param string $scheme_from
 *   The scheme to sync from.
 * @param string $scheme_to
 *   The scheme to sync to.
 * @param string $filepath
 *   The file to sync.
 * @param array &$context
 *   The batch context.
 */
function _flysystem_config_form_copy_file($scheme_from, $scheme_to, $filepath, array &$context) {
  $context['message'] = t('Copying: %file', array('%file' => $filepath));
  $context['finished'] = 1;

  $factory = flysystem_factory();

  // Copying files could take a very long time. Using streams will keep memory
  // usage down, but we could still timeout.
  drupal_set_time_limit(0);

  try {
    $read_handle = $factory->getFilesystem($scheme_from)->readStream($filepath);

    if (!is_resource($read_handle)) {
      $args = array('%scheme' => $scheme_from, '%file' => $filepath);
      $context['results']['errors'][] = array('The file %scheme://%file could not be opened.', $args);
      return;
    }

    $success = $factory->getFilesystem($scheme_to)->putStream($filepath, $read_handle);

    if (!$success) {
      $args = array('%scheme' => $scheme_to, '%file' => $filepath);
      $context['results']['errors'][] = array('The file %scheme://%file could not be saved.', $args);
    }
  }

  // Catch all exceptions so we don't break batching. The types of exceptions
  // that adapters can throw varies greatly.
  catch (Exception $e) {
    $context['results']['errors'][] = array('An eror occured while copying %file.', array('%file' => $filepath));
    $context['results']['errors'][] = $e->getMessage();

    watchdog_exception('flysystem', $e);
  }

  if (isset($read_handle) && is_resource($read_handle)) {
    fclose($read_handle);
  }
}

/**
 * Finishes a batch.
 */
function _flysystem_config_form_finish_batch($success, array $results, array $operations) {
  if (!$success) {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $operation = reset($operations);
    $args = array('%file' => $operation[2]);
    drupal_set_message(t('An error occurred while syncing: %file', $args), 'error');
    return;
  }

  if (empty($results['errors'])) {
    drupal_set_message(t('File synchronization finished successfully.'));
    return;
  }

  foreach ($results['errors'] as $error) {
    if (is_array($error)) {
      drupal_set_message(t($error[0], $error[1]), 'error', TRUE);
      watchdog('flysystem', $error[0], $error[1], WATCHDOG_ERROR);
    }
    else {
      drupal_set_message(check_plain($error), 'error', TRUE);
    }
  }

  drupal_set_message(t('File synchronization experienced errors.'), 'warning');
}

/**
 * Returns the file list for a scheme.
 *
 * @param string $scheme
 *   The scheme.
 *
 * @return string[]
 *   A list of files.
 */
function _flysystem_config_form_get_file_list($scheme) {
  $filesystem = flysystem_factory()->getFilesystem($scheme);

  $files = array_filter($filesystem->listContents('', TRUE), function (array $meta) {
    return $meta['type'] === 'file';
  });

  return array_map(function (array $meta) {
    return $meta['path'];
  }, $files);
}
