<?php
/**
 * @file
 */

/**
 * Settings form callback.
 */
function webducateapi_form($form, $form_state) {
  $form = array();

  $settings = variable_get('webducateapi_settings', array());

  $form['webducateapi_settings'] = array(
    '#title' => t('API settings'),
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#collapsible' => FALSE,
  );

  $form['webducateapi_settings']['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['url']) ? $settings['url'] : 'http://portal.webducate.com.au/services/xmlrpc',
    '#required' => TRUE,
  );

  $form['webducateapi_settings']['key'] = array(
    '#title' => t('Key'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['key']) ? $settings['key'] : NULL,
    '#required' => TRUE,
  );

  $pathinfo = pathinfo($GLOBALS['base_url']);
  $form['webducateapi_settings']['domain'] = array(
    '#title' => t('Domain'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['domain']) ? $settings['domain'] : $pathinfo['basename'],
    '#required' => TRUE,
  );

  $form['webducateapi_settings']['username'] = array(
    '#title' => t('Username'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['username']) ? $settings['username'] : NULL,
    '#required' => TRUE,
  );

  $form['webducateapi_settings']['password'] = array(
    '#title' => t('Password'),
    '#type' => 'textfield',
    '#default_value' => isset($settings['password']) ? $settings['password'] : NULL,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
