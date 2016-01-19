<?php

/**
 * @file
 * Administrative page callbacks for the Keyade Madmetrics module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function keyade_madmetrics_admin_settings_form($form_state) {
  $form = array();

  $form['keyade_madmetrics_pc_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('keyade_madmetrics_pc_id'),
  );

  return system_settings_form($form);
}
