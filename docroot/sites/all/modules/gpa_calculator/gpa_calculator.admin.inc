<?php

/**
 * @file
 * GPA Calculator admin settings.
 */

/**
 * GPA Calculator settings form.
 */
function gpa_calculator_settings_form($form, &$form_state) {
  global $user;
  $form = array();

  $form['gpa_calculator'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for GPA Calculator'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['gpa_calculator']['gpa_calculator_school_name'] = array(
    '#type' => 'textfield',
    '#title' => t('School'),
    '#description' => t('Enter you school\'s name here. If blank, block subject will read as "GPA Calculator."'),
    '#default_value' => variable_get('gpa_calculator_school_name', ''),
  );

  // Set form element to default text format if logged in user has one,
  // otherwise make it a regular textarea.
  $user_filter_formats = filter_formats($user);
  if (!empty($user_filter_formats)) {
    $text_format = filter_default_format();
    $fapi_type = 'text_format';
    $format = $text_format;
  }
  else {
    $fapi_type = 'textarea';
    $format = '';
  }

  $gpa_calc_instructions = variable_get('gpa_calculator_instructions', '');
  if (isset($gpa_calc_instructions['format'])) {
    $gpa_instructions_value = variable_get('gpa_calculator_instructions', '');
    $gpa_instructions = $gpa_instructions_value['value'];
  }
  else {
    $gpa_instructions = variable_get('gpa_calculator_instructions', '');
  }

  $form['gpa_calculator']['gpa_calculator_instructions'] = array(
    '#type' => $fapi_type,
    '#title' => t('Instructions'),
    '#description' => t('Provide instructions or a description for your GPA calculator.'),
    '#default_value' => $gpa_instructions,
    '#format' => $format,
  );

  $form['gpa_calculator']['gpa_calculator_grades'] = array(
    '#type' => 'textarea',
    '#title' => t('Grades'),
    '#description' => t('Enter grade options for the select box values.  Key-value pairs must be entered separated by pipes. i.e. safe_key|Some readable option on separate lines.  If blank, default values will be: 4.0|A 3.67|A- 3.33|B+ 3.0|B 2.67|C+ 2.33|C 2.0|C- 1.67|D+ 1.33|D 1.0|D- 0.0|F'),
    '#default_value' => variable_get('gpa_calculator_grades', ''),
  );

  $form['gpa_calculator']['gpa_calculator_prev_gpa_instructions'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Cumulative GPA Instructions'),
    '#description' => t('Enter instructions to display as the description under "Previous Cumulative GPA" on the GPA Calculator.'),
    '#maxlength' => 255,
    '#default_value' => variable_get('gpa_calculator_prev_gpa_instructions', ''),
  );

  $form['gpa_calculator']['gpa_calculator_prev_credits_instructions'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Cumulative Credits Earned Instructions'),
    '#description' => t('Enter instructions to display as the description under "Previous Cumulative Credits Earned" on the GPA Calculator.'),
    '#maxlength' => 255,
    '#default_value' => variable_get('gpa_calculator_prev_credits_instructions', ''),
  );

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'gpa_calculator') . '/css/gpa_calculator.admin.css',
  );

  $form['#attached']['js'] = array(
    array(
      'data' => drupal_get_path('module', 'gpa_calculator') . '/js/gpa_calculator.admin.js',
      'scope' => 'footer',
    ),
  );

  return system_settings_form($form);
}
