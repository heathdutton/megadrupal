<?php

/**
 * @file
 * Provides function for form building for google_map_field_apikey.
 */

/**
 *  Form builder for google_map_field_apikey settings page.
 */
function google_map_field_settings_form($form, &$form_state) {
  $form = array();

  $form['google_map_field_apikey'] = array(
    '#title' => t('Google Maps API key'),
    '#type' => 'textfield',
    '#description' => t('Enter your Google Maps API v3 key in this box.'),
    '#default_value' => variable_get('google_map_field_apikey', ''),
  );

  $form['google_map_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Map settings'),
  );

  // Require must be because http://stackoverflow.com/a/8664298/473390
  require module_load_include('inc', 'google_map_field');

  foreach($settings as $type => $data) {
    $form['google_map_field']['google_map_field_'.$type] = array(
      '#type' => 'checkbox',
      '#title' => t($type),
      '#description' => t($data['description']),
      '#default_value' => variable_get('google_map_field_'. $type, $data['default']),
    );
  }

  return system_settings_form($form);
}
