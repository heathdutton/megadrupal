<?php

/**
 * @file raygun.admin.inc.
 */


/**
 * Administration form callback for raygun settings.
 */
function raygun_admin_form($form, &$form_state) {

  $form['common'] = array(
    '#type' => 'fieldset',
    '#title' => t('Common'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['common']['raygun_apikey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('API key'),
    '#description' => t('Raygun.io API key for the application.'),
    '#default_value' => variable_get('raygun_apikey', ''),
  );
  $form['common']['raygun_async_sending'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use asynchronous sending'),
    '#description' => t('If checked, the message will be sent asynchronously. This provides a great speedup versus the older cURL method. On some environments (e.g. Windows), you might be forced to uncheck this.'),
    '#default_value' => variable_get('raygun_async_sending', 1),
  );
  $form['common']['raygun_send_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send application version'),
    '#description' => t('If checked, all error messages to Raygun.io will include your application version that is currently running. This is optional but recommended as the version number is considered to be first-class data for a message.'),
    '#default_value' => variable_get('raygun_send_version', 1),
  );
  $form['common']['raygun_application_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Application version'),
    '#description' => t('What is the current version of your Drupal application. This can be any string or number or even a git commit hash.'),
    '#default_value' => variable_get('raygun_application_version', ''),
    '#states' => array(
      'invisible' => array(
        ':input[name="raygun_send_version"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['common']['raygun_send_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send current user email'),
    '#description' => t('If checked, all error messages to Raygun.io will include the current email address of any logged in users.  This is optional - if it is not checked, a random identifier will be used.'),
    '#default_value' => variable_get('raygun_send_username', 1),
  );


  $form['php'] = array(
    '#type' => 'fieldset',
    '#title' => t('PHP'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['php']['raygun_exceptions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Register global exception handler'),
    '#default_value' => variable_get('raygun_exceptions', 1),
  );
  $form['php']['raygun_error_handling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Register global error handler'),
    '#default_value' => variable_get('raygun_error_handling', 1),
  );

  $form['#validate'][] = 'raygun_admin_form_validate';
  return system_settings_form($form);
}

/**
 * Form validation function for raygun_admin_form().
 */
function raygun_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Simple API key pattern matching.
  // @TODO confirm that this is the correct pattern for all accounts.
  if (!preg_match("/^[0-9a-zA-Z\+\/]{22}==$/", $values['raygun_apikey'])) {
    form_set_error('raygun_apikey', t('You must specify a valid Raygun.io API key. You can find this on your dashboard.'));
  }

  $application_version = trim($values['raygun_application_version']);
  if ($values['raygun_send_version'] && empty($application_version)) {
    form_set_error('raygun_application_version', t('You must specify an application version if you are going to send this.'));
  }
}
