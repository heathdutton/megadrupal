<?php

/**
 * @file
 * Corus schedule
 *
 * contains all forms and saving function for the schedule UI
 */

/**
 * Defines a settings form.
 */
function human_settings($form) {
  $form['human_form'] = array(
    '#type' => 'textarea',
    '#title' => t('Setup forms that will be checked by human module.'),
    '#default_value' => variable_get('human_form', 'user_register_form, user_login_form, user_pass, user_login'),
    '#description' => t('Enter the form ids for human module, seperate each form id with ",".'),
  );
  return system_settings_form($form);
}
