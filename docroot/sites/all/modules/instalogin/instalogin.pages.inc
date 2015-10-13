<?php

/**
 * @file
 * InstaLogin pages.
 */

/**
 * Form callback for the settings form.
 */
function instalogin_admin_settings_form($form, &$form_state) {

  $form['instalogin'] = array(
    '#type' => 'fieldset',
    '#title' => 'InstaLogin authentication',
  );

  $form['instalogin']['instalogin_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable InstaLogin authentication'),
    '#default_value' => variable_get('instalogin_enabled', 0),
    '#description' => 'If enabled, those who have the secret key will be able to log in to the site as any user using the InstaLogin app.',
  );

  $form['instalogin']['instalogin_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#attributes' => array('readonly' => 'readonly'),
    '#default_value' => variable_get('instalogin_secret_key', ''),
    '#description' => t('Enter this secret key in the InstaLogin app. Generate a new key using the button below if you need to revoke access from the apps using this key.'),
  );

  $form['instalogin']['instalogin_regenerate'] = array(
    '#type' => 'submit',
    '#value' => t('Regenerate secret key'),
    '#submit' => array('instalogin_regenerate_key_submit'),
  );

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'instalogin') . '/instalogin.js',
  );

  return system_settings_form($form);
}

/**
 * Submit handler for the 'Regenerate secret key' button.
 */
function instalogin_regenerate_key_submit() {
  instalogin_generate_secret_key();
  drupal_set_message(t('A new secret key has been generated.'));
}

/**
 * Page callback for the service url.
 */
function instalogin_service_page() {

  if (!variable_get('instalogin_enabled', 0)) {
    return MENU_NOT_FOUND;
  }

  $error = FALSE;

  if ($_SERVER['REQUEST_METHOD'] != 'POST') {
    // Accept only POST requests.
    $error = 'Invalid request.';
  }

  if (!$error) {
    $data = file_get_contents('php://input');

    // Content should be encrypted json.
    $data = instalogin_decrypt_data($data);

    $params = array();

    if ($data) {
      $params = json_decode($data, TRUE);
    }

    if (empty($params)) {
      $error = 'Invalid request content. Please verify your secret key.';
    }
  }

  if (!$error) {
    $command = preg_replace('@[^a-z_]@', '', $params['cmd']);
    $callback = 'instalogin_service_' . $command . '_command';

    if (!function_exists($callback)) {
      $error = 'Invalid command.';
    }
  }

  if (!$error) {
    // Call the command requested in the 'cmd' parameter.
    $response = call_user_func($callback, $params);
  }
  else {
    $response = array(
      'status' => 'error',
      'error' => $error,
    );
  }

  drupal_json_output($response);
}

/**
 * Callback for the 'get_login' command.
 *
 * @param $params
 *   Array of parameters received with the request.
 *   Expected keys:
 *     'name': The name of the user to log in.
 *
 * @return
 *   Response array to be sent back to the client.
 */
function instalogin_service_get_login_command($params) {
  $error = FALSE;

  if (empty($params['name'])) {
    $error = 'User name not specified.';
  }
  else {
    $users = user_load_multiple(array(), array('name' => $params['name'], 'status' => '1'));

    $account = reset($users);
    if (!$account) {
      $error = 'This user does not exist.';
    }
  }

  if (!$error) {
    $login_url = user_pass_reset_url($account) . '/login';

    $response = array(
      'status' => 'ok',
      'data' => instalogin_encrypt_data($login_url),
    );
  }
  else {
    $response = array(
      'status' => 'error',
      'error' => $error,
    );
  }

  return $response;
}

/**
 * Callback for the 'validate_users' command.
 *
 * @param $params
 *   Array of parameters received with the request.
 *   Expected keys:
 *     'names': Comma separated list of user names to validate.
 *
 * @return
 *   Response array to be sent back to the client.
 */
function instalogin_service_validate_users_command($params) {
  $error = FALSE;

  if (empty($params['names']) || !is_string($params['names'])) {
    $error = 'User names not specified.';
  }
  else {
    $user_names = explode(',', $params['names']);

    $existing_names = db_select('users', 'u')
      ->fields('u', array('name'))
      ->condition('status', 1)
      ->condition('name', $user_names, 'IN')
      ->execute()
      ->fetchCol();

    $incorrect_names = array_diff($user_names, $existing_names);

    if (empty($incorrect_names)) {
      $result = 'verified';
    }
    else {
      $result = 'invalid:' . implode(',', $incorrect_names);
    }

  }

  if (!$error) {
    $response = array(
      'status' => 'ok',
      'data' => instalogin_encrypt_data($result),
    );
  }
  else {
    $response = array(
      'status' => 'error',
      'error' => $error,
    );
  }

  return $response;
}
