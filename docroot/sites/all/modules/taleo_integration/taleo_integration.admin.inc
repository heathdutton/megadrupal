<?php

/**
 *@file This file includes the admin settings for the taleo intergration module.
 *
 */


/**
 * @return
 * An array that contains the values for the admin settings form
 *
 */
function taleo_integration_admin_settings($form, &$form_state) {
  // Username
  $form['taleo_username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('taleo_username', ''),
    '#description' => t('Your login username on  <a href="https://tbe.taleo.net/login/index.html">Taleo Website</a>.'),
  );
  // Password
  $form['taleo_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#default_value' => variable_get('taleo_password', ''),
    '#description' => t('Your password on  <a href="https://tbe.taleo.net/login/index.html">Taleo Website</a>.'),
  );
  // Company name
  $form['taleo_company_code'] = array(
    '#type' => 'textfield',
    '#title' => t('Company Code'),
    '#default_value' => variable_get('taleo_company_code', ''),
    '#description' => t('Your Company Code on  <a href="https://tbe.taleo.net/login/index.html">Taleo Website</a>.'),
  );
  // Dispatcher WSDL
  $form['taleo_wsdl_dispatcher'] = array(
    '#type' => 'textfield',
    '#title' => t('Taleo dispatcher WSDL'),
    '#default_value' => variable_get('taleo_wsdl_dispatcher', 'https://tbe.taleo.net/wsdl/DispatcherAPI.wsdl'),
    '#description' => t('The dispatcher wsdl url will generally look like https://tbe.taleo.net/wsdl/DispatcherAPI.wsdl'),
  );
  // Web WSDL
  $form['taleo_wsdl_web'] = array(
    '#type' => 'textfield',
    '#title' => t('Taleo web  WSDL'),
    '#default_value' => variable_get('taleo_wsdl_web', 'https://tbe.taleo.net/wsdl/WebAPI.wsdl'),
    '#description' => t('The web wsdl url will generally look like https://tbe.taleo.net/wsdl/WebAPI.wsdl'),
  );


  return system_settings_form($form);
}
