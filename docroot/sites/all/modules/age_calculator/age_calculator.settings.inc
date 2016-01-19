<?php

/**
 * @file
 * Display of subscription list & operations.
 */

/**
 * Helper function to get age calculator output options.
 *
 * @return array $options
 *   Returns the configuration options.
 */
function age_calculator_output_options() {
  // Defining empty options array.
  $options = array();
  // Defining output formats inside options array.
  $options['age_calculator_years_months_days'] = t('Y years M months D days');
  $options['age_calculator_months_days'] = t('M months D days');
  $options['age_calculator_weeks_days'] = t('W Weeks D days');
  $options['age_calculator_days'] = t('D days');
  $options['age_calculator_hours'] = t('H hours <b>(Approximate)</b>');
  $options['age_calculator_minutes'] = t('M minutes <b>(Approximate)</b>');
  $options['age_calculator_seconds'] = t('S seconds <b>(Approximate)</b>');
  // Returning options.
  return $options;
}

/**
 * Form constructor for the age calculator output configuration form.
 *
 * @ingroup forms
 */
function age_calculator_output_configuration_form($form, &$form_state) {
  // Defining empty form array.
  $form = array();
  // Calling helper function to get output options.
  $options = age_calculator_output_options();
  // Calling helper function to get already saved output options.
  $default_values = age_calculator_default_output_options();
  // Defining age calculator output form element.
  $form['age_calculator_output'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select age calculator output format'),
    '#description' => t('Please note that some values will be displayed with approximate values & may not be 100% true.'),
    '#options' => $options,
    '#default_value' => count(variable_get('age_calculator_output')) > 0 ? variable_get('age_calculator_output') : $default_values,
    '#required' => TRUE,
    '#weight' => 0,
  );
  // Return system settings form.
  return system_settings_form($form);
}

/**
 * Helper function to return already saved output format.
 */
function age_calculator_default_output_options() {
  // Defining default values definition.
  $default_values = array();
  $default_values['age_calculator_years_months_days'] = 'age_calculator_years_months_days';
  $default_values['age_calculator_months_days'] = 0;
  $default_values['age_calculator_weeks_days'] = 0;
  $default_values['age_calculator_days'] = 0;
  $default_values['age_calculator_hours'] = 0;
  $default_values['age_calculator_minutes'] = 0;
  $default_values['age_calculator_seconds'] = 0;
  // Returning default values.
  return $default_values;
}
