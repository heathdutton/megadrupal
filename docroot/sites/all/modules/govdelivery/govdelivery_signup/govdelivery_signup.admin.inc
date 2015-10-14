<?php
/**
 * @file
 * GovDelivery Signup Module Settings UI.
 */

/**
 * Define signup admin settings.
 *
 * @return array
 *   System settings form array.
 */
function govdelivery_signup_admin_settings() {
  $form = array();
  $form['govdelivery_signup_fieldsetdesc'] = array(
    '#type' => 'textfield',
    '#title' => t('Signup Box Label'),
    '#default_value' => variable_get('govdelivery_signup_fieldsetdesc', 'Stay informed'),
    '#maxlength' => 25,
    '#required' => FALSE,
  );
  $form['govdelivery_signup_buttonlabel'] = array(
    '#type' => 'textfield',
    '#title' => t('Button Label'),
    '#default_value' => variable_get('govdelivery_signup_buttonlabel', 'Sign me up'),
    '#maxlength' => 25,
    '#required' => TRUE,
  );
  $form['govdelivery_signup_description'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter a Description'),
    '#default_value' => variable_get('govdelivery_signup_description', 'Sign up for updates'),
    '#maxlength' => 100,
    '#required' => FALSE,
  );
  $form['govdelivery_signup_emaillabel'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail Address Box Label'),
    '#default_value' => variable_get('govdelivery_signup_emaillabel', 'E-mail address'),
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['govdelivery_signup_emailvalue'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail Address Box Text'),
    '#default_value' => variable_get('govdelivery_signup_emailvalue', 'Enter your e-mail address'),
    '#maxlength' => 100,
    '#required' => FALSE,
  );
  $form['govdelivery_signup_emaildesc'] = array(
    '#type' => 'textfield',
    '#title' => t('E-mail Description Box Text'),
    '#default_value' => variable_get('govdelivery_signup_emaildesc', 'E-mail address description'),
    '#maxlength' => 100,
    '#required' => FALSE,
  );
  $form['govdelivery_signup_clientcode'] = array(
    '#type' => 'textfield',
    '#title' => t('GovDelivery DCM Client Account Code'),
    '#default_value' => variable_get('govdelivery_signup_clientcode', ''),
    '#maxlength' => 20,
    '#required' => TRUE,
  );
  $form['govdelivery_signup_server'] = array(
    '#type' => 'textfield',
    '#title' => t('GovDelivery DCM URL (Include HTTPS://)'),
    '#default_value' => variable_get('govdelivery_signup_server', ''),
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  return system_settings_form($form);
}
