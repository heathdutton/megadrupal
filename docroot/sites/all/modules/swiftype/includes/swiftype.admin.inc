<?php

/**
 * @file
 * Swiftype integration module admin functions.
 */

/**
 * Swiftype module admin form.
 */
function swiftype_integration_configure_form($form, &$form_state) {
  $form['swiftype_integration_install_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Swiftype install key'),
    '#description' => t('Enter your Swiftype install key'),
    '#default_value' => variable_get('swiftype_integration_install_key'),
    '#size' => 20,
    '#maxlength' => 20,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Swiftype admin form validation.
 */
function swiftype_integration_configure_form_validate($form, &$form_state) {
  $api_key = $form_state['values']['swiftype_integration_install_key'];
  $pattern = '/^[a-zA-Z0-9-_]+$/';

  if (!preg_match($pattern, $api_key)) {
    form_set_error('swiftype_integration_install_key', t('Swiftype install key
    should contain only letters and numbers'));
  }

  if (drupal_strlen($api_key) != 20) {
    form_set_error('swiftype_integration_install_key', t('Swiftype install key
    should be 20 characters long'));
  }
}
