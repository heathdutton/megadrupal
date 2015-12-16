<?php

/**
 * @file
 *   Webform Mass Email administration page.
 *
 * @ingroup webform_mass_email
 */

/**
 * Callback for admin/config/webform_mass_email menu item.
 */
function webform_mass_email_settings() {
  $form = array();

  $throttle = drupal_map_assoc(array(1, 10, 20, 30, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000));
  $interval = drupal_map_assoc(array(0, 60, 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 76800, 153600, 307200));
  $throttle[0] = t('Unlimited');

  $form['webform_mass_email_throttle'] = array(
    '#type' => 'select',
    '#title' => t('Cron throttle'),
    '#options' => $throttle,
    '#default_value' => variable_get('webform_mass_email_throttle', 20),
    '#description' => t('Sets the numbers of messages sent per cron run. Failure to send will also be counted. Cron execution must not exceed the PHP maximum execution time of %max seconds. You find the time spend to send emails in the !recent_logs.', array('%max' => ini_get('max_execution_time'), '!recent_logs' => l(t('Recent log entries'), 'admin/reports/dblog'))),
  );
  $form['webform_mass_email_queue_expire'] = array(
    '#type' => 'select',
    '#title' => t('Mail expiration'),
    '#options' => $interval,
    '#default_value' => variable_get('webform_mass_email_queue_expire', 600),
    '#description' => t('E-mails are stored in the Drupal queue table. How long (in seconds) must messages be retained in the queue if the sending failed.'),
  );
  $form['webform_mass_email_log'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log emails'),
    '#default_value' => variable_get('webform_mass_email_log', FALSE),
    '#description' => t('When checked all outgoing mesages are logged in the system log. A logged E-mail does not guarantee that it is send or will be delivered. It only indicates that a message is send to the PHP mail() function.'),
  );

  return system_settings_form($form);
}
