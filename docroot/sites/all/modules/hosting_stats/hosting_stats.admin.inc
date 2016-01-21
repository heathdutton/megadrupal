<?php

/**
 * @file hosting_stats.admin.inc
 * Admin settings functions
 */

/**
 * Provide admin settings form
 */
function hosting_stats_admin_settings($form, &$form_state) {
  $form = array();
  $form['hosting_stats_logging'] = array(
    '#type' => 'radios',
    '#title' => t('Capture performance data'),
    '#default_value' => variable_get('hosting_stats_logging', 0),
    '#options' => array(
      1 => t('On'),
      0 => t('Off'),
    ),
    '#description' => t('When <em>on</em>, Drupal page requests by anonymous and authenicated users will be tracked.'),
  );
  $form['hosting_stats_row_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Performance log limit'),
    '#default_value' => variable_get('hosting_stats_row_limit', 1000000),
    '#validate' => array('hosting_stats_admin_settings_validate'),
    '#description' => t('Number of rows to retain. Older rows will be deleted by the cron process. Note that 1 million rows is approximately equal to about 66MB on disk, so choose this number according to your available disk and expected traffic levels during the test.'),
  );
  $form['hosting_stats_mailto'] = array(
    '#type' => 'textfield',
    '#title' => t('Report destination'),
    '#default_value' => variable_get('hosting_stats_mailto', 'presales@acquia.com'),
    '#validate' => array('hosting_stats_admin_settings_validate'),
    '#description' => t('E-mail address that you wish to send reports to. Reports are sent manually.'),
  );
  return system_settings_form($form);
}

/**
 * Validate email address.
 */
function hosting_stats_admin_settings_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['hosting_stats_mailto'])) {
    form_set_error('hosting_stats_mailto', t('The e-mail address you have entered does not appear valid.'));
  }
  if (is_numeric($form_state['values']['hosting_stats_row_limit']) === TRUE) {
    if ((int) $form_state['values']['hosting_stats_row_limit'] != $form_state['values']['hosting_stats_row_limit']) {
      form_set_error('hosting_stats_row_limit', t('You must specifiy an integer value for the log limit.'));
    }
  }
  else {
    form_set_error('hosting_stats_row_limit', t('You must specifiy an integer value for the log limit.'));
  }
}
