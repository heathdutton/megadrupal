<?php

/**
 * @file
 * Administration callbacks for the role_activity module.
 */

/**
 * Administrator settings page.
 */
function role_activity_admin_settings($form, &$form_state) {
  $form['role_activity'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Track activity for these roles'),
    '#options' => user_roles(TRUE),
    '#default_value' => variable_get('role_activity', array()),
  );

  return system_settings_form($form);
}
