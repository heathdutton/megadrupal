<?php

/**
 * @file
 * Implementation of all common customer interface hooks for the user type
 * of customers.
 */

/**
 * Implements hook_customer_get_id().
 */
function ec_customer_user_customer_get_id($info) {
  global $user;
  if (isset($info['type']) && $info['type'] == 'user' && isset($info['exid']) && is_numeric($info['exid'])) {
    return array(
      'type' => 'user',
      'exid' => $info['exid'],
    );
  }
  elseif (!empty($user->uid)) {
    return array(
      'type' => 'user',
      'exid' => $user->uid,
    );
  }
}

/**
 * Implements hook_customer_get_by_uid().
 */
function ec_customer_user_customer_get_by_uid($uid) {
  return array(
    'type' => 'user',
    'exid' => $uid,
  );
}

/**
 * Implements hook_customer_get_by_email().
 */
function ec_customer_user_customer_get_by_email($mail) {
  if ($uid = db_query_range("SELECT uid FROM {users} WHERE mail = :mail", array())->fetchField()) {
    return array(
      'type' => 'user',
      'exid' => $uid,
    );
  }
}

/**
 * Implements hook_customer_get_uid().
 */
function ec_customer_user_customer_get_uid($customer) {
  if ($customer->type == 'user') {
    return $customer->exid;
  }
}

/**
 * Implements hook_customer_get_email().
 */
function ec_customer_user_customer_get_email($customer) {
  if ($customer->type == 'user' && ($account = user_load($customer->exid))) {
    return $account->mail;
  }
}

/**
 * Implements hook_customer_get_info().
 */
function ec_customer_user_customer_get_info($customer) {
  if ($account = user_load($customer->exid)) {
    $info = new stdClass;
    $info->name = $account->name;
    return $info;
  }
}

/**
 * Implements hook_customer_get_name().
 */
function ec_customer_user_customer_get_name($customer) {
  $name = NULL;
  if ($addresses = ec_customer_get_addresses($customer)) {
    $address = reset($addresses);
    $name = trim($address['firstname'] . ' ' . $address['lastname']);
  }
  if (empty($name) && $account = user_load($customer->exid)) {
    $name = $account->name;
  }
  return $name;
}

