<?php
/**
 * @file
 * Batch functions.
 */

/**
 * Batch callback.
 */
function chargebee_batch_subscriptions_verify_callback($uid, &$context) {
  $user = user_load($uid);
  $message = 'Attempt to verify the subscription for non-existent user';
  if ($user) {
    $result = chargebee_verify_subscription_status($user);
    if (!$result) {
      $context['results'][] = $uid;
    }
    $message = $user->name;
  }
  else {
    watchdog('ChargeBee Verify', $message, array(), WATCHDOG_WARNING);
  }
  $context['message'] = $message;
}

/**
 * Batch finished callback.
 */
function chargebee_batch_subscriptions_verify_finished($success, $results, $operations) {
  $message = t('No users with errors.');
  $type = 'status';
  if ($success && $results) {
    $message = format_plural(count($results), 'One user with discrepancy.', '@count users with discrepancy.');
    $type = 'warning';
  }
  drupal_set_message($message, $type);
}
