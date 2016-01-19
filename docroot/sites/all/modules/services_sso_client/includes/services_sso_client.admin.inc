<?php

function services_sso_client_user_admin_form($form_state) {
  $form['services_sso_client_server_address'] = array(
    '#title' => t('Server address'),
    '#description' => t('The address of the server in the format of http://exammple.com/subdir without the trailing slash. The /subdir part is only necessary if you Services Drupal site is not on the root of the domain.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('services_sso_client_server_address', ''),
  );
  $form['services_sso_client_server_endpoint'] = array(
    '#title' => t('Endpoint name'),
    '#description' => t('The name of the endpoint on the Services REST service. Example: authentication'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('services_sso_client_server_endpoint', ''),
  );
  $form['services_sso_client_api_key'] = array(
    '#title' => t('API key'),
    '#description' => t('API key obtained from the Services SSO server provider.'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('services_sso_client_api_key', ''),
  );

  $form['services_sso_client_last_verified_threshold'] = array(
    '#type' => 'textfield',
    '#size' => 3,
    '#title' => t('Number of seconds between remote session re-validation'),
    '#required' => TRUE,
    '#default_value' => variable_get('services_sso_client_last_verified_threshold', 600),
  );

  $form['services_sso_client_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Single sign-on cookie domain'),
    '#default_value' => variable_get('services_sso_client_cookie_domain', _services_sso_client_get_common_cookie_domain()),
    '#description' => t('The base domain to use in order to share cookies between sites with this SSO module enabled and the SSO server Drupal site. Note: you may only share a cookie on sites that share the same base domain. IE: <em>yoursite.co.uk</em> and <em>my.yoursite.co.uk</em>. In that case, you would put ".yoursite.co.uk" in this field. The leading period <strong>is important</strong>.'),
  );

  // Profile editing settings
  $form['profile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Profile options'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['profile']['services_sso_client_use_roles_from_sso'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use roles from the SSO server rather than interacting with roles locally.'),
    '#default_value' => variable_get('services_sso_client_use_roles_from_sso', FALSE),
  );
  $form['profile']['services_sso_client_only_external_account_editing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only use external profile for user account editing'),
    '#default_value' => variable_get('services_sso_client_only_external_account_editing', TRUE),
  );
  $form['profile']['services_sso_client_optional_load_remote_modal'] = array(
    '#type' => 'checkbox',
    '#title' => t("Use modal overlay to load remote dialogs."),
    '#default_value' => variable_get('services_sso_client_optional_load_remote_modal', FALSE),
  );

  // Extra settings
  $form['extra'] = array(
    '#type' => 'fieldset',
    '#title' => t('Extra options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['extra']['services_sso_client_optional_profile_fields'] = array(
    '#type' => 'textarea',
    '#title' => 'Profile Fields',
    '#description' => t('Other fields to be carried over from the sso server (non-standard profile fields). Separate remote field machine names from local field machine names with a |. Ex: field_profile_fname|profile_name'),
    '#default_value' => variable_get('services_sso_client_optional_profile_fields', ''),
    '#required' => FALSE,
  );

  // Controls
  $form['controls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Controls'),
  );
  $form['controls']['propagate'] = array(
    '#type' => 'button',
    '#value' => t('Propagate accounts from SSO server'),
    '#submit' => array('services_sso_client_propagate_batch'),
    '#executes_submit_callback' => TRUE,
  );

  // Debugging settings
  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debugging options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['debug']['services_sso_client_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log debugging info to watchdog'),
    '#default_value' => variable_get('services_sso_client_debug', TRUE),
  );

  // Experimental features
  $form['experimental'] = array(
    '#type' => 'fieldset',
    '#title' => t('Experimental features'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Experimental features are extremely unstable and buggy. Do not enable if you don\t know what you are doing!'),
  );
  $form['experimental']['services_sso_client_remote_registration_client'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remote registration client'),
    '#default_value' => variable_get('services_sso_client_remote_registration_client', FALSE),
  );
  $form['experimental']['services_sso_client_remote_registration_client_ask_email_twice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ask user to type email twice'),
    '#default_value' => variable_get('services_sso_client_remote_registration_client_ask_email_twice', FALSE),
  );

  return system_settings_form($form);
}

function services_sso_client_user_admin_form_validate($form, &$form_state) {
  $base_url = $form_state['values']['services_sso_client_server_address'] . '/' . $form_state['values']['services_sso_client_server_endpoint'];
  $data = array(
    'username' => 'admin',
    'password' => 'ishouldnotwork',
  );
  $data = http_build_query($data, '', '&');
  $headers = array(
    'Content-Type' => 'application/x-www-form-urlencoded',
  );

  $response = drupal_http_request($base_url . '/user/login', array('headers' => $headers, 'method' => 'POST', 'data' => $data));

  if ($response->code == '401' && stripos($response->error, 'Wrong username or password') !== FALSE) {
    if (variable_get('services_sso_client_server_address', '') . '/' . variable_get('services_sso_client_server_endpoint', '') != $base_url) {
      // Success
      drupal_set_message(t('The provided SSO server address seems to be valid.'));
    }

  }
  else {
    form_set_error('services_sso_client_server_address', t('The provided SSO server address does not seem like a valid Services 3.x (http://drupal.org/project/services) Drupal site. Please verify that the address is entered correctly. This is the message returned by the server: ') . $response->code . ' ' . $response->error);
  }
}

/**
 * Batch callback for propagating users accounts from SSO.
 */
function services_sso_client_propagate_batch() {
  $batch = array(
    'operations' => array(
      array('services_sso_client_propagate_batch_process', array()),
    ),
    'title' => t('Services SSO Client Propagate Batch'),
    'finished' => 'services_sso_client_propagate_batch_finished',
    'init_message' => t('Services SSO Client Propagate Batch is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Services SSO Client Propagate Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'services_sso_client') . '/includes/services_sso_client.batch.inc',
  );
  batch_set($batch);
  batch_process('admin/config/services/sso-client');
}
