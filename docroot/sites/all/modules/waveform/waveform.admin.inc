<?php
/**
 * @file
 * Administration forms and functions for the Waveform module.
 */

function waveform_settings_form($form, &$form_state) {
  $form = array();

  $form['waveform_lame_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Lame path'),
    '#description' => '',
    '#default_value' => variable_get('waveform_lame_path', 'lame'),
    '#required' => TRUE,
  );
  $form['wav2png'] = array(
    '#type' => 'fieldset',
    '#title' => t('wav2png settings'),
  );
  $form['wav2png']['waveform_wav2png_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => '',
    '#default_value' => variable_get('waveform_wav2png_path', 'wav2png'),
    '#required' => TRUE,
  );
  $form['wav2png']['waveform_wav2png_foreground_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Foreground color'),
    '#description' => '',
    '#default_value' => variable_get('waveform_wav2png_foreground_color', '508fa200'),
    '#required' => TRUE,
  );
  $form['wav2png']['waveform_wav2png_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => '',
    '#default_value' => variable_get('waveform_wav2png_background_color', 'f4f4f4ff'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

function waveform_update_waveform_form($form, &$form_state) {
  $form = array();

  $form['message'] = array(
    '#markup' => format_plural(_waveform_files_count(), 'There is 1 file to process.', 'There are @count files to process.'),
  );
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Process'),
  );

  return $form;
}

function waveform_update_waveform_form_submit($form, &$form_state) {
  $batch = array(
    // 'title' => t('Processing'),
    // 'init_message' => t('Initializing.'),
    // 'progress_message' => t('Completed @current of @total.'),
    // 'error_message' => t('An error has occurred.'),
    'operations' => array(
      array('_waveform_update_batch_operation', array()),
    ),
    'finished' => '_waveform_update_finished_callback',
    'file' => drupal_get_path('module', 'waveform') . '/waveform.admin.inc',
  );
  batch_set($batch);
}

function _waveform_update_batch_operation(&$context) {
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = _waveform_files_count();
  }

  $limit = 5;
  $files = _waveform_files($context['sandbox']['progress'], $limit);

  foreach ($files as $id => $file) {
    waveform_generate_waveform_image($file);
    $context['results'][] = $file->filename;
    $context['sandbox']['progress']++;
  }
  $context['message'] = t('Processed file !filename', array('!filename' => $file->filename));
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _waveform_update_finished_callback($success, $results, $operations) {
  if ($success) {
    drupal_set_message(format_plural(count($results), 'Processed 1 file.', 'Processed @count files.'));
  }
  else {
    drupal_set_message(t('An error has occurred.'), 'error');
  }
}

function _waveform_files_count() {
  $result = db_select('file_managed', 'f_m')
    ->condition('filemime', 'audio/mpeg', '=')
    ->condition('status', '1', '=')
    ->countQuery()
    ->execute()
    ->fetchField();

  return $result;
}

function _waveform_files($start = 0, $limit = 5) {
  $result = db_select('file_managed', 'f_m')
    ->condition('filemime', 'audio/mpeg', '=')
    ->condition('status', '1', '=')
    ->fields('f_m')
    ->range($start, $limit)
    ->execute()
    ->fetchAllAssoc('fid');
  return $result;
}
