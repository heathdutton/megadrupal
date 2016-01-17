<?php

/**
 * @file
 * Code required for KeyCAPTCHA
 */

/**
 * Implementation of admin settings().
 */
function keycaptcha_admin_settings() {
  $form = array();
  $form['keycaptcha_private_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Private Key'),
    '#default_value' => variable_get('keycaptcha_private_key', ''),
    '#maxlength' => 40,
    '#description' => t('You can get this key in "My sites" section at <a href="https://www.keycaptcha.com"target="_blank">www.keycaptcha.com</a> (Please note that "My sites" section is available only for users who are logged in)'),
    '#required' => FALSE,
  );
  $form['keycaptcha_js_code'] = array(
    '#type' => 'textarea',
    '#title' => t('KeyCAPTCHA code'),
    '#default_value' => variable_get('keycaptcha_js_code', ''),
    '#description' => t('You can find this code in "My sites" section at <a href="https://www.keycaptcha.com">www.keycaptcha.com</a> (Please note that "My sites" section is available only for users who are logged in)'),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

