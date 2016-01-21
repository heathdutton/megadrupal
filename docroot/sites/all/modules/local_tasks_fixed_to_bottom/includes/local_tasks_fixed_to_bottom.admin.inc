<?php

/**
 * @file
 * Admin functions for the Local tasks fixed to bottom module.
 */

/**
 * Callback for local_tasks_fixed_to_bottom_menu().
 */
function local_tasks_fixed_to_bottom_admin_settings($form, &$form_state) {
  $form['local_tasks_fixed_to_bottom'] = array(
    '#type' => 'fieldset',
    '#title' => t('Local tasks fixed to bottom configuration'),
  );

  $form['local_tasks_fixed_to_bottom']['local_tasks_fixed_to_bottom_style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#options' => array(
      0 => t('Dark'),
      1 => t('White'),
    ),
    '#default_value' => variable_get('local_tasks_fixed_to_bottom_style', array()),
    '#description' => t('Set this to <em>dark</em> or <em>white</em> according to how it suits better with your theme.'),
  );

  return system_settings_form($form);
}
