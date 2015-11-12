<?php

/**
 * @file
 * Admin functions for Commerce Urbano.
 */

/**
 * Form builder function for module settings.
 */
function commerce_urbano_settings_form() {
  $form = array();

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
    '#description' => t('Enter your Urbano credentials.'),
  );
  $form['credentials']['commerce_urbano_client'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Nr.'),
    '#default_value' => variable_get('commerce_urbano_client', ''),
    '#required' => TRUE,
  );
  $form['credentials']['commerce_urbano_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#default_value' => variable_get('commerce_urbano_key', ''),
    '#required' => TRUE,
  );
  $form['credentials']['commerce_urbano_server'] = array(
    '#type' => 'radios',
    '#title' => t("Urbano's server"),
    '#options' => array(
      'sandbox' => t('Sandbox'),
      'live' => t('Live'),
    ),
    '#default_value' => variable_get('commerce_urbano_server', 'sandbox'),
    'sandbox' => array('#description' => t('Use for testing.')),
    'live' => array('#description' => t('Use for real shipping.')),
  );

  return system_settings_form($form);
}
