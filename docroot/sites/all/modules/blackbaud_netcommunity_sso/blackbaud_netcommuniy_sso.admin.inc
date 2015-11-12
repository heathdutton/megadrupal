<?php

/**
 * Blacbaud Login Module settings
 */
function blackbaud_netcommuniy_sso_admin_settings(){
  $form['blackbaud_netcommuniy_sso_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Secret Key',
    '#default_value' => variable_get('blackbaud_netcommuniy_sso_secret_key', NULL),
    '#description' => 'Please enter key in order configure your application as to login from blackbaud net community',
    '#required' => TRUE,
  );
  $form['blackbaud_netcommuniy_sso_site_url'] = array(
    '#type' => 'textfield',
    '#title' => 'Site URL',
    '#default_value' => variable_get('blackbaud_netcommuniy_sso_site_url', NULL),
    '#description' => 'Please enter site url in order configure your application as to login from blackbaud net community',
    '#required' => TRUE,
  );  
  return system_settings_form($form);
}