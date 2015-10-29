<?php

/**
 * @file
 * Module settings UI.
 */


function dokeos_admin_settings() {
  /**
   * Space construct aditional variables, do validation etc.
   */
  return drupal_get_form('dokeos_admin_settings_form');
}

function dokeos_admin_settings_form() {
  $form['dokeos_sso_protocol'] = array(
    '#type' => 'radios',
    '#title' => t('SSO Server Protocol'),
    '#options' => array('http://', 'https://'),
    '#default_value' => variable_get('dokeos_sso_protocol', 0),
  );

  $form['dokeos_server'] = array(
    '#type' => 'textfield',
    '#title' => t('Application URL and Path'),
    '#description' => t('NOTICE: Do NOT use trailing slash nor http://. i.e: yourdokeossite.com'),
    '#default_value' => variable_get('dokeos_server', ''),
  );

  $form['dokeos_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('User ID (example: user1).'),
    '#default_value' => variable_get('dokeos_user', ''),
  );
  $form['account']['dokeos_appkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Security Key'),
    '#default_value' => variable_get('dokeos_appkey', ''),
  );
  return system_settings_form($form);
}

