<?php
/**
 * @file
 * Module that implements two factor authentication using Authy.
 */

/**
 * System Settings Form callback: authy_settings().
 *
 * Creates the settings form for the Authy module.
 */
function authy_form_settings($form, &$form_state) {
  $form['authy_host_uri'] = array(
    '#title' => t('Authy API'),
    '#type' => 'select',
    '#options' => array(
      'https://api.authy.com' => t('Production API'),
      'http://sandbox-api.authy.com' => t('Sandbox API'),
    ),
    '#default_value' => variable_get('authy_host_uri', 'https://api.authy.com'),
    '#required' => TRUE,
  );

  $form['authy_application'] = array(
    '#type' => 'textfield',
    '#title' => t('Authy Application name'),
    '#default_value' => variable_get('authy_application', ''),
    '#required' => TRUE,
  );

  $form['authy_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('authy_api_key'),
    '#description' => t('Your application API Key.'),
    '#required' => TRUE,
  );

  $form['authy_trigger_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Trigger threshold'),
    '#default_value' => variable_get('authy_trigger_threshold', '5'),
    '#options' => array(
      '2' => '2',
      '3' => '3',
      '4' => '4',
      '5' => '5',
      '7' => '7',
      '10' => '10'
    ),
    '#description' => t('Trigger the authy failed multiple times rule after this many failed attempts.'),
    '#required' => TRUE,
  );

  $authy_about = t(
    'Traditional security relies on something you know; your username and password. Authy increases your security by also requiring something you have.<br>'."\n".
    'Authy is available for iPhone, Android or via normal SMS. When required during logging in or when entering secure areas, you will be asked to enter a token.'."\n".
    'You simply open your Authy app and copy the short code to the website. If the app is not working for you, you can request an SMS be sent as a backup method.<br>'."\n".
    'Our server will then confirm your token is as expected. If your password is ever compromised, your account will still be safe.'
  );

  $form['authy_about'] = array(
    '#type' => 'textarea',
    '#title' => t('About text'),
    '#default_value' => variable_get('authy_about', $authy_about),
    '#description' => t('Enter a short about text that will be displayed on the users Authy configuration page.'),
    '#required' => FALSE,
  );

  $form['authy_forms'] = array(
    '#type' => 'textarea',
    '#title' => t('Require Authy in the following forms'),
    '#default_value' => variable_get('authy_forms'),
    '#description' => t('Enter form IDs, one on each line.'),
    '#required' => FALSE,
  );

  $form['authy_show_form_id'] = array(
    '#title' => t('Show Drupal form id'),
    '#type' => 'select',
    '#options' => array(
      'y' => t('Yes'),
      'n' => t('No'),
    ),
    '#default_value' => variable_get('authy_show_form_id', 'n'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
