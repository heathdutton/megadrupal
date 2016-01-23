<?php
/**
 * @file
 * Administration pages for the PHP-FFMpeg module.
 */

/**
 *  Form constructor for the PHP-FFMpeg settings form.
 */
function php_ffmpeg_settings($form, $form_state) {
  $form = array();
  $form['php_ffmpeg_ffmpeg_binary'] = array(
    '#type' => 'textfield',
    '#title' => t('ffmpeg binary'),
    '#description' => t('Path to the ffmpeg binary. Leave empty if the binary is located within system PATH.'),
    '#default_value' => variable_get('php_ffmpeg_ffmpeg_binary', NULL),
  );
  $form['php_ffmpeg_ffprobe_binary'] = array(
    '#type' => 'textfield',
    '#title' => t('ffprobe binary'),
    '#description' => t('Path to the ffprobe binary. Leave empty if the binary is located within system PATH.'),
    '#default_value' => variable_get('php_ffmpeg_ffprobe_binary', NULL),
  );
  $form['php_ffmpeg_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Timeout'),
    '#description' => t('Timeout for ffmpeg/ffprobe command execution in seconds. Leave empty for none.'),
    '#default_value' => variable_get('php_ffmpeg_timeout', NULL),
  );
  $form['php_ffmpeg_threads'] = array(
    '#type' => 'textfield',
    '#title' => t('Threads'),
    '#description' => t('Number of threads to use for ffmpeg commands.'),
    '#default_value' => variable_get('php_ffmpeg_threads', 0),
  );
  if (function_exists('monolog_channel_info_load_all') && ($channels = monolog_channel_info_load_all())) {
    $channel_options = array(NULL => t('-None-')) + array_map(function($channel) { return $channel['label'];}, $channels);
    $form['php_ffmpeg_monolog_channel'] = array(
      '#type' => 'select',
      '#title' => t('Monolog channel'),
      '#description' => t('Select the monolog channel to use for logging.'),
      '#default_value' => variable_get('php_ffmpeg_monolog_channel', NULL),
      '#options' => $channel_options,
    );
  }
  $form['#submit'][] = 'php_ffmpeg_settings_validate';
  return system_settings_form($form);
}

/**
 * Form validation handler for php_ffmpeg_settings().
 */
function php_ffmpeg_settings_validate($form, &$form_state) {
  if (!empty($form_state['values']['php_ffmpeg_ffmpeg_binary']) && !file_exists($form_state['values']['php_ffmpeg_ffmpeg_binary'])) {
    form_set_error('php_ffmpeg_ffmpeg_binary', t('File not found: @file', array('@file' => $form_state['values']['php_ffmpeg_ffmpeg_binary'])));
  }
  if (!empty($form_state['values']['php_ffmpeg_ffprobe_binary']) && !file_exists($form_state['values']['php_ffmpeg_ffprobe_binary'])) {
    form_set_error('php_ffmpeg_ffprobe_binary', t('File not found: @file', array('@file' => $form_state['values']['php_ffmpeg_ffprobe_binary'])));
  }
  if (!empty($form_state['values']['php_ffmpeg_timeout']) && (!is_numeric($form_state['values']['php_ffmpeg_timeout']) || $form_state['values']['php_ffmpeg_timeout'] < 0 || (intval($form_state['values']['php_ffmpeg_timeout']) != $form_state['values']['php_ffmpeg_timeout']))) {
    form_set_error('php_ffmpeg_timeout', t('The value of the Timeout field must be a positive integer.'));
  }
  if (!empty($form_state['values']['php_ffmpeg_threads']) && (!is_numeric($form_state['values']['php_ffmpeg_threads']) || $form_state['values']['php_ffmpeg_threads'] < 0) || (intval($form_state['values']['php_ffmpeg_threads']) != $form_state['values']['php_ffmpeg_threads'])) {
    form_set_error('php_ffmpeg_threads', t('The value of the Threads field must be zero or a positive integer.'));
  }
}