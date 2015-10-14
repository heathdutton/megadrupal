<?php

/**
 * @file
 * Helper functions for the User Scheduled Delete module.
 */

/**
 * Queue an account for deletion after a scheduled time.
 * 
 * @param $account
 *  A fully-loaded Drupal user object or a uid.
 * 
 * @param $after
 *   The date after which the account should be deleted. Unix timestamp format.
 *   Defaults to two weeks from today. Cron must be running on your site for
 *   the account to actually be deleted.
 */
function user_scheduled_delete_schedule($account, $after = NULL) {
  global $user;

  // Load up the user account if they gave us a uid.
  $account = is_object($account) ? $account : user_load($account);
  // Default to scheduling the deletion for two weeks in the future.
  if (!$after) {
    $delay_in_days = variable_get('user_scheduled_delete_delay_in_days', 15);
    $after = strtotime("+{$delay_in_days} day");
  }

  // Queue for deletion.
  db_query('INSERT INTO {user_scheduled_delete} (uid, after) VALUES (:uid, :after) ON DUPLICATE KEY UPDATE after = :after', array(
    ':uid' => $account->uid,
    ':after' => $after,
  ));

  // Trigger any Rules set to fire
  rules_invoke_event('user_scheduled_delete_delete_scheduled', $account, $user, $after);

  $message = 'User account @username scheduled for deletion on @time by @deleter.';
  $variables = array(
    '@username' => $account->name,
    '@time' => format_date($after),
    '@deleter' => $user->name,
  );
  watchdog('user_scheduled_delete', $message, $variables);
  return TRUE;
}

/**
 * Delete a batch of user accounts queued for deletion.
 *
 * This routine is normally called by cron, but you can call it from drush too.
 * You can manage how many users are cleared per batch process by setting the
 * variable user_scheduled_delete_batch_size.
 */
function user_scheduled_delete_process_delete_queue() {
  $batch_size = (int) variable_get('user_scheduled_delete_batch_size', 50);

  // Process up to the set max users and delete them. 
  $results = db_query_range('SELECT uid FROM {user_scheduled_delete} WHERE after < :now', 0, $batch_size, array(
    ':now' => time(),
  ));
  $count = 0;
  foreach ($results as $result) {
    if (!($account = user_load($result->uid))) {
      continue;
    }

    user_delete($account->uid);

    // Since user_delete() doesn't return, we can't check if this worked.
    $message = 'User %user deleted.';
    $variables = array('%user' => $account->name);
    watchdog('user_scheduled_delete', $message, $variables);

    // Trigger any Rules set to fire.
    rules_invoke_event('user_scheduled_delete_user_deleted', $account);

    // Clear from queue processing.
    db_query('DELETE FROM {user_scheduled_delete} WHERE uid = :uid', array(':uid' => $account->uid));

    $count++;
  }

  // Only report on deletion count if we actually deleted anyone.
  if ($count) {
    watchdog('user_scheduled_delete', 'Deleted @count users.', array('@count' => $count));
  }
}
