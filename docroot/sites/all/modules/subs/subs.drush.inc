<?php
/**
 * @file
 * Subs Drush integration.
 */

/**
 * Implements hook_drush_command().
 */
function subs_drush_command() {
  return array(
    // Utility commands.
    'subs-cron' => array(
      'description' => 'Execute subs cron script.',
    ),
    'subs-load' => array(
      'description' => 'Load and view a subscription',
      'aliases' => array('subs-view'),
      'arguments' => array(
        'id' => "The subscription's id.",
        'full' => "true/false. Whether to load the full subscription object. Defaults to false.",
      ),
    ),
    'subs-delete' => array(
      'description' => 'Delete a subscription',
      'aliases' => array('subs-del'),
      'arguments' => array(
        'id' => "The subscription's id.",
      ),
    ),
    'subs-status' => array(
      'description' => "Returns a subscription's status",
      'arguments' => array(
        'id' => "The subscription's id.",
      ),
    ),
    // Change subscription status commands.
    'subs-pending' => array(
      'description' => "Change a subscription's status to pending",
      'arguments' => array(
        'id' => "The subscription's id.",
      ),
    ),
    'subs-active' => array(
      'description' => "Change a subscription's status to active",
      'arguments' => array(
        'id' => "The subscription's id.",
      ),
    ),
    'subs-grace' => array(
      'description' => "Change a subscription's status to grace period",
      'arguments' => array(
        'id' => "The subscription's id.",
        'extend' => "true/false. Whether to extend the subscription's end date by the grace period length. Defaults to true.",
      ),
    ),
    'subs-expire' => array(
      'description' => "Change a subscription's status to expired",
      'arguments' => array(
        'id' => "The subscription's id.",
      ),
    ),
    'subs-cancel' => array(
      'description' => "Change a subscription's status to cancelled",
      'arguments' => array(
        'id' => "The subscription's id.",
      ),
    ),
  );
}

/**
 * Implements hook_drush_help().
 */
function subs_drush_help($section) {
  switch ($section) {
    case 'drush:subs-cron':
      return dt("Execute subs cron script. Use this to execute the subs cron script independently from Drupal's cron.");
    case 'drush:subs-load':
      return dt('Load and view a subscription.');
    case 'drush:subs-delete':
      return dt('Delete a subscription.');
    case 'drush:subs-status':
      return dt("Returns a subscription's status.");
    case 'drush:subs-pending':
      return dt("Change a subscription's status to pending.");
    case 'drush:subs-active':
      return dt("Change a subscription's status to active.");
    case 'drush:subs-grace':
      return dt("Change a subscription's status to grace period.");
    case 'drush:subs-expire':
      return dt("Change a subscription's status to expired.");
    case 'drush:subs-cancel':
      return dt("Change a subscription's status to cancelled.");
  }
}

########################
### Utility Commands ###
########################

/**
 * Drush command.
 * Execute subscription cron script.
 *
 * @see subs_drush_command()
 */
function drush_subs_cron() {
  subs_cron();
  drush_print(dt('Subs cron executed!'));
}

/**
 * Drush command.
 * Load and view a subscription.
 *
 * @param int
 *   The subscription's id.
 * @param boolean
 *   Whether to load the full subscription object.
 */
function drush_subs_load($id, $full = FALSE) {
  if ($subscription = subs_load($id)) {
    if (!$full) {
      $dummy = new stdClass;

      foreach ($subscription as $property => $value) {
        $dummy->$property = $value;
      }

      $subscription = $dummy;
    }
    drush_print(dt('Subscription loaded successfully!') . "\n");
    print_r($subscription);
    drush_print("\n");
  }
  else {
    drush_print(dt('Failed to load the subscription. Invalid subscription id.'));
  }
}

/**
 * Drush command.
 * Delete a subscription.
 *
 * @param int
 *   The subscription's id.
 */
function drush_subs_delete($id) {
  if ($subscription = subs_load($id)) {
    if (drush_confirm(dt('Are you sure you want to delete this subscription?'))) {
      subs_delete($subscription);
      drush_print(dt('Subscription deleted successfully!'));
    }
  }
  else {
    drush_print(dt('Failed to load the subscription. Invalid subscription id.'));
  }
}

/**
 * Drush command.
 * Return a subscription's status.
 *
 * @param int
 *   The subscription's id.
 */
function drush_subs_status($id) {
  if ($subscription = subs_load($id)) {
    $status_options = subs_status_options_list();
    $status = $status_options[$subscription->status];
    drush_print(dt("The subscription's status is: !status", array('!status' => $status)));
  }
  else {
    drush_print(dt('Failed to load the subscription. Invalid subscription id.'));
  }
}

##############################
### Change Status Commands ###
##############################

/**
 * Drush command.
 * Make a subscription pending.
 *
 * @param int
 *   The subscription's id.
 */
function drush_subs_pending($id) {
  _drush_subs_status($id, 'subs_set_pending');
}

/**
 * Drush command.
 * Activate a subscription.
 *
 * @param int
 *   The subscription's id.
 */
function drush_subs_active($id) {
  _drush_subs_status($id, 'subs_set_active');
}

/**
 * Drush command.
 * Change a subscription to grace period.
 *
 * @param int
 *   The subscription's id.
 * @param boolean
 *   Whether to extend the subscription's end date by the grace period length.
 */
function drush_subs_grace($id, $extend = TRUE) {
  _drush_subs_status($id, 'subs_set_grace', (bool) $extend);
}

/**
 * Drush command.
 * Expire a subscription.
 *
 * @param int
 *   The subscription's id.
 */
function drush_subs_expire($id) {
  _drush_subs_status($id, 'subs_set_expired');
}

/**
 * Drush command.
 * Cancel a subscription.
 *
 * @param int
 *   The subscription's id.
 */
function drush_subs_cancel($id) {
  _drush_subs_status($id, 'subs_set_cancelled');
}

/**
 * Helper function.
 * Executes status operations on subscriptions.
 *
 * @param int
 *   The subscription's id.
 * @param string
 *   The operation $callback we want to execute.
 * @param mixed
 *   Any extra parameter to pass to the callback.
 */
function _drush_subs_status($id, $callback, $extra = NULL) {
  if ($subscription = subs_load($id)) {
    // Confirm action.
    if (drush_confirm(dt("Are you sure you want to change this subscription's status?"))) {
      // Try to execute operation callback.
      if ($callback($subscription, $extra)) {
        drush_print(dt('Subscription status changed!'));
      }
      else {
        drush_print(dt('Failed to change subscription status.'));
      }
    }
  }
  else {
    drush_print(dt('Failed to load the subscription. Invalid subscription id.'));
  }
}