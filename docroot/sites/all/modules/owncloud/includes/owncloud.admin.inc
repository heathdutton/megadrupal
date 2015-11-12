<?php

/**
 * Displays the ownCloud administration page.
 */
function owncloud_settings($form, &$form_state) {

  $form['owncloud_url'] = array(
    '#type' => 'textfield',
    '#title' => t('ownCloud URL'),
    '#default_value' => variable_get('owncloud_url', ''),
    '#required' => TRUE,
    '#description' => t('Provide the url under which you get access to the web interface of your ownCloud Server.'),
  );
  $form['owncloud_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#default_value' => variable_get('owncloud_secret', ''),
    '#required' => TRUE,
    '#description' => t('Use the same secret here and in your ownCloud settings, it is used to validate the requests between the two systems.'),
  );

  $form['#validate'][] = 'owncloud_settings_validate';

  return system_settings_form($form);
}

/**
 * Form validation handler for owncloud_settings().
 */
function owncloud_settings_validate(&$form, &$form_state) {
  $form_state['values']['owncloud_url'] = trim(trim($form_state['values']['owncloud_url']), '/');
  $old_secret = variable_get('owncloud_secret', '');
  variable_set('owncloud_secret', $form_state['values']['owncloud_secret']);
  $response = owncloud_request('validate.php', array(), $form_state['values']['owncloud_url']);
  variable_set('owncloud_secret',$old_secret);
  if (empty($response)) {
    form_error($form['owncloud_url'], t('Invalid credentials.'));
    return;
  }
  drupal_set_message(print_r($response, TRUE));
}
