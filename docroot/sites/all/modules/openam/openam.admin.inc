<?php

/**
 * @file
 * The admin file of the module.
 */

/**
 * Create the admin configuration form.
 */
function openam_admin_form() {
  $form = array();

  // Add a general fieldset.
  $form['openam_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General configuration'),
    '#collapsible' => TRUE,
  );

  // Even if the module is activated, it is required to enabled the OpenAM authentication here.
  $form['openam_general']['openam_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable OpenAM'),
    '#default_value' => variable_get('openam_enabled', 0),
    '#description' => t('Activate OpenAM when a user trying to connect to Drupal.'),
    '#required' => FALSE,
  );

  // Add the possibility to automatically connect the user on each page.
  $form['openam_general']['openam_auto_connect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Auto connect'),
    '#default_value' => variable_get('openam_auto_connect', 0),
    '#description' => t('Automatically connect the user on each page, when the token is found in the session.'),
    '#required' => FALSE,
  );

  // If the user is automatically authenticated, define how he is logout (only logout from Drupal or logout from OpenAM too).
  $form['openam_general']['openam_logout_from_openam'] = array(
    '#type' => 'checkbox',
    '#title' => t('Logout from OpenAM'),
    '#default_value' => variable_get('openam_logout_from_openam', 0),
    '#description' => t('Authorized Drupal to destroy the OpenAM token. This will disconnect the user from the other system which are plugged to OpenAM. <b>BE CAREFUL</b>: if this option is not checked, the user will cannot disconnect from the Drupal (because he will be automatically reconnected to the OpenAM)'),
    '#states' => array(
      'disabled' => array(
        ':input[name="openam_auto_connect"]' => array('checked' => FALSE),
      ),
    ),
    '#required' => FALSE,
  );

  // Define the default page where the user will be redirected when he will be connected.
  $form['openam_general']['openam_default_page'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenAM Default Page'),
    '#default_value' => variable_get('openam_default_page', ''),
    '#size' => 25,
    '#maxlength' => 64,
    '#description' => t('Define where the user must be redirected when he will be logged. Empty by default means home page.'),
    '#required' => false,
  );

  // Add a cookie fieldset.
  $form['openam_cookie'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cookie information'),
    '#collapsible' => TRUE,
  );

  // Define the cookie name from which the token will be retrieved.
  $form['openam_cookie']['openam_cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenAM cookie name'),
    '#default_value' => variable_get('openam_cookie_name', ''),
    '#size' => 25,
    '#maxlength' => 64,
    '#description' => t('The cookie name from which user data will be retrieved.'),
    '#required' => TRUE,
  );

  // Define the cookie domain. This will be used for destroy the token when the user logout.
  $form['openam_cookie']['openam_cookie_path'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenAM cookie path'),
    '#default_value' => variable_get('openam_cookie_path', '/'),
    '#size' => 25,
    '#maxlength' => 64,
    '#description' => t('The cookie path for which it will be used.'),
    '#required' => TRUE,
  );

  // Define the cookie domain. This will be used for destroy the token when the user logout.
  $form['openam_cookie']['openam_cookie_domain'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenAM cookie domain'),
    '#default_value' => variable_get('openam_cookie_domain', ''),
    '#size' => 25,
    '#maxlength' => 64,
    '#description' => t('The cookie domain for which it will be used.'),
    '#required' => TRUE,
  );

  // Add a OpenAM configuration fieldset.
  $form['openam_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('OpenAM Configuration'),
    '#collapsible' => TRUE,
  );

  // Define the URL of the server.
  $form['openam_config']['openam_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenAM server URL'),
    '#default_value' => variable_get('openam_server_url', ''),
    '#size' => 64,
    '#maxlength' => 128,
    '#description' => t('The server URL of the open am, without trailing slash. For example : http://mydomain:8080/authservice'),
    '#required' => TRUE,
  );

  // If needed, define additional parameters, like REALM name.
  $form['openam_config']['openam_parameters'] = array(
    '#type' => 'textfield',
    '#title' => t('OpenAM server URL additionnal parameters'),
    '#default_value' => variable_get('openam_parameters', ''),
    '#size' => 64,
    '#maxlength' => 128,
    '#description' => t('Additional parameters to add to the request (for example, a REALM name)'),
    '#required' => FALSE,
  );

  // Add an attributes fieldset.
  $form['openam_attributes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attributes mapping'),
    '#collapsible' => TRUE,
  );

  // Define the username attribute, which will be used for connection.
  $form['openam_attributes']['openam_username_attribute'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => variable_get('openam_username_attribute', 'uid'),
    '#size' => 25,
    '#maxlength' => 64,
    '#description' => t('The name of the OpenAM profile attribute to use as the Drupal username. Use the default, <b>uid</b>, if users have the same username in Drupal and OpenAM.'),
    '#required' => TRUE,
  );

  // Define the mail attribute.
  $form['openam_attributes']['openam_mail_attribute'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail'),
    '#default_value' => variable_get('openam_mail_attribute', ''),
    '#size' => 25,
    '#maxlength' => 64,
    '#description' => t('Define the OpenAM attribute for the field mail.'),
    '#required' => TRUE,
  );

  // Add a mapping for each user field.
  $fields = field_info_instances('user', 'user');
  foreach ($fields as $field_name => $field_data) {
    $form['openam_attributes']['openam_' . $field_name . '_attribute'] = array(
      '#type' => 'textfield',
      '#title' => $field_data['label'],
      '#default_value' => variable_get('openam_' . $field_name . '_attribute', ''),
      '#size' => 25,
      '#maxlength' => 64,
      '#description' => t('Define the OpenAM attribute for the field %field', array(
        '%field' => $field_data['label'],
      )),
      '#required' => FALSE,
    );
  }

  return system_settings_form($form);
}

/**
 * Validate the admin form.
 */
function openam_admin_form_validate($form, &$form_state) {

  // Get the server URL and check if it is valid.
  $server_url = $form_state['values']['openam_server_url'];
  if (!valid_url($server_url, true)) {
    form_set_error('openam_server_url', t('OpenAM Base URL is not a valid URL.'));
  }
  if (strpos($server_url, 'http') !== 0) {
    form_set_error('openam_server_url', t('OpenAM Base URL scheme must be http or https.'));
  }

  // Make sure the base URL not ends in a slash.
  if (substr($server_url, -1) == '/') {
    $form_state['values']['openam_server_url'] = substr($server_url, 0, drupal_strlen($server_url) - 1);
  }
}
