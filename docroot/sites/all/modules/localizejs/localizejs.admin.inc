<?php

/**
 * @file
 * Administrative page callbacks for the localizejs module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function localizejs_admin_settings_form($form_state) {
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['account']['localizejs_account'] = array(
    '#title' => t('Project Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('localizejs_account', ''),
    '#size' => 15,
    '#maxlength' => 20,
    '#required' => TRUE,
    '#description' => t('Put here your localize project key. Create your project on https://localizejs.com.'),
  );

  return system_settings_form($form);
}

/**
 * Implements _form_validate().
 */
function localizejs_admin_settings_form_validate($form, &$form_state) {
    
}

/**
 * Implements _form_submit().
 */
function localizejs_admin_settings_form_submit($form, &$form_state) {
    
}
