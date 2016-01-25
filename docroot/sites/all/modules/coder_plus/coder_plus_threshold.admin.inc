<?php

/**
 * @file
 * This file holds functions for handling coder_plus Admin threshold form.
 *
 * @SNDCP coder_plus
 */

/**
 * Implements hook_form().
 */
function coder_plus_admin_settings_threshold_form($node, &$form_state) {
  $error_type = array(
    'critical' => t('Critical'),
    'major' => t('Major'),
    'minor' => t('Minor'));

  $base = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#maxlength' => 10,
    '#required' => TRUE,
  );

  $form['error_type'] = array(
    '#type' => 'fieldset',
    '#title' => t('Threshold Values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  foreach ($error_type as $key => $value) {
    $description = ($key == 'critical') ?
    array('#description' => t('Please Specify Range for complexity. Any thing Above this range will be considered as blocker')) : array(
      '#description' => t('Please Specify Range for complexity'));

    $form['error_type'][$value] = array(
      '#type' => 'fieldset',
      '#title' => t('%title', array('%title' => $value)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    ) + $description;

    $form['error_type'][$value][$key . '_from'] = array(
      '#title' => t('From'),
      '#default_value' => variable_get($key . '_from', ''),
      '#prefix' => '<div class="field-values-wrapper clearfix">',
    ) + $base;

    $form['error_type'][$value]['error_type'][$key . '_to'] = array(
      '#title' => t('To'),
      '#default_value' => variable_get($key . '_to', ''),
      '#suffix' => '</div>',
    ) + $base;
  }

  $form['error_type']['actions'] = array(
    '#type' => 'actions',
    '#weight' => 300,
  );

  $form['error_type']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Values'),
  );

  return $form;
}

/**
 * Implements hook_form_validate().
 */
function coder_plus_admin_settings_threshold_form_validate($form, &$form_state) {
  $error_type = array(
    'critical' => t('Critical'),
    'major' => t('Major'),
    'minor' => t('Minor'));

  coder_plus_check_threshold_field_numeric($error_type, $form, $form_state);
  coder_plus_check_threshold_field_range($error_type, $form, $form_state);
  coder_plus_threshold_boundary_check($error_type, $form, $form_state);
  coder_plus_threshold_difference_check($error_type, $form, $form_state);
}

/**
 * Function to check numeric value for threshold form.
 *
 * @param array $error_type
 *   Error type array.
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function coder_plus_check_threshold_field_numeric($error_type, $form, $form_state) {
  foreach ($error_type as $key => $value) {
    if (!is_numeric($form_state['values'][$key . '_from'])) {
      $message = t('Only numbers are allowed in %field.', array('%field' => $key . ' from'));
      form_set_error($key . '_from', $message);
    };

    if (!is_numeric($form_state['values'][$key . '_to'])) {
      $message = t('Only numbers are allowed in %field.', array('%field' => $key . ' to'));
      form_set_error($key . '_to', $message);
    }
  }
}

/**
 * Function to check threshold field range.
 *
 * @param array $error_type
 *   Error type array.
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function coder_plus_check_threshold_field_range($error_type, $form, $form_state) {
  foreach ($error_type as $key => $value) {
    if ($form_state['values'][$key . '_from'] !== '' &&
            $form_state['values'][$key . '_to'] === '') {
      $message = t('%name: you must specify two values.', array('%name' => $key . '_to'));
      form_set_error($key . '_to', $message);
    }
    elseif ($form_state['values'][$key . '_from'] === '' &&
            $form_state['values'][$key . '_to'] !== '') {
      $message = t('%name: you must specify two values.', array('%name' => $key . '_from'));
      form_set_error($key . '_from', $message);
    }
    elseif ($form_state['values'][$key . '_from'] != '' &&
            $form_state['values'][$key . '_to'] != '') {
      if ($form_state['values'][$key . '_from'] > $form_state['values'][$key . '_to']) {
        $message = t('%name: FROM value is greater than TO value.', array('%name' => $key . '_from'));
        form_set_error($key . '_from', $message);
      }
    }
  }
}

/**
 * Implements hook_form_submit().
 */
function coder_plus_admin_settings_threshold_form_submit($form, &$form_state) {
  $error_type = array(
    'critical' => t('Critical'),
    'major' => t('Major'),
    'minor' => t('Minor'));

  foreach ($error_type as $key => $value) {
    variable_set($key . '_from', $form_state['values'][$key . '_from']);
    variable_set($key . '_to', $form_state['values'][$key . '_to']);
  }

  drupal_set_message(t('Threshold value submitted successfully.'));
}

/**
 * Function to check the boundary values.
 *
 * @param array $error_type
 *   Error type array.
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function coder_plus_threshold_boundary_check($error_type, $form, $form_state) {
  if ($form_state['values']['minor_to'] > $form_state['values']['major_from']) {
    $message = t('Minor To is greater than Major From.');
    form_set_error('minor_to', $message);
  }

  if ($form_state['values']['major_from'] == $form_state['values']['minor_to']) {
    $message = t('Major From and Minor To cannot have same values, difference of 1 is required.');
    form_set_error('major_from', $message);
  }

  if ($form_state['values']['major_to'] > $form_state['values']['critical_from']) {
    $message = t('Major To is greater than Critical From.');
    form_set_error('major_to', $message);
  }

  if ($form_state['values']['critical_from'] == $form_state['values']['major_to']) {
    $message = t('Critical From and Major To cannot have same values,difference of 1 is required.');
    form_set_error('critical_from', $message);
  }
}

/**
 * Function to check the difference.
 *
 * @param array $error_type
 *   Error type array.
 * @param array $form
 *   Form values in array format.
 * @param array $form_state
 *   Form State in array format.
 */
function coder_plus_threshold_difference_check($error_type, $form, $form_state) {
  if (($form_state['values']['critical_from'] - $form_state['values']['major_to']) > 1) {
    $message = t('Difference between Critical From and Major To should not be more than 1.');
    form_set_error('critical_from', $message);
  }

  if (($form_state['values']['major_from'] - $form_state['values']['minor_to']) > 1) {
    $message = t('Difference between Major From and Minor To should not be more than 1.');
    form_set_error('major_from', $message);
  }
}
