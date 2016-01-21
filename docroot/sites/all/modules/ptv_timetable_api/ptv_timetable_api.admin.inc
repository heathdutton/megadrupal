<?php
/**
 * @file
 * This file contains PTV timetable API configuration form and functions.
 */

/**
 * PTV timetable settings form.
 */
function ptv_timetable_api_settings_form() {

  $form = array();

  $form['ptv_timetable_api'] = array(
    '#type' => 'fieldset',
    '#title' => 'PTV Timetable API settings',
  );

  $form['ptv_timetable_api']['ptv_timetable_api_developer_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Developer ID'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => variable_get('ptv_timetable_api_developer_id', ''),
  );

  $form['ptv_timetable_api']['ptv_timetable_api_security_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Security Key'),
    '#maxlength' => 128,
    '#required' => TRUE,
    '#default_value' => variable_get('ptv_timetable_api_security_key', ''),
  );

  return system_settings_form($form);
}
