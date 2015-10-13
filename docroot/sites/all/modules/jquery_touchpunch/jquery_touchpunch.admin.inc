<?php

/**
 * @file
 * Admin form and validation.
 */

/**
 * Implements hook_requirements().
 */
function jquery_touchpunch_settings_form($form) {
  // Check library installed.
  $library = libraries_detect('jquery.ui.touch-punch');
  $error_message = isset($library['error message']) ? $library['error message'] : '';
  if ($error_message) {
    drupal_set_message(jquery_touchpunch_error_message($error_message, $library), 'error');
  }
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Settings'),
  );
  $form['global']['jquery_touchpunch_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable jQuery Touch Punch'),
    '#default_value' => variable_get('jquery_touchpunch_enabled', JQUERY_TOUCHPUNCH_ENABLED),
    '#description' => t('Uncheck this box to completely disable jQuery Touch Punch plugin.'),
  );
  $form['global']['jquery_touchpunch_variant'] = array(
    '#type' => 'radios',
    '#title' => t('Choose plugin compression type'),
    '#options' => array(
      'minified' => t('Production (Minified)'),
      'source' => t('Development (Uncompressed Code)'),
    ),
    '#default_value' => variable_get('jquery_touchpunch_variant', 'minified'),
  );

  return system_settings_form($form);
}

/**
 * Validation handler.
 */
function jquery_touchpunch_settings_form_validate($form, $form_state) {
  $library = libraries_detect('jquery.ui.touch-punch');
  if ($library['installed']) {
    $variant = $form_state['values']['jquery_touchpunch_variant'];
    if ($library['variants'][$variant]['installed'] == FALSE) {
      form_set_error('jquery_touchpunch_variant', t('Chosen variant is not installed.'));
    }
  }
  else {
    return form_set_error('global', t('Library is not installed.'));
  }
}
