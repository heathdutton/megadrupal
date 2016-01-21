<?php

/**
 * @file
 * Configure credentials for Gharpay.
 */

/**
 * Configuration form for Gharpay credentials.
 */
function gharpay_credentials_form($form, &$form_state) {

  $form = array();

  // Add a description about Gharpay.
  $form['gharpay_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t("API KEY"),
    '#required' => TRUE,
    '#description' => t("Enter the Api Key provided by gharpay.in"),
    '#default_value' => variable_get('gharpay_api_key', ''),
    );
  // Add a description about Gharpay.
  $form['gharpay_password'] = array(
    '#type' => 'password',
    '#title' => t("PASSWORD"),
    '#required' => TRUE,
    '#description' => t("Enter your gharpay.in password."),
    '#default_value' => variable_get('gharpay_password', ''),
    );
  // Add a description about Gharpay.
  $form['gharpay_service_url'] = array(
    '#type' => 'textfield',
    '#title' => t("SERVICE URL"),
    '#required' => TRUE,
    '#description' => t("Enter the service url."),
    '#default_value' => variable_get('gharpay_service_url', 'http://services.gharpay.in'),
    );

  return system_settings_form($form);
}