<?php

/**
 * @file
 * Administrative functions for PHP fatal error module.
 */

/**
 * Form builder; settings form.
 *
 * @see hide_php_fatal_error_settings_form_validate()
 * @ingroup forms
 */
function hide_php_fatal_error_settings_form($form, &$form_state) {
  $form['hide_php_fatal_error_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Error page'),
    '#size' => 40,
    '#default_value' => (variable_get('hide_php_fatal_error_page') != 'site-error' ? drupal_get_path_alias(variable_get('hide_php_fatal_error_page', 'site-error')) : ''),
    '#description' => t('Specify an internal path to which the user is redirected when a PHP fatal error occurs.'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler; validate settings form.
 *
 * @see hide_php_fatal_error_settings_form()
 */
function hide_php_fatal_error_settings_form_validate($form, &$form_state) {
  $path = empty($form_state['values']['hide_php_fatal_error_page']) ? 'site-error' : drupal_get_normal_path($form_state['values']['hide_php_fatal_error_page']);
  form_set_value($form['hide_php_fatal_error_page'], $path, $form_state);

  // Validate path.
  if (!drupal_valid_path($path)) {
    form_set_error('hide_php_fatal_error_page', t("The path '%path' is either invalid or you do not have access to it.", array('%path' => $path)));
  }
}
