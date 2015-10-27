<?php
/**
 * @file
 * XC Authentication account funtions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_xc_account().
 *
 * Get information about an account.
 *
 * @param $op (string)
 *   The type of requested information. E.g.: 'personal info', 'loaned items', 'requested items'
 * @param $account (Object)
 *   The object or record identifier of the account
 *
 * @return (Array)
 *   The requested information about the account
 */
function xc_auth_xc_account($op, $account, $values) {
  $account_info = array();
  $auth_users = xc_auth_user_get_all($account->uid);
  foreach ($auth_users as $auth_user) {
    $auth_type = xc_auth_type_load($auth_user->tid);

    // Run method function for returning account information
    // xc_auth_method_ncip_provider_xc_account
    $method_func = 'xc_auth_method_' . check_plain($auth_type->method) . '_xc_account';

    if (function_exists($method_func)) {
      $account_info = array_merge_recursive($account_info, $method_func($op, $account, $values, $auth_user, $auth_type));
    }
  }

  return $account_info;
}
