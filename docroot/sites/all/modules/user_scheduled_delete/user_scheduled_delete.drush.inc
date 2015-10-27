<?php

/**
 * Drush integration for User Scheduled Delete module.
 */

/**
 * Implements hook_drush_command().
 *
 * @return
 *   An associative array describing your command(s).
 */
function user_scheduled_delete_drush_command() {
  return array(
    'process-user-scheduled-delete-queue' => array(
      'description' => dt('Puts your site in dev mode.'),
      'aliases' => array('usd'),
    ),
    'schedule-user-delete' => array(
      'description' => dt('Queue a user for deletion.'),
      'aliases' => array('sud'),
      'arguments' => array(
        'uid_or_username' => dt('Account username or user id.'),
      ),
    ),
  );
}

/**
 * Drush command callback for processing the queue.
 * 
 * For busier sites set the drush command up as it's own cron task and disable
 * cron process by setting user_scheduled_delete_cron to 0.
 */
function drush_user_scheduled_delete_process_user_scheduled_delete_queue() {
  module_load_include('inc', 'user_scheduled_delete');
  user_scheduled_delete_process_delete_queue();
}

/**
 * Drush command callback for add a user account to the deletion queue.
 */
function drush_user_scheduled_delete_schedule_user_delete($uid_or_username) {
  module_load_include('inc', 'user_scheduled_delete');
  if (!$uid_or_username) {
    drush_log(dt('You must provide either a uid or a username.'), 'error');
    return;
  }
  if (!($account = user_load($uid_or_username))) {
    drush_log(dt('User @user not found.', array('@user' => $uid_or_username)), 'error');
    return;
  }

  if (user_scheduled_delete_schedule($account)) {
    drush_log(dt('User @user queued for deletion.', array(
      '@user' => $account->name,
    )), 'ok');
  }
  else {
    drush_log(dt('User @user could not be queued for deletion.', array(
      '@user' => $account->name,
    )), 'error');
  }

  return;
}

