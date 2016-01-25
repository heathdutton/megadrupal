<?php

/**
 * @file
 *   Force timestamp module admin functions.
 */

function force_timestamp_settings() {
  $default = variable_get('force_timestamp_varname', 'forced_timestamp');

  $form = array();

  $form['force_timestamp_varname'] = array(
    '#type' => 'textfield',
    '#title' => t('Force timestamp variable name'),
    '#description' => t('Enter the name of the GET variable to be used for setting the timestamp.'),
    '#default_value' => $default,
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
