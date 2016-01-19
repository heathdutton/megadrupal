<?php
/**
 * @file
 * Defines the administration pages for the webclient module.
 */

/*
 * Form generator functions.
 */

/**
 * Form generator function for 'admin/config/webclient/general'.
 */
function webclient_admin_form() {
  $form = array();
  // Get the current drupal_http_request() override function.
  $drupal_http_request_function = variable_get('drupal_http_request_function', FALSE);
  // Check if another module is already overridding the drupal_http_request().
  if (!$drupal_http_request_function && $drupal_http_request_function != WEBCLIENT_DRUPAL_HTTP_REQUEST_FUNCTION && function_exists($drupal_http_request_function)) {
    // Notify the user that another module is already overriding
    // drupal_http_request().
    drupal_set_message(t('Another module is already integrating the Drupal HTTP API.'), 'warning');
  }
  // Configure the Drupal HTTP request override.
  $form['webclient_drupal_http_request_override'] = array(
    '#type' => 'select',
    '#title' => t('Integrate with Drupal HTTP API?'),
    '#description' => t('Requires Drupal 7.22 or higher.'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => variable_get('webclient_drupal_http_request_override', FALSE),
    '#disabled' => version_compare(VERSION, '7.22', 'lt'),
  );
  // Convert the form to a settings form.
  $form = system_settings_form($form);
  // Attach our own submit handler.
  $form['#submit'][] = 'webclient_admin_form_submit';
  // Return the newly build form.
  return $form;
}

/*
 * Form submit handlers.
 */

/**
 * Apply WebClient configuration.
 */
function webclient_admin_form_submit($form, &$form_state) {
  // Get the WebClient override state.
  $override = (bool) $form['webclient_drupal_http_request_override']['#value'];
  // Check if drupal_http_request() needs to be overridden.
  if ($override) {
    // Override drupal_http_request().
    variable_set('drupal_http_request_function', WEBCLIENT_DRUPAL_HTTP_REQUEST_FUNCTION);
  }
  // Check if the drupal_http_request_function variable should be cleared.
  elseif (variable_get('drupal_http_request_function', FALSE) == WEBCLIENT_DRUPAL_HTTP_REQUEST_FUNCTION) {
    // Remove the drupal_http_request() override.
    variable_del('drupal_http_request_function');
  }
}
