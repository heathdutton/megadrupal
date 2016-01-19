<?php
/**
 * @file
 * This file contains the admin functions for the FlickrUp module.
 */

/**
 * Implements hook_settings().
 */
function flickrup_admin_settings() {
  $api_key = variable_get('flickrup_api_key', '');
  $api_secret = variable_get('flickrup_api_secret', '');

  $form['flickrup_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#required' => TRUE,
    '#default_value' => $api_key,
    '#description' => t('Flickr API Key'),
  );

  $form['flickrup_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#required' => TRUE,
    '#default_value' => $api_secret,
    '#description' => t('Flickr API Secret'),
  );

  $form['flickrup_auth_token'] = array(
    '#type' => 'markup',
    '#theme' => 'flickrup_auth_token',
  );

  $form['flickrup_css'] = array(
    '#prefix' => '<label>' . t('Style') . '</label>',
    '#type' => 'checkbox',
    '#title' => t('Use flickrup.css'),
    '#default_value' => variable_get('flickrup_css', 1),
    '#description' => t('Uncheck to take care of the styling yourself in custom CSS.'),
  );

  $form['flickrup_is_public'] = array(
    '#type' => 'select',
    '#title' => t('Is public'),
    '#default_value' => variable_get('flickrup_is_public', 0),
    '#options' => array(
      0 => t('no'),
      1 => t('yes'),
    ),
  );

  if (variable_get('flickrup_auth_process_step', 0) === 2) {
    flickrup_set_error(t('An error occurred in authorization process. Please check your authorization information'));
    variable_set('flickrup_auth_process_step', 0);
  }

  $form['#validate'][] = 'flickrup_admin_settings_validate';
  $form['#submit'][] = 'flickrup_admin_settings_submit';
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'flickrup') . '/css/flickrup-admin.css',
  );

  return system_settings_form($form);
}

/**
 * Validates user input.
 */
function flickrup_admin_settings_validate($form, &$form_state) {
  $api_key = trim($form_state['values']['flickrup_api_key']);
  $api_secret = trim($form_state['values']['flickrup_api_secret']);

  if ($api_key && (preg_match('/^[A-Fa-f\d]{32}$/', $api_key) != 1)) {
    form_set_error('flickrup_api_key', t('This does not appear to be a Flickr API Key.'));
  }

  if ($api_secret && (preg_match('/^[A-Fa-f\d]{16}$/', $api_secret) != 1)) {
    form_set_error('flickrup_api_secret', t('This does not appear to be a Flickr API Secret.'));
  }
}

/**
 * Submits form data.
 */
function flickrup_admin_settings_submit($form, &$form_state) {
  // Clean up the data.
  $api_key = trim($form_state['values']['flickrup_api_key']);
  $api_secret = trim($form_state['values']['flickrup_api_secret']);
  $default_api_key = variable_get('flickrup_api_key', '');
  $default_api_secret = variable_get('flickrup_api_secret', '');

  variable_set('flickrup_api_key', $api_key);
  variable_set('flickrup_api_secret', $api_secret);

  if (!_flickrup_is_authorized()) {
    variable_set('flickrup_auth_process_step', 1);
    $form_state['redirect'] = 'flickrup/auth';
  }
  elseif ($api_key != $default_api_key || $api_secret != $default_api_secret) {
    variable_set('flickrup_auth_process_step', 1);
    $form_state['redirect'] = 'flickrup/auth';
  }
}

/**
 * Authorization callback.
 */
function flickrup_auth() {
  global $base_url;
  $auth_process_step = variable_get('flickrup_auth_process_step', 0);
  $query_parameters = drupal_get_query_parameters();

  $flickr = flickrup_phpFlickr();

  if ($auth_process_step === 1) {
    variable_del('flickrup_auth_token');
    variable_set('flickrup_auth_process_step', 2);
    $flickr->auth('delete');
  }
  elseif ($auth_process_step === 2 && isset($query_parameters['frob'])) {
    $frob = $query_parameters['frob'];
    if ($response = $flickr->auth_getToken($frob)) {
      $token = is_array($response['token']) ? $response['token']['_content'] : $response['token'];
      variable_set('flickrup_auth_token', $token);
      variable_set('flickrup_auth_process_step', 0);
      drupal_set_message(t('You have successfully got auth token: @token', array('@token' => $token)));
    }
    else {
      flickrup_set_error(t('An error occurred in authorization process and auth token did not get.'));
    }
    if (module_exists('overlay')) {
      $options = array('fragment' => 'overlay=admin/config/media/flickrup');
      drupal_goto($base_url, $options);
    }
    else {
      drupal_goto('admin/config/media/flickrup');
    }
  }
  else {
    if (module_exists('overlay')) {
      $options = array('fragment' => 'overlay=admin/config/media/flickrup');
      drupal_goto($base_url, $options);
    }
    else {
      drupal_goto('admin/config/media/flickrup');
    }
  }
}
