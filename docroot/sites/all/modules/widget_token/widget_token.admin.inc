<?php

/**
 * @file
 * Defines the administration pages for the Widget Token module.
 */

/*
 * Form callback functions.
 */

/**
 * Page callback for 'admin/config/content/widget-token'.
 */
function widget_token_admin_form() {
  // Initialize $form to an empty array which represents the default form
  // structure.
  $form = array();
  // Build the server base URL setting element.
  $form['widget_token_server_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Server base URL'),
    '#description' => t('The widget server base URL which will be used to resolve embed codes.'),
    '#default_value' => variable_get('widget_token_server_base_url', 'http://widgets.vlaanderen.be'),
    '#element_validate' => array('widget_token_element_validate_url'),
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  // Convert form to a variable settings form.
  return system_settings_form($form);
}

/*
 * Form element validation functions.
 */

/**
 * Form element validation for absolute URLs.
 */
function widget_token_element_validate_url($element, &$form_state) {
  // Get the element value.
  $url = $element['#value'];
  // Check if the URL is not empty and invalid.
  if (!empty($url) && !valid_url($url, TRUE)) {
    // Raise form error: Invalid absolute URL format.
    form_error($element, t('@field must contain an absolute URL.', array('@field' => $element['#title'])));
  }
}
