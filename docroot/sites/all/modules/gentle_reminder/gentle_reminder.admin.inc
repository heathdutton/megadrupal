<?php

/**
 * @file
 * Admin settings.
 */


/**
 * Reminder settings.
 */
function gentle_reminder_admin_settings() {
  $form = array();

  $form['gentle_reminder_dev_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Development mode'),
    '#description' => t('Will not sent notifications by cron.'),
    '#default_value' => variable_get('gentle_reminder_dev_mode', FALSE),
  );

  $form['dev_mode_note'] = array(
    '#markup' => t("You may also want to add %code to your %file file.", array(
      '%code' => '$conf[\'gentle_reminder_dev_mode\'] = TRUE;',
      '%file' => 'settings.php',
    )),
  );

  return system_settings_form($form);
}
