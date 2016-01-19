<?php

/**
 * @file
 * Oempro module admin settings.
 */

/**
 * Return the Oempro global settings form.
 */
function oempro_admin_settings() {
  // only show the list selection forms if account info provided
  $username = variable_get('oempro_username', false);
  $password = variable_get('oempro_password', false);
  $oempro_url = variable_get('oempro_url', false);
  if ($username && $password && $oempro_url) {
    $logged_in_admin = oempro_get_api_object();
//    oempro_debug($logged_in_admin, array());
    if (!empty($logged_in_admin->userLogged)) {
      $form['oempro_account_info'] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#title' => 'OEMPro Account Information',
      );
      $form['oempro_account_info']['oempro_connection_status'] = array(
        '#type' => 'item',
        '#markup' => t('Connected to a valid Oempro instance at @url using: @username.', array('@url' => $oempro_url, '@username' => $username))
      );
    }
  }
  $form['oempro_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Oempro API URL'),
    '#required' => TRUE,
    '#default_value' => variable_get('oempro_url', ''),
    '#description' => t('The external URL for your Oempro installation.'),
  );
  $form['oempro_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Oempro username'),
    '#required' => TRUE,
    '#default_value' => variable_get('oempro_username', ''),
    '#description' => t('The admin username used to login to your Oempro installation.'),
  );
  $form['oempro_password'] = array(
    '#type' => 'password',
    '#title' => t('Oempro password'),
    '#required' => TRUE,
    '#default_value' => variable_get('oempro_password', ''),
    '#description' => t('The admin password used to login to your Oempro installation.'),
  );
  $form['oempro_batch_limit'] = array(
    '#type' => 'select',
    '#options' => array(
      '1' => '1',
      '10' => '10',
      '25' => '25',
      '50' => '50',
      '75' => '75',
      '100' => '100',
      '250' => '250',
      '500' => '500',
      '750' => '750',
      '1000' => '1000',
      '2500' => '2500',
      '5000' => '5000',
      '7500' => '7500',
      '10000' => '10000',
    ),
    '#title' => t('Batch limit'),
    '#description' => t('Maximum number of users to process in a single cron run. It is recommended keeping this below 5000-10000. Ignored if updates take place on user add / edit.'),
    '#default_value' => variable_get('oempro_batch_limit', 100),
  );
  return system_settings_form($form);
}
