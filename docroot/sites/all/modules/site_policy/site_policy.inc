<?php

/**
 * @file
 * Private functions for best practice module.
 */

/**
 * Determine if user 1 should be disabled.
 */
function _site_policy_should_user_1_be_disabled() {
  $time_limit = variable_get('site_policy_user_1_active_time', 0);
  if (empty($time_limit) || !is_numeric($time_limit) || $time_limit < 1) {
    return FALSE;
  }

  $account = user_load(1);
  return (!empty($account->status) && $account->access < (time() - $time_limit));
}

/**
 * Disable user 1 account if the last access time is outside maximum.
 */
function _site_policy_disable_user_1() {
  if (_site_policy_should_user_1_be_disabled()) {
    $account = user_load(1);
    $account->status = 0;
    user_save($account);
    watchdog('site_policy', 'User 1 account was disabled');
  }
}

/**
 * Scramble user 1 account password
 */
function _site_policy_scramble_user_1_password() {
  $time_limit = variable_get('site_policy_user_1_pw_scramble', 0);

  if (empty($time_limit) || !is_numeric($time_limit) || $time_limit < 1) {
    return;
  }

  if (_site_policy_get_next_scramble_user_1_password_time() > time()) {
    return;
  }

  $account = user_load(1);
  $new_passsword = user_password(50);
  user_save($account, array('pass' => $new_passsword));
  variable_set('site_policy_last_user1_password_scramble', time());
  watchdog('site_policy', 'User 1 password was scrambled');
}

/**
 * Get the next time the uset 1 password will be scrambled.
 * @return null
 */
function _site_policy_get_next_scramble_user_1_password_time() {
  $time_limit = variable_get('site_policy_user_1_pw_scramble', 0);
  $last_user1_password_scramble = variable_get('site_policy_last_user1_password_scramble', 0);

  if (empty($last_user1_password_scramble)) {
    $last_user1_password_scramble = time();
    variable_set('site_policy_last_user1_password_scramble', $last_user1_password_scramble);
  }

  return $last_user1_password_scramble + $time_limit;
}
