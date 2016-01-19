<?php
/**
 * @file
 * Instance settings admin page callbacks.
 */

/**
 * Administration settings form.
 */
function instance_settings_admin_settings($form, &$form_state) {
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['general']['instance_settings_apply_on_page_load'] = array(
    '#type' => 'checkbox',
    '#title' => t('Apply on page load'),
    '#description' => t('Applies instance settings on each page load. It should be switched off on production instances. You can use drush command to apply settings: drush instance-settings-apply or drush isa.'),
    '#default_value' => variable_get('instance_settings_apply_on_page_load', 0),
  );

  $form['general']['instance_settings_verbose_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose mode'),
    '#description' => t('Defines if detailed messages should be showed during settings apply.'),
    '#default_value' => variable_get('instance_settings_verbose_mode', 0),
  );

  return system_settings_form($form);
}
