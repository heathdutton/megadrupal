<?php

/**
 * @file
 * Admin page callbacks for the Nimble CRM module.
 */

/**
 * Settings form for Nimble CRM.
 */
function nimble_crm_admin_settings_form($form) {
  $session_token = oauth2_client_get_token(NIMBLE_CRM_OAUTH_CLIENT);
  // Set token value into the database if it was initialized.
  if (!empty($session_token['access_token'])) {
    _nimble_crm_log('token', 'Token was stored in database.', $session_token);
    variable_set('nimble_crm_token', $session_token);
  }
  $form['nimble_crm_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('nimble_crm_client_id'),
    '#required' => TRUE,
  );
  $form['nimble_crm_client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secert'),
    '#default_value' => variable_get('nimble_crm_client_secret'),
    '#required' => TRUE,
  );

  $form['nimble_crm_log'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Log the following messages for debugging'),
    '#options' => array(
      'token' => t('Token API messages'),
      'contact' => t('Contact API messages'),
    ),
    '#default_value' => variable_get('nimble_crm_log', array()),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['#submit'][] = 'system_settings_form_submit';
  $form['#submit'][] = 'nimble_crm_admin_settings_form_submit';
  return $form;
}

/**
 * Submit handler for Nimble CRM settings form.
 */
function nimble_crm_admin_settings_form_submit($form, &$form_state) {
  // Closes overlay, before redirect page was open in overlay.
  if (module_exists('overlay') && overlay_get_mode() == 'child') {
    unset($_GET['destination']);
    overlay_close_dialog(NIMBLE_CRM_OAUTH_REDIRECT_PATH, array('external' => TRUE));
    $form_state['redirect'] = FALSE;
  }
  else {
    nimble_crm_api_token_get();
  }
}

/**
 * Callback for oauth2 authorized path.
 *
 * An authorized request in server-side flow
 * will be redirected here (having variables
 * 'code' and 'state').
 */
function nimble_crm_oauth2_authorized() {
  // If there is any error in the server response, display it.
  if (isset($_GET['error'])) {
    $error = $_GET['error'];
    $error_description = $_GET['error_description'];
    watchdog('nimble_crm', "Error: %error: %error_description", array(
      '%error' => $error,
      '%error_description' => $error_description,
    ), WATCHDOG_DEBUG);
  }
  nimble_crm_api_token_get();
  drupal_goto('admin/config/services/nimble-crm');
}
