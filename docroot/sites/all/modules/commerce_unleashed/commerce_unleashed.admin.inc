<?php
/**
 * @file
 *   commerce_unleashed.admin.inc
 *   Administrtive forms for Commerce Exactor module
 */

/**
 * Settings form
 */
function commerce_unleashed_settings_form($form_state) {
  $form['commerce_unleashed_url'] = array(
    '#type'=> 'textfield',
    '#title' => t('API URL'),
    '#default_value' => variable_get('commerce_unleashed_url', 'https://api.unleashedsoftware.com/'),
    '#description' => t('The URL for the API to connect to. https://api.unleashedsoftware.com/'),
    '#required' => TRUE,
  );

  $form['commerce_unleashed_id'] = array(
    '#type' => 'textfield',
    '#title' => t('API ID'),
    '#default_value' => variable_get('commerce_unleashed_id'),
    '#description' => t('Your ID.'),
    '#required' => TRUE,
  );

  $form['commerce_unleashed_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('commerce_unleashed_key'),
    '#description' => t('Your Key.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
