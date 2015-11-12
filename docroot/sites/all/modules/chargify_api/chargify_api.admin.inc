<?php

/**
 * @file
 *   Holds all admin forms and handlers.
 */

/**
 * Configuration settings form.
 */
function chargify_api_settings_form($form, &$form_state) {
  $form = array();

  $test_mode = variable_get('chargify_api_test_mode', TRUE);

  $form['postback_url'] = array(
    '#type' => 'item',
    '#title' => 'Post-Back URL',
    '#value' => url('chargify-postback', array('absolute' => TRUE)),
    '#description' => t('In order to notify this site of events that happen inside the Chargify application, such as new signups and accounts entering dunning, you can add this url to your Post-Back Settings page within Chargify.'),
  );

  $domain = $test_mode ? variable_get('chargify_api_test_domain', '') : variable_get('chargify_api_domain', '');
  if ($domain) {
    $url = url('http://' . $domain . '.chargify.com/settings', array('fragment' => 'postback-settings'));
    $form['postback_url']['#description'] .= t(' Please visit <a href="!url">your Chargify account</a> to update this value.', array('!url' => $url));
  }

  $form['test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test mode'),
    '#collapsible' => TRUE,
    '#collapsed' => !$test_mode,
  );
  $form['test']['chargify_api_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test Mode'),
    '#default_value' => $test_mode,
  );
  $form['test']['chargify_api_test_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Test API Key'),
    '#default_value' => variable_get('chargify_api_test_api_key', ''),
  );
  $form['test']['chargify_api_test_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Test Subdomain'),
    '#default_value' => variable_get('chargify_api_test_domain', ''),
    '#description' => t('It is advised to use a different Chargify domain for testing, as there is currently no way to remove test subscriptions in Chargify.'),
  );

  $form['live'] = array(
    '#type' => 'fieldset',
    '#title' => t('Live mode'),
    '#collapsible' => TRUE,
    '#collapsed' => $test_mode,
  );
  $form['live']['chargify_api_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('chargify_api_api_key', ''),
    '#required' => !$test_mode,
  );
  $form['live']['chargify_api_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#default_value' => variable_get('chargify_api_domain', ''),
    '#required' => !$test_mode,
  );

  return system_settings_form($form);
}
