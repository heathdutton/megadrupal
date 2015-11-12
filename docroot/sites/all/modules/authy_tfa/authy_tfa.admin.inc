<?php
/**
 * @file
 * Administration functions for the Authy_TFA module.
 */

/**
 *
 */
function authy_tfa_admin_settings() {
  $form = array();

  $form['authy_description'] = array(
    '#markup' => t('<p>To get an API Key you will need to signup at <a href="https://www.authy.com">https://www.authy.com</a> and create an application.</p>'),
  );

  $form['authy_key'] = array(
    '#type' => 'fieldset',
    '#title' => t('Authy API Key'),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('authy_api_key', '') != ''),
  );
  $form['authy_key']['authy_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#description' => t('Get your API Key from https://dashboard.authy.com/'),
    '#default_value' => variable_get('authy_api_key', ''),
    '#required' => TRUE,
  );

  $form['authy_tfa'] = array(
    '#type' => 'fieldset',
    '#title' => t('Force Authy Two Factor'),
    '#description' => t('By checking this box every user will be forced to use Authy Two-Factor authentication.'),
    '#collapsible' => FALSE,
  );

  $form['authy_tfa']['authy_require_all'] = array(
    '#type' => 'checkbox',
    '#title' => t('Require Authy Two-Factor Authetication.'),
    '#default_value' => variable_get('authy_require_all', ''),
  );

  $form['authy_tfa_alt'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show alternate methods.'),
    '#description' => t("Alternate methods such as SMS or Phonecall may be used. Your account will need to be setup at least on the !plan plan to use these.", array('!plan' => l('Starter', 'https://www.authy.com/pricing'))),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['authy_tfa_alt']['authy_display_sms'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show SMS option'),
    '#default_value' => variable_get('authy_display_sms', 0),
  );

  $form['authy_tfa_alt']['authy_display_phonecall'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Phone Call option'),
    '#default_value' => variable_get('authy_display_phonecall', 0),
  );

  return system_settings_form($form);
}
