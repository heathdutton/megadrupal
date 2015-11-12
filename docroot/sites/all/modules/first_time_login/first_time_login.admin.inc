<?php

/**
 * @file
 * Admin configuration file of First time login module.
 */

/**
 * Implements first_time_login_form().
 */
function first_time_login_form($form, &$form_state) {
  $form['first_time_login_config_days'] = array(
    '#type' => 'textfield',
    '#element_validate' => array('element_validate_integer_positive'),
    '#title' => t('Enter the number of days.'),
    '#required' => TRUE,
    '#default_value' => variable_get('first_time_login_config_days'),
    '#description' => t('Numeric field - enter number of days.
      For example: Enter 60 for 60 days / 2 Months. </br>
      User who has not updated their account since the above mentioned days,
      will be prompted to updated their account after login.'),
  );
  return system_settings_form($form);
}
