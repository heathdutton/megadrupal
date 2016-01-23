<?php

/**
 * Form callback for module configuration.
 */
function eloqua_rest_api_admin_settings($form_state) {
  $form = array();

  $form['eloqua_rest_api_site_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Site name'),
    '#default_value' => variable_get('eloqua_rest_api_site_name', ''),
    '#description' => t("Your company's site name in Eloqua."),
    '#required' => TRUE,
  );

  $form['eloqua_rest_api_login_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Login name'),
    '#default_value' => variable_get('eloqua_rest_api_login_name', ''),
    '#description' => t('The user name used to verify access to the Eloqua REST API. Consider using an admin account whose credentials do not expire.'),
    '#required' => TRUE,
  );

  $form['eloqua_rest_api_login_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Login password'),
    '#default_value' => variable_get('eloqua_rest_api_login_password', ''),
    '#description' => t('The password associated with the user above; used to verify access to the API.'),
    '#required' => TRUE,
  );

  $form['eloqua_rest_api_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#default_value' => variable_get('eloqua_rest_api_base_url', NULL),
    '#description' => t('The base URL of the Eloqua instance against which API calls will be made. If left blank, this will default to %example.', array(
      '%example' => 'https://secure.eloqua.com',
    )),
  );

  return system_settings_form($form);
}
