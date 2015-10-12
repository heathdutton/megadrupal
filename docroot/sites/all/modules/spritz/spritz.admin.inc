<?php

/**
 * @file spritz.admin.inc
 */

/**
 * Form builder; Sprtiz administration form.
 */
function spritz_admin_form($form) {
  $form = array();

  $form['spritz_client_id'] = array(
    '#title' => t('Client ID'),
    '#description' => t('Enter your Spritz application Client ID.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('spritz_client_id', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
