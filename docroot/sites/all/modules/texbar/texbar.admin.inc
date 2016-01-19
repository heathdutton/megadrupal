<?php

/**
 * @file
 * Whiteboard admin file for validating and specifying form texbar data
 */

/**
 * Form API callback to validate the upload settings form.
 */
function texbar_admin_settings_validate($form, &$form_state) {
  $size = $form_state['values']['texbar_size'];

  $path = file_exists($_SERVER['DOCUMENT_ROOT'] . base_path() . '/' . $form_state['values']['texbar_markitup']);

  $path2 = file_exists($_SERVER['DOCUMENT_ROOT'] . base_path() . '/' . $form_state['values']['texbar_output']);

  $path3 = file_exists($form_state['values']['texbar_html2ps']);

  $path4 = file_exists($form_state['values']['texbar_ps2pdf']);

  if (!is_numeric($size) || ($size <= 0)) {
    form_set_error('texbar_size', t('The texbar size limit must be a number and greater than zero.'));
  }
  if (!$path) {
    form_set_error('texbar_markitup', t('markItUp library not found.'));
  }

  if (!$path2) {
    form_set_error('texbar_output', t('Output path not found.'));
  }

  if (!$path3) {
    form_set_error('texbar_html2ps', t('html2ps not found.'));
  }
  if (!$path4) {
    form_set_error('texbar_ps2pdf', t('ps2pdf not found.'));
  }
}

/**
 * Menu callback for the upload settings form.
 */
function texbar_admin_settings($form, &$form_state) {

  $form['settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
  );
  $form['settings_general']['texbar_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of characters'),
    '#default_value' => check_plain(variable_get('texbar_size', 50000)),
    '#size' => 8,
    '#maxlength' => 8,
    '#description' => t('The maximum number of characters a user can convert to PDF.'),
    '#field_suffix' => t('characters'),
  );

  $form['settings_general']['texbar_html2ps'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to html2ps'),
    '#default_value' => check_plain(variable_get('texbar_html2ps', '/usr/bin/html2ps')),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('Absolute path to the binary html2ps.'),
    '#field_suffix' => t(''),
  );

  $form['settings_general']['texbar_ps2pdf'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to ps2pdf'),
    '#default_value' => check_plain(variable_get('texbar_ps2pdf', '/usr/bin/ps2pdf')),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('Absolute path to the binary ps2pdf'),
    '#field_suffix' => t(''),
  );

  $form['settings_general']['texbar_output'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to save output'),
    '#default_value' => check_plain(variable_get('texbar_output', '')),
    '#size' => 50,
    '#maxlength' => 50,
    '#description' => t('The Drupal relative path to save output to(must be accessible from the web.)'),
    '#field_suffix' => t(''),
  );

  $form['settings_general']['texbar_markitup'] = array(
    '#type' => 'textfield',
    '#title' => t('markItUp path'),
    '#default_value' => check_plain(variable_get('texbar_markitup', 'sites/all/libraries/markitup/markitup')),
    '#size' => '50',
    '#maxlength' => '150',
    '#description' => t('The Drupal relative path to the markItUp library including filename. Recommended setting is prepopulated.'),
  );

  $form['settings_general']['texbar_mimetex'] = array(
    '#type' => 'textfield',
    '#title' => t('mimeTex URL path'),
    '#default_value' => check_plain(variable_get('texbar_mimetex', 'http://www.example.com/cgi-bin/mimetex.cgi')),
    '#size' => '50',
    '#maxlength' => '150',
    '#description' => t('Full URL path to mimetex CGI script.'),
  );

  $form['settings_general']['texbar_tag'] = array(
    '#type' => 'textfield',
    '#title' => t('Textarea Tag'),
    '#default_value' => check_plain(variable_get('texbar_tag', 'texbar')),
    '#size' => '25',
    '#maxlength' => '25',
    '#description' => t('Including this tag in the textarea class will load LaTeX Toolbar.'),
  );

  $form['#validate'][] = 'texbar_admin_settings_validate';

  return system_settings_form($form);
}

