<?php

/**
 * @file
 * This file is for all administration forms for the Bronto API module.
 */

/**
 * Admin settings form for the Bronto API.
 */
function brontoapi_admin_form() {
  $form = array();

  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bronto account'),
    '#collapsible' => FALSE,
  );

  $form['account']['brontoapi_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Bronto token'),
    '#description' => t('Token for Bronto API'),
    '#default_value' => variable_get('brontoapi_token', ''),
    "#required" => TRUE,
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mail configuration'),
    '#collapsible' => FALSE,
  );

  $form['settings']['brontoapi_default_from'] = array(
    '#type' => 'textfield',
    '#title' => t('Default sender address'),
    '#description' => t('This is the address that will appear on outgoing messages unless it is overridden in an API call'),
    '#default_value' => variable_get('brontoapi_default_from', ''),
    '#required' => TRUE,
  );

  $form['settings']['brontoapi_default_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Default sender name'),
    '#description' => t('This is the name that will appear on outgoing messages unless it is overridden in an API call'),
    '#default_value' => variable_get('brontoapi_default_from_name', ''),
    "#required" => TRUE,
  );

  $form['settings']['brontoapi_default_message_folder'] = array(
    '#type' => 'textfield',
    '#title' => t('Default message folder'),
    '#description' => t('The default location for saved sent messages in your Bronto account'),
    '#default_value' => variable_get('brontoapi_default_message_folder', 'Messages'),
    "#required" => TRUE,
  );

  $form['settings']['brontoapi_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#description' => t('If enabled, test mode will cause all messages to be sent to a single test list instead of what is passed to the API call. This is intended for testing new integration modules or templates.'),
    '#default_value' => variable_get('brontoapi_test_mode', 0),
  );

  $form['settings']['brontoapi_test_list'] = array(
    '#type' => 'textfield',
    '#title' => t('Test mode list'),
    '#description' => t('The name of the list to which all messages will be sent when test mode is enabled'),
    '#default_value' => variable_get('brontoapi_test_list', 'testing'),
  );

  return system_settings_form($form);
}
