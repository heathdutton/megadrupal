<?php
/**
 * @file
 * Module API.
 */

/**
 * Module functions.
 */

/**
 * Check existing entity cache tables.
 */
function chargebee_check_cache_tables_exists() {
  $tables = array(
    'cache_entity_chargebee_subscription',
    'cache_entity_chargebee_customer',
    'cache_entity_chargebee_card',
    'cache_entity_chargebee_plan',
  );
  foreach ($tables as $table) {
    if (!db_table_exists($table)) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Get plans list from ChargeBee.
 */
function chargebee_get_plans() {
  $plans = array();
  if (_chargebee_common_init()) {
    $plans = _chargebee_plan_get_all();
  }
  return $plans;
}

/**
 * Retrieve plan from ChargeBee.
 */
function chargebee_plan_retrieve($plan_id) {
  $plan = array();
  if (_chargebee_common_init()) {
    $plan = _chargebee_plan_retrieve($plan_id);
  }
  return $plan;
}

/**
 * Get checkout hostedpage.
 *
 * @param object $user
 *   User object.
 * @param string $plan_id
 *   ChargeBee plan identifier.
 *
 * @return array
 *   Hostedpage array.
 */
function chargebee_get_hostedpage_checkout($user, $plan_id) {
  $hostedpage = '';
  if (_chargebee_common_init()) {
    $params = array(
      'customer' => array(
        'email' => $user->mail,
      ),
    );
    $subscription = chargebee_subscription_load($user->uid);
    if ($subscription) {
      $params['subscription']['id'] = $subscription->subscription_id;
      $hostedpage = _chargebee_hostedpage_checkout_existing($params);
    }
    else {
      $params['subscription']['planId'] = $plan_id;
      $hostedpage = _chargebee_hostedpage_checkout_new($params);
    }
  }
  return $hostedpage;
}

/**
 * Get card update hostedpage.
 *
 * @param object $user
 *   User object.
 *
 * @return array|null
 *   Hostedpage array.
 */
function chargebee_get_hostedpage_card_update($user) {
  $hostedpage = NULL;
  if (_chargebee_common_init()) {
    $customer = chargebee_customer_load($user->uid);
    if ($customer) {
      $hostedpage = _chargebee_hostedpage_update_card($customer->customer_id);
    }
  }
  return $hostedpage;
}

/**
 * Retrieve hostedpage by ID.
 *
 * @param string $hostedpage_id
 *   Hostedpage identifier.
 *
 * @return array
 *   Hostedpage with or without content.
 */
function chargebee_hostedpage_retrieve($hostedpage_id) {
  $hostedpage = NULL;
  if (_chargebee_common_init()) {
    $hostedpage = _chargebee_hostedpage_retrieve($hostedpage_id);
  }
  return $hostedpage;
}

/**
 * Retrieve subscription from ChargeBee.
 *
 * @param string $subscription_id
 *   ChargeBee subscription identifier.
 *
 * @return array
 *   Subscription array.
 */
function chargebee_subscription_retrieve($subscription_id) {
  $subscription = NULL;
  if (_chargebee_common_init()) {
    $subscription = _chargebee_subscription_retrieve($subscription_id);
  }
  return $subscription;
}

/**
 * Update billing information for customer.
 *
 * @param string $customer_id
 *   Chargebee customer identifier.
 * @param array $params
 *   Params for update.
 *
 * @return array
 *   Customer and card array.
 */
function chargebee_customer_update_billinginfo($customer_id, $params) {
  $result = array();
  if (_chargebee_common_init()) {
    $result = _chargebee_customer_update_billinginfo($customer_id, $params);
  }
  return $result;
}

/**
 * Update subscription plan.
 *
 * @param string $subscription_id
 *   ChargeBee subscription identifier.
 * @param string $plan_id
 *   New plan identifier.
 *
 * @return array
 *   Subscription array.
 */
function chargebee_subscription_update_plan($subscription_id, $plan_id) {
  $result = array();
  if (_chargebee_common_init()) {
    $result = _chargebee_subscription_update_plan($subscription_id, $plan_id);
  }
  return $result;
}

/**
 * Cancel subscription.
 *
 * @param string $subscription_id
 *   ChargeBee subscription identifier.
 *
 * @return array
 *   Subscription array.
 */
function chargebee_subscription_cancel($subscription_id) {
  $result = array();
  if (_chargebee_common_init()) {
    $result = _chargebee_subscription_cancel($subscription_id);
  }
  return $result;
}

/**
 * Verify subscription data.
 *
 * @param object $user
 *   User object.
 *
 * @return bool
 *   Verify status.
 */
function chargebee_verify_subscription_status($user) {
  $subscription = chargebee_subscription_load($user->uid);
  if ($subscription) {
    $retrieve = chargebee_subscription_retrieve($subscription->subscription_id);
    if ($retrieve) {
      $attributes = array(
        'plan_id',
        'status',
        'current_term_start',
        'current_term_end',
      );
      foreach ($attributes as $attr) {
        if ($subscription->$attr != $retrieve['subscription'][$attr]) {
          chargebee_subscription_update($subscription, $retrieve['subscription']);
          $message = 'Found a discrepancy in the database for user @uid (@name). Updated automatically.';
          watchdog('ChargeBee Verify', $message, array('@uid' => $user->uid, '@name' => $user->name), WATCHDOG_WARNING);
          return FALSE;
        }
      }
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Change user role by subscription status.
 *
 * @param object $user
 *   User object.
 * @param object $subscription
 *   ChargeBee subscription entity object.
 *
 * @return bool
 *   Update status.
 */
function chargebee_change_user_role($user, $subscription) {
  $active_roles = array('in_trial', 'active', 'non_renewing');
  $updated = NULL;
  if (in_array($subscription->status, $active_roles)) {
    $updated = chargebee_assign_role_by_plan($user, $subscription->plan_id);
  }
  else {
    $updated = chargebee_unassign_role_by_plan($user, $subscription->plan_id);
  }
  return $updated;
}

/**
 * Assign role to user by plan ID.
 *
 * @param object $user
 *   User object.
 * @param string $plan_id
 *   ChargeBee plan identifier.
 *
 * @return object
 *   Changed user object.
 */
function chargebee_assign_role_by_plan($user, $plan_id) {
  $plans_roles = variable_get('chargebee_plans_roles', array());
  if ($plans_roles) {
    $role = user_role_load($plans_roles[$plan_id]);
    $user->roles[$role->rid] = $role->name;
    user_save($user);
    return TRUE;
  }
  return FALSE;
}

/**
 * Unassign role from user by plan ID.
 *
 * @param object $user
 *   User object.
 * @param string $plan_id
 *   ChargeBee plan identifier.
 *
 * @return object
 *   Changed user object.
 */
function chargebee_unassign_role_by_plan($user, $plan_id) {
  $plans_roles = variable_get('chargebee_plans_roles');
  if ($plans_roles) {
    $role = user_role_load($plans_roles[$plan_id]);
    if (isset($user->roles[$role->rid])) {
      unset($user->roles[$role->rid]);
      user_save($user);
      return TRUE;
    }
  }
  return FALSE;
}
