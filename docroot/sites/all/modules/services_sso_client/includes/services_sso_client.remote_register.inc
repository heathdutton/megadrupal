<?php

/**
 * Implements hook_form_FORM_ID_alter().
 */
function services_sso_client_form_user_register_form_alter(&$form, &$form_state, $form_id) {

  /**
   * Remote SSO registration
   * A lot more coordination between the SSO server and SSO client site are
   * needed to make this work. So this should not be on by default.
   */
  if (variable_get('services_sso_client_remote_registration_client', FALSE)) { // @todo this should be off by default after development

    // Disable the username field
    // @todo determine if this belongs
    $form['account']['name']['#access'] = FALSE;

    $form['#submit'] = array('services_sso_client_user_register_form_submit');
    $form['#validate'] = array('services_sso_client_user_register_form_validate');

    // If enabled, ask for email to be entered twice like password
    if (variable_get('services_sso_client_remote_registration_client_ask_email_twice', FALSE)) {
      $email_translate = array(
        'email_translate' => array(
          'confirmTitle' => t('Email match:'),
          'confirmSuccess' => t('yes'),
          'confirmFailure' => t('no'),
        ),
      );
      $form['#attached']['js'] = array(array(
        'data' => array('services_sso_client' => $email_translate),
        'type' => 'setting',
      ));
      $form['#attached']['css'] = array(
        drupal_get_path('module', 'services_sso_client') . '/css/remote_registration.css',
      );

      $form['account']['mail']['#weight'] = $form['account']['name']['#weight'] + 1;
      $form['account']['mail']['#attributes']['autocomplete'] = 'off';

      $form['account']['mail_confirm'] = $form['account']['mail'];
      $form['account']['mail_confirm']['#title'] = t('Confirm e-mail address');
      $form['account']['mail']['#description'] = '';
      $form['account']['mail_confirm']['#weight']++;
      $form['account']['mail_confirm']['#attributes']['autocomplete'] = 'off';

      // Disable pasting.
      $form['account']['mail']['#attributes']['onpaste'] = 'return false;';
      $form['account']['mail_confirm']['#attributes']['onpaste'] = 'return false;';

      $form['account']['mail']['#prefix'] = '<div class="confirm-email-wrapper clearfix">';
      $form['account']['mail_confirm']['#suffix'] = '</div>';


      // Add additional validation for matching email addresses
      array_unshift($form['#validate'], 'services_sso_client_form_user_register_form_email_confirm_validate');
    }
  }

}

/**
 * Validation handler for confirming email and re-entered emails match.
 */
function services_sso_client_form_user_register_form_email_confirm_validate(&$form, &$form_state) {
  if ($form_state['values']['mail'] != $form_state['values']['mail_confirm']) {
    form_set_error('mail_confirm', t('The emails you typed do not match. Please check to make sure you have entered your email correctly!'));
  }
}

/**
 * Validation handler for remote_user_register_form.
 */
function services_sso_client_user_register_form_validate(&$form, &$form_state) {
  global $base_url;

  // If there are pre-existing errors, stop validating further and try to
  // see if any of the errors are related to non-matching emails.
  if (form_get_errors()) {
    $messages = drupal_get_messages('error', FALSE);
    $look_for = t('The emails you typed do not match. Please check to make sure you have entered your email correctly!');
    if (in_array($look_for, $messages['error'])) {
      $messages = drupal_get_messages('error');

      drupal_set_message($look_for, 'error');
      foreach ($messages['error'] as $message) {
        if ($message != $look_for)
        drupal_set_message($message, 'error');
      }
    }
    return;
  }

  $response = services_sso_client_http_request_user_register(array(
    'mail' => $form_state['values']['mail'],
    'mail_confirm' => $form_state['values']['mail_confirm'],
    'pass' => $form_state['values']['pass'],
    'field_profile_fname' => $form_state['values']['field_profile_fname'],
    'field_profile_lname' => $form_state['values']['field_profile_lname'],
    'field_referrer_base_url' => array(LANGUAGE_NONE => array(array('value' => $base_url))),
  ));

  if ($response->code == 406) {
    $data = drupal_json_decode($response->data);

    foreach ($data['form_errors'] as $key => $val) {
      form_set_error($key, $val);
    }
  }
  elseif ($response->code == 200) {
    $data = drupal_json_decode($response->data);

    if (!empty($data['uid'])) {
      if (empty($form_state['values']['pass'])) {
        $response = services_sso_client_http_request_user_update_password($data['uid'], REQUEST_TIME);
      }

      if (empty($form_state['values']['pass']) && $response->code != 200) {
        drupal_set_message('An error has occurred. We cannot log you into the site. Please contact an administrator for assistance.', 'error');
      }
      else {
        $response = services_sso_client_http_request_user_retrieve($data['uid']);

        if ($response->code == 200) {
          $remote_user = drupal_json_decode($response->data);
          $fake_form_state = array(
            'values' => array(
              'name' => $remote_user['name'],
              'pass' => empty($form_state['values']['pass']) ? REQUEST_TIME : $form_state['values']['pass'],
            ),
          );
          $tmp = services_sso_client_authenticate_remote($fake_form_state);

          if (empty($tmp)) {
            drupal_set_message(t('Your account was created but an error has occured attempting to autheticate with this account. Please contact the administrator.'), 'warning');
          }
        }
        //
      }
    }
  }
  else {
    watchdog('services_sso_client', t('Remote registration failure for @mail', array('@mail' => $form_state['values']['mail'])));
    drupal_set_message(t('An unexpected error has occured while trying to register a new user. Please try again later or contact technical support for assistance.'), 'error');
  }
}

/**
 * Submit handler for remote_user_register_form.
 */
function services_sso_client_user_register_form_submit(&$form, &$form_state) {

}

function services_sso_client_http_request_user_register($values) {
  // Construct full endpoint URL from saved settings in admin config.
  $endpoint_url = variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_server_endpoint', '');

  $headers = array(
    'Content-Type' => 'application/json',
  );

  return drupal_http_request($endpoint_url . '/user/register.json', array('headers' => $headers, 'method' => 'POST', 'data' => drupal_json_encode($values)));
}

function services_sso_client_http_request_user_retrieve($remote_uid) {
  $api_session_data = services_sso_client_propagate_api_account();

  // Construct full endpoint URL from saved settings in admin config.
  $endpoint_url = variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_server_endpoint', '');

  $headers = array(
    'Content-Type' => 'application/json',
    'Cookie' => $api_session_data->session_name . '=' . $api_session_data->sessid,
  );

  $response = drupal_http_request($endpoint_url . "/user/{$remote_uid}.json", array('headers' => $headers, 'method' => 'GET'));

  services_sso_client_propagate_api_account($api_session_data);

  return $response;
}

function services_sso_client_http_request_user_update_password($remote_uid, $pass = 'temp123') {
  $api_session_data = services_sso_client_propagate_api_account();

  // Construct full endpoint URL from saved settings in admin config.
  $endpoint_url = variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_server_endpoint', '');

  $headers = array(
    'Content-Type' => 'application/json',
    'Cookie' => $api_session_data->session_name . '=' . $api_session_data->sessid,
    'X-CSRF-Token' => _services_sso_client_get_csrf_token($api_session_data->session_name, $api_session_data->sessid),
  );

  $data = array(
    'pass' => $pass,
  );

  $response = drupal_http_request($endpoint_url . "/user/{$remote_uid}.json", array('headers' => $headers, 'method' => 'PUT', 'data' => drupal_json_encode($data)));

  services_sso_client_propagate_api_account($api_session_data);

  return $response;
}

