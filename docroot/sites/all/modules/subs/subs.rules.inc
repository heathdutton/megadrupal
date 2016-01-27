<?php
/**
 * @file
 * Subs Rules integration.
 */

/**
 * Access callback.
 *
 * @see subs_rules_event_info()
 * @see subs_rules_action_info()
 */
function subs_rules_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return user_access('administer subs');
  }
}

/**
 * Implements hook_rules_event_info().
 *
 * Define rules events.
 */
function subs_rules_event_info() {
  $events = array();

  // For convenience.
  $defaults = array(
    'group' => t('Subscription'),
    'access callback' => 'subs_rules_access',
    'variables' => array(
      'subs' => array(
        'label' => t('Subscription object'),
        'type' => 'subs',
        'skip save' => TRUE,
      ),
    ),
  );

  // Subscription is pending.
  $events['subs_pending'] = array(
    'label' => t('Subscription is pending.'),
    ) + $defaults;

  // Subscription is activated.
  $events['subs_activate'] = array(
    'label' => t('Subscription is activated.'),
    ) + $defaults;

  // Subscription is renewed.
  $events['subs_renew'] = array(
    'label' => t('Subscription is renewed.'),
    ) + $defaults;

  // Subscription enters grace period.
  $events['subs_grace'] = array(
    'label' => t('Subscription enters grace period.'),
    ) + $defaults;

  // Subscription expires.
  $events['subs_expire'] = array(
    'label' => t('Subscription expires.'),
    ) + $defaults;

  // Subscription expires.
  $events['subs_cancel'] = array(
    'label' => t('Subscription is cancelled.'),
    ) + $defaults;

  return $events;
}

/**
 * Implements hook_rules_condition_info().
 *
 * Define rules conditions.
 */
function subs_rules_condition_info() {
  $conditions = array();

  $conditions['subs_status'] = array(
    'group' => t('Subscription'),
    'label' => t("Check a subscription's status"),
    'base' => 'subs_condition_status',
    'parameter' => array(
      'subscription' => array(
        'type' => 'subs',
        'label' => t('Subscription object'),
        'save' => FALSE,
      ),
      'status' => array(
        'type' => 'integer',
        'label' => t('The status we want to check for'),
        'options list' => 'subs_status_options_list',
        'restriction' => 'input',
      ),
    ),
  );

  $conditions['subs_in_force'] = array(
    'group' => t('Subscription'),
    'label' => t('Check whether a subscription is in force'),
    'base' => 'subs_condition_in_force',
    'parameter' => array(
      'subscription' => array(
        'type' => 'subs',
        'label' => t('Subscription object'),
        'save' => FALSE,
      ),
    ),
  );

  return $conditions;
}

/**
 * Implements hook_rules_action_info().
 *
 * Define rules actions.
 */
function subs_rules_action_info() {
  $actions = array();

  // For convenience.
  $defaults = array(
    'group' => t('Subscription'),
    'access callback' => 'subs_rules_access',
    'parameter' => array(
      'subscription' => array(
        'type' => 'subs',
        'label' => t('Subscription object'),
        'save' => FALSE,
      ),
    ),
  );

  // Make a subscription pending.
  $actions['subs_pending'] = array(
    'label' => t('Make a subscription pending'),
    'base' => 'subs_rules_action_pending',
    ) + $defaults;

  // Make a subscription active.
  $actions['subs_active'] = array(
    'label' => t('Make a subscription active'),
    'base' => 'subs_rules_action_active',
    ) + $defaults;

  // Change a subscription to grace period.
  $actions['subs_grace'] = array(
    'label' => t('Change a subscription to grace period'),
    'base' => 'subs_rules_action_grace',
    ) + $defaults;

  // Expire a subscription.
  $actions['subs_expire'] = array(
    'label' => t('Expire a subscription'),
    'base' => 'subs_rules_action_expire',
    ) + $defaults;

  // Cancel a subscription.
  $actions['subs_cancel'] = array(
    'label' => t('Cancel a subscription'),
    'base' => 'subs_rules_action_cancel',
    ) + $defaults;

  // Renew a subscription.
  $actions['subs_renew'] = array(
    'label' => t('Renew a subscription'),
    'base' => 'subs_rules_action_renew',
    ) + $defaults;

  return $actions;
}

##################
### Conditions ###
#################

/**
 * Rules condition callback.
 * Checks whether a subscription has a given status.
 *
 * @param Subs
 *   The $subscription we want to check.
 * @param int
 *   The $status we want to check for.
 * @return boolean
 *   Whether the $subscription has the $status.
 *
 * @see subs_rules_condition_info()
 */
function subs_condition_status($subscription, $status) {
  // We bypss the standard subscription API to avoid hacky callbacks.
  return $subscription->status == $status;
}

/**
 * Rules condition callback.
 * Checks whether a subscription is in force (valid).
 *
 * @param Subs
 *   The $subscription we want to check.
 * @return boolean
 *   Whether the $subscription is in force.
 *
 * @see subs_rules_condition_info()
 */
function subs_condition_in_force($subscription) {
  return subs_in_force($subscription);
}

###############
### Actions ###
###############

/**
 * Rules action callback.
 * Makes a subscription pending.
 *
 * @param Subs
 *   The $subscription we want to make pending.
 * @return boolean
 *   Whether the action succeeded.
 *
 * @see subs_rules_action_info()
 */
function subs_rules_action_pending($subscription) {
  return subs_set_pending($subscription);
}

/**
 * Rules action callback.
 * Makes a subscription active.
 *
 * @param Subs
 *   The $subscription we want to make active.
 * @return boolean
 *   Whether the action succeeded.
 *
 * @see subs_rules_action_info()
 */
function subs_rules_action_active($subscription) {
  return subs_set_active($subscription);
}

/**
 * Rules action callback.
 * Changes a subscription to grace period.
 *
 * @param Subs
 *   The $subscription we want to change to grace period.
 *
 * @see subs_rules_action_info()
 */
function subs_rules_action_grace($subscription) {
  return subs_set_grace($subscription);
}

/**
 * Rules action callback.
 * Expires a subscription.
 *
 * @param Subs
 *   The $subscription we want to expire.
 * @return boolean
 *   Whether the action succeeded.
 *
 * @see subs_rules_action_info()
 */
function subs_rules_action_expire($subscription) {
  return subs_set_expired($subscription);
}

/**
 * Rules action callback.
 * Cancels a subscription.
 *
 * @param Subs
 *   The $subscription we want to cancel.
 * @return boolean
 *   Whether the action succeeded.
 *
 * @see subs_rules_action_info()
 */
function subs_rules_action_cancel($subscription) {
  return subs_set_cancelled($subscription);
}

/**
 * Rules action callback.
 * Renews a subscription.
 *
 * @param Subs
 *   The $subscription we want to renew.
 * @return boolean
 *   Whether the action succeeded.
 *
 * @see subs_rules_action_info()
 */
function subs_rules_action_renew($subscription) {
  return subs_renew($subscription);
}