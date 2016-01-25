<?php

/**
 * @file
 * Administrative page callbacks for the Check Profile-module.
 */

/**
 * @param $form_state
 * @return array
 */
function check_profile_admin_settings_form($form_state) {
  // Profile settings.
  $form['fieldset_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['fieldset_settings']['check_profile_ajax_calls_passthrough'] = array(
    '#type' => 'checkbox',
    '#title' => t('AJAX Passthrough'),
    '#default_value' => variable_get('check_profile_ajax_calls_passthrough', FALSE),
    '#required' => FALSE,
    '#description' => t('Allow AJAX calls to ignore URL checks.'),
  );

  return system_settings_form($form);
}
