<?php

/**
 * @file
 * Form callbacks for administering Magneto Integration.
 */

/**
 * Form callback to refine integration settings with Magento.
 */
function magento_admin_settings_form($form, &$form_state) {
  $form['magento_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Magento API Username'),
    '#description' => t('This is an account in Magento with permissions to use the Magento CORE API.'),
    '#default_value' => variable_get('magento_api_username'),
  );

  $form['magento_api_password'] = array(
    '#type' => 'password',
    '#title' => t('Magento API Password'),
    '#description' => t('Enter a password to update the current password. Leave it blank to not change it.'),
  );

  $form['http'] = array(
    '#type' => 'fieldset',
    '#title' => t('HTTP authentication'),
    '#description' => t('If your Magento site uses HTTP authentication (think popup username/password prompt), configure that here.'),
  );

  $form['http']['magento_use_http_authentication'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use HTTP Authentication'),
    '#default_value' => variable_get('magento_use_http_authentication', FALSE),
  );
  $form['http']['magento_http_authentication_login'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('magento_http_authentication_login', ''),
  );
  $form['http']['magento_http_authentication_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
  );

  $form['test'] = array(
    '#type' => 'submit',
    '#value' => t('Test API Connection'),
    '#submit' => array('magento_admin_settings_test_api'),
  );

  $form['#submit'][] = 'magento_admin_settings_form_submit_before';
  $form = system_settings_form($form);
  $form['#submit'][] = 'magento_admin_settings_form_submit_after';

  return $form;
}

/**
 * Custom submit handler for updating admin settings.
 */
function magento_admin_settings_form_submit_before($form, &$form_state) {
  // Only set the password if they actually entered text into the field.
  if (empty($form_state['values']['magento_api_password'])) {
    unset($form_state['values']['magento_api_password']);
  }
  if (empty($form_state['values']['magento_http_authentication_password'])) {
    unset($form_state['values']['magento_http_authentication_password']);
  }
}

/**
 * Custom submit handler after all variables have been saved.
 */
function magento_admin_settings_form_submit_after($form, &$form_state) {
  module_load_include('inc', 'magento');
  magneto_test_api_connection();
}

/**
 * Submit handler to test the connection to the form.
 */
function magento_admin_settings_test_api($form, &$form_state) {
  module_load_include('inc', 'magento');
  magneto_test_api_connection();
}
