<?php

/**
 * @file
 * Waywire admin settings.
 */

/**
 * Waywire Settings form.
 */
function waywire_settings_form($form) {
  $form['waywire_api_key'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('API Key'),
    '#description' => t('The api key found in your Waywire admin settings.'),
    '#required' => TRUE,
    '#default_value' => variable_get('waywire_api_key'),
  );
  $form['waywire_channel'] = array(
    '#type' => 'textfield',
    '#size' => 60,
    '#title' => t('Waywire Channel URL'),
    '#required' => TRUE,
    '#default_value' => variable_get('waywire_channel'),
  );

  return system_settings_form($form);
}
