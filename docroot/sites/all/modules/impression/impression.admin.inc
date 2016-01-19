<?php

/**
 * @file
 * Admin the setting of Impression module
 *
 * contains all the setting for impression admin function.
 */

/**
 * Defines a settings form.
 */
function impression_admin_settings($form) {
  $form['visibility'] = array(
    '#type' => 'vertical_tabs',
  );
  $form['visibility']['purge'] = array(
    '#type' => 'fieldset',
    '#title' => t('Purge'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'visibility',
    '#weight' => 2,
  );
  $form['visibility']['purge']['impression_purge'] = array(
    '#type' => 'textfield',
    '#title' => t('How many days to keep the impression data?'),
    '#maxlength' => 16,
    '#default_value' => variable_get('impression_purge', 0),
    '#description' => t('Do not want to purge? Leave this field to 0. Use number only, no space and other characters.'),
  );
  $role_options = array_map('check_plain', user_roles());
  $form['visibility']['role'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'visibility',
    '#weight' => 3,
  );
  $form['visibility']['role']['impression_active_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Collect actions for specific roles'),
    '#default_value' => variable_get('impression_active_roles', array(1 => '1', 2 => '2', 3 => 0)),
    '#options' => $role_options,
    '#description' => t('Collection Action only for the selected role(s). If no role is selected, no action will be collected by the Impression module.
'),
  );
  $form['visibility']['role']['impression_execlude_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Revert'),
    '#default_value' => variable_get('impression_execlude_roles', FALSE),
    '#description' => t('Do not record the roles selected above.'),
  );
  return system_settings_form($form);
}
