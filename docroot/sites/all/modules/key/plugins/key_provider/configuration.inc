<?php

/**
 * @file
 * Plugin definition for the Configuration key provider.
 */

$plugin = array(
  'title' => t('Configuration'),
  'description' => t('Saves the key in configuration in the database.'),
  'key get value' => 'key_provider_configuration_get_key',
  'config form submit' => 'key_provider_configuration_config_form_submit',
  'key value form' => 'key_provider_configuration_key_value_form',
);

/**
 * Return a key stored with the configuration.
 */
function key_provider_configuration_get_key($config) {
  if (isset($config['provider_settings']['key_value'])) {
    $key_value = $config['provider_settings']['key_value'];
  }
  else {
    $key_value = NULL;
  }

  // Base64-decode if necessary.
  if (isset($config['provider_settings']['base64_encoded']) && $config['provider_settings']['base64_encoded']) {
    $key_value = base64_decode($key_value);
  }

  return $key_value;
}

/**
 * Submit callback.
 */
function key_provider_configuration_config_form_submit($form, &$form_state) {
  $form_state['values']['provider_settings']['key_value'] = $form_state['values']['key_value']['key_text'];
}

function key_provider_configuration_key_value_form($defaults) {
  $form = array();

  $form['key_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Key value'),
    '#description' => t('Enter the key to save it to the database.'),
    '#required' => TRUE,
    '#default_value' => isset($defaults['key_value']) ? $defaults['key_value'] : '',
  );

  return $form;
}
