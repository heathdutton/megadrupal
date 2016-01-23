<?php
/**
 * @file
 */

/**
 * FAPI definition for settings page.
 */
function queued_invoke_settings_form() {
  $form = array();
  $form['queued_invoke_buffer_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Buffer size'),
    '#description' => t('Default buffer size for queue items)'),
    '#default_value' => variable_get('queued_invoke_buffer_size', QUEUED_INVOKE_BUFFER_SIZE),
  );

  $form = system_settings_form($form);
  return $form;
}

