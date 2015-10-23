<?php

/**
 * @file
 * CARE module settings form.
 */

/**
 * Drupal Form API function to generate settings form.
 */
function care_settings_form($form, &$form_state) {
  $form['care_wsdl_url'] = array(
    '#title' => t('CARE WSDL URL'),
    '#type' => 'textfield',
    '#description' => t('Use the button below to test the URL without saving it.'),
    '#length' => 50,
    '#default_value' => variable_get('care_wsdl_url', 'http://127.0.0.1/careservices/dataaccess.asmx?WSDL'));

  $form['test_wsdl'] = array(
    '#value' => t('Test WDSL URL'),
    '#type' => 'submit',
    '#submit' => array(
      'care_settings_form_test_wsdl'));

  $form['care_doc_root'] = array(
    '#title' => t('CARE documentation URL'),
    '#type' => 'textfield',
    '#description' => t('Home page for CARE API documentation.'),
    '#length' => 50,
    '#default_value' => variable_get('care_doc_root', 'http://127.0.0.1/careservices/documentation'));

  $form['logging'] = array(
    '#title' => 'Logging Options',
    '#type' => 'fieldset');

  $form['logging']['care_log_calls'] = array(
    '#title' => 'Log calls to CARE',
    '#type' => 'radios',
    '#options' => array(
      1 => 'Yes',
      0 => 'No'),
    '#default_value' => variable_get('care_log_calls', 0));

  $form['logging']['care_log_results'] = array(
    '#title' => 'Log results from CARE',
    '#type' => 'radios',
    '#options' => array(
      1 => 'Yes',
      0 => 'No'),
    '#default_value' => variable_get('care_log_results', 0));

  return system_settings_form($form);
}

/**
 * Drupal Form API validation for the settings form.
 */
function care_settings_form_test_wsdl($form, &$form_state) {
  $url = $form_state['values']['care_wsdl_url'];
  try {
    $client = @new SoapClient($url);
    drupal_set_message(t('CARE WSDL URL %url is OK.', array(
      '%url' => $url)));
    drupal_set_message(t('Click %savebutton to use it.', array(
      '%savebutton' => t('Save configuration'))));
  }
  catch (Exception $e) {
    drupal_set_message(t('CARE WSDL URL %url failed.', array(
      '%url' => $url)), 'error');
    drupal_set_message(t('Reverted to previous value.'), 'error');
  }
}
