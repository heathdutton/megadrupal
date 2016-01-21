<?php

/**
 * @file
 * Form functions for User Scheduled Delete module.
 */



/**
 * Page callback form for rejecting a user.
 */
function user_scheduled_delete_schedule_form($form, &$form_state, $account) {
  $delay_in_days = variable_get('user_scheduled_delete_delay_in_days', 15);

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $account->uid,
  );

  $form['confirmation'] = array(
    '#markup' => t('Are you sure you want to schedule @username for deletion in @days?', array(
      '@username' => $account->name,
      '@days' => format_plural($delay_in_days, '@count day', '@count days'),
    )),
    '#prefix' => '<p>',
    '#suffix' => '</p>',
  );

  $form['submit'] = array(
    '#value' => t('Schedule'),
    '#type' => 'submit',
  );

  $form['cancel'] = array(
    '#value' => t('Cancel'),
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Submit handler that schedules a user for deletion and triggers an event
 * for sending an email via Rules.
 */
function user_scheduled_delete_schedule_form_submit($form, &$form_state) {
  global $user;
  module_load_include('inc', 'user_scheduled_delete');
  $account = user_load($form_state['values']['uid']);

  // If they clicked cancel, don't reject them.
  if (isset($form_state['clicked_button']['#value']) && $form_state['clicked_button']['#value'] == 'Cancel') {
    drupal_goto('user/' . $account->uid);
  }

  user_scheduled_delete_schedule($account);

  $delay_in_days = variable_get('user_scheduled_delete_delay_in_days', 15);
  drupal_set_message(t('User @username scheduled for deletion in @days.', array(
    '@username' => $account->name,
    '@days' => format_plural($delay_in_days, '@count day', '@count days'),
  )));

  // Since $form_state['redirect'] takes precedence.
  if (!isset($_GET['destination'])) {
    $form_state['redirect'] = 'user/' . $account->uid;
  }
}
