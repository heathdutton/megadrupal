<?php

/**
 * @file - Administrative code for Hotfolder.
 */

/**
 * Callback to admin settings page.
 */
function _hotfolder_admin() {
  $form['folders'] = array(
    '#title' => t('Folder configurations'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['folders']['hotfolder_internal_folders'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Enable internal folders'),
    '#description' => t('Use hotfolder internal folders during job processing. These folders keep the same file from ' .
      'creating multiple jobs. Disabling this setting will avoid moving the file to a new location during the job process. ' .
      'But this will require an additional database query to filter out files that have already been added to jobs.'),
    '#default_value' => variable_get('hotfolder_internal_folders', TRUE),
  );
  $form['folders']['hotfolder_submitted_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Submitted Files Folder'),
    '#description' => t('Configure folder name where submitted files are moved within the watched folder during processing.'),
    '#default_value' => variable_get('hotfolder_submitted_folder', 'hotfolder_submitted'),
  );
  // Files are moved to the done folder when job is completed.
  // Therefore no files should remain stuck in the submitted folder.
  // It should be empty on average.
  $form['folders']['hotfolder_done_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Processed Files Folder'),
    '#description' => t('Configure folder name where resulting files are moved within the watched folder after processing.'),
    '#default_value' => variable_get('hotfolder_done_folder', 'hotfolder_done'),
  );
  $hotfolder_streams = _hotfolder_get_schemas();
  $streams = array();
  foreach ($hotfolder_streams as $name => $path) {
    if ($name != 'public://' && $name != 'private://') {
      $path = _hotfolder_realpath($path);
      $stream = array(check_plain($name), check_plain($path));
      // Check that permissions and access are ok.
      $notes = file_exists($path) ? '<div class="messages status">' . t('This path is accessible.') . '</div>' : '<div class="messages error">' . t('This path is NOT accessible.') . '</div>';
      $stream[] = $notes;
      $streams[] = $stream;
    }
  }
  // Check file handling settings.
  $safe_mode = t('Safe mode is turned on.');
  if (!ini_get('safe_mode')) {
    $safe_mode = t('Safe mode is not turned on.');
  }
  $open_basedir = t('Open basedir is not set.');
  $base = ini_get('open_basedir');
  if (!empty($base)) {
    $open_basedir = t('Open basedir is set to: %dir', array('%dir' => $base));
  }
  $messages = array(
    $safe_mode,
    $open_basedir,
    t('Edit additional streams under the <a href="@streams_link">Hotfolder Streams taxonomy</a>. Public and private streams are provided by default.',
      array('@streams_link' => url('admin/structure/taxonomy/hotfolder_streams'))),
  );
  $form['folders']['hotfolder_streams'] = array(
    '#type' => 'item',
    '#title' => t('Additonal Streams'),
    '#markup' => implode('</br>', $messages) . theme('table',
    array(
      'header' => array(t('Stream'), t('Path'), t('Notes')),
      'rows' => $streams,
    )));
  $form['errors'] = array(
    '#title' => t('Error configurations'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array(
    HOTFOLDERS_LOG_NONE => 'No logging',
    HOTFOLDERS_LOG_PART => 'Errors (some)',
    HOTFOLDERS_LOG_WARN => 'Warnings (most)',
    HOTFOLDERS_LOG_DBUG => 'Debug (all)',
  );
  $form['errors']['hotfolder_error_reporting'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Error reporting level'),
    '#description' => t('Select your preferred level of logging from the hotfolder system.'),
    '#default_value' => variable_get('hotfolder_error_reporting', HOTFOLDERS_LOG_PART),
  );
  $form['errors']['hotfolder_error_verbose'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Verbose mode'),
    '#description' => t('Send all hotfolder watchdogs to user messages as well as log. (Useful for debugging.)'),
    '#default_value' => variable_get('hotfolder_error_verbose', FALSE),
  );
  $form['retry'] = array(
    '#title' => t('Retry configurations'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['retry']['hotfolder_error_retry'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Enable retry'),
    '#description' => t('Retry a job if it fails.'),
    '#default_value' => variable_get('hotfolder_error_retry', TRUE),
  );
  $form['retry']['hotfolder_error_retry_max'] = array(
    '#type' => 'select',
    '#options' => range(0,10),
    '#title' => t('Retries'),
    '#description' => t('How many times to retry a failed job.'),
    '#default_value' => variable_get('hotfolder_error_retry_max', 3),
  );
  $form['threading'] = array(
    '#title' => t('Worker configurations'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array(
    '0' => t('Never timeout'),
    '15' => t('15 seconds'),
    '30' => t('30 seconds'),
    '45' => t('45 seconds'),
    '60' => t('1 minute'),
    '120' => t('2 minutes'),
    '180' => t('3 minutes'),
    '300' => t('5 minutes'),
    '600' => t('10 minutes'),
    '900' => t('15 minutes'),
    '1800' => t('30 minutes'),
    '3600' => t('1 hour'),
  );
  $form['threading']['hotfolder_timeout'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t('Timeout'),
    '#description' => t('Some jobs may take a long time to complete. How long should we wait to timeout?'),
    '#default_value' => variable_get('hotfolder_timeout', 30),
  );
  $form['threading']['hotfolder_threading'] = array(
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#title' => t('Enable thread limits'),
    '#description' => t('Limit how many jobs can be in progress at the same time.'),
    '#default_value' => variable_get('hotfolder_threading', TRUE),
  );
  $form['threading']['hotfolder_threading_max'] = array(
    '#type' => 'select',
    '#options' => range(0, 20),
    '#title' => t('Job limit'),
    '#description' => t('How many jobs to allow simultaniously.'),
    '#default_value' => variable_get('hotfolder_threading_max', 16),
  );
  // Controls.
  $form['hotfolder_controls'] = array(
    '#title' => t('Controls'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Include the forms.
  module_load_include('inc', 'hotfolder', 'hotfolder.forms');
  // Call the forms.
  $submit_watches = _hotfolder_run_watches();
  if (!empty($submit_watches)) {
    $form['hotfolder_controls']['submit_watches'] = $submit_watches;
  }
  $submit_jobs = _hotfolder_run_jobs();
  if (!empty($submit_jobs)) {
    $form['hotfolder_controls']['submit_jobs'] = $submit_jobs;
  }
  $reset_threads = _hotfolder_reset_threads();
  if (!empty($reset_threads)) {
    $form['hotfolder_controls']['reset_threads'] = $reset_threads;
  }
  $toggle_cron = _hotfolder_toggle_cron();
  if (!empty($toggle_cron)) {
    $form['hotfolder_controls']['toggle_cron'] = $toggle_cron;
  }
  $run_cron = _hotfolder_run_cron();
  if (!empty($run_cron)) {
    $form['hotfolder_controls']['run_cron'] = $run_cron;
  }
  return system_settings_form($form);
}
