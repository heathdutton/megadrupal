<?php

/**
 * @file
 * Administration forms for the User Scheduled Delete module.
 */

/**
 * Form page callback for module settings form.
 */
function user_scheduled_delete_settings_form($form, &$form_state) {
  $days = array(1 => 1, 5 => 5, 7 => 7, 15 => 15, 30 => 30, 45 => 45);
  $form['user_scheduled_delete_delay_in_days'] = array(
    '#type' => 'select',
    '#title' => t('Days to Expiration'),
    '#description' => t('The number of days in the future to delete the account.'),
    '#options' => $days,
    '#default_value' => variable_get('user_scheduled_delete_delay_in_days', 15),
  );

  $batch_sizes = array(15 => 15, 50 => 50, 100 => 100, 200 => 200, 500 => 500);
  $form['user_scheduled_delete_batch_size'] = array(
    '#type' => 'select',
    '#title' => t('Accounts to process per run'),
    '#description' => t('The number of accounts to delete from the queue each time the queue is processed.'),
    '#options' => $batch_sizes,
    '#default_value' => variable_get('user_scheduled_delete_batch_size', 50),
  );

  $form['user_scheduled_delete_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Process queue on cron run'),
    '#description' => t('Larger sites should consider setting up a seperate cron process to call <em>drush asd</em> directly and not have the queue processed during cron runs.'),
    '#default_value' => variable_get('user_scheduled_delete_cron', 1),
  );

  return $form;
}
