<?php

/**
 * @file
 * Absolute Messages module admin functions.
 */

/**
 * Implements hook_admin_settings().
 */
function notification_bar_settings_config_form() {
  $form = array();

  $form['notification_bar_settings_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load default CSS'),
    '#default_value' => variable_get('notification_bar_settings_css', 1),
    '#description' => t("Uncheck the box, If you don't want to use module's default CSS.")
  );
  $form['notification_bar_settings_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load default JS'),
    '#default_value' => variable_get('notification_bar_settings_js', 1),
    '#description' => t("Uncheck the box, If you don't want to use Javascript way to align the notification at the top of the page.")
  );

  return system_settings_form($form);
}
