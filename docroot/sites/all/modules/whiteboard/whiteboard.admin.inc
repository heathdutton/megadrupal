<?php

/**
 * @file
 * Whiteboard admin file for validating and specifying form whiteboard data
 */

/**
 * Form API callback to validate the upload settings form.
 */
function whiteboard_admin_settings_validate($form, &$form_state) {
  $size = $form_state['values']['whiteboard_size'];

  $path = file_exists($_SERVER['DOCUMENT_ROOT'] . base_path() . $form_state['values']['whiteboard_js2d_path']);

  if (!is_numeric($size) || ($size <= 0)) {
    form_set_error('whiteboard_size', t('The whiteboard size limit must be a number and greater than zero.'));
  }
  if (!$path) {
    form_set_error('whiteboard_js2d_path', t('jsDraw2D library not found.'));
  }
}

/**
 * Menu callback for the upload settings form.
 */
function whiteboard_admin_settings($form, &$form_state) {

  $form['settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
  );
  $form['settings_general']['whiteboard_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum file size of whiteboard'),
    '#default_value' => check_plain(variable_get('whiteboard_size', 500000)),
    '#size' => 8,
    '#maxlength' => 8,
    '#description' => t('The maximum whiteboard size a user can upload in terms of characters. Usually 10000 to 1000000. Recommended: 500000.'),
    '#field_suffix' => t('characters'),
  );

  $form['settings_general']['whiteboard_js2d_path'] = array(
    '#type' => 'textfield',
    '#title' => t('jsDraw2D Path'),
    '#default_value' => check_plain(variable_get('whiteboard_js2d_path', 'sites/all/libraries/jsDraw2D/jsDraw2D.js')),
    '#size' => '35',
    '#maxlength' => '100',
    '#description' => t('The Drupal path to the jsDraw2D library including filename. Recommended: sites/all/libraries/jsDraw2D/jsDraw2D.js'),
  );

  $form['#validate'][] = 'whiteboard_admin_settings_validate';

  if (!file_exists($_SERVER['DOCUMENT_ROOT'] . base_path() . '/' . variable_get('whiteboard_js2d_path', NULL))) {
    drupal_set_message('jsDraw2D library not found', 'warning', TRUE);
  }

  return system_settings_form($form);
}


