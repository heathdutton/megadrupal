<?php

/**
 * @file
 * Hooks provided by the PayPal Roles module.
 */
/**
 * @addtogroup hooks
 * @{
 */

/**
 * This hook allow to modify / override user data array before creating
 * a new user account.
 *
 * @param array $account_data
 *   The account array which contains data about user.
 *
 * @param array $rawipn
 *   The raw IPN data posted by PayPal.
 */
function hook_paypal_roles_user_data_alter(&$account_data, $rawipn) {
  // Example to replace value of "name" with email address, and add
  // extra field ("field_name") with full name of PayPal user.
  $user_data = array(
    'name' => $account_data['mail'],
    'pass' => $account_data['pass'],
    'mail' => $account_data['mail'],
    'status' => $account_data['status'],
    'init' => $account_data['mail'],
    'field_name' => array(
      LANGUAGE_NONE => array(
        0 => array(
          'value' => $rawipn['first_name'] . ' ' . $rawipn['last_name'],
        ),
      ),
    ),
  );

  // Override user data array.
  $account_data = $user_data;
}

/**
 * This hook is invoked from paypal_roles_memberships_load() after the
 * memberships are loaded using paypal_roles_memberships_load_multiple().
 *
 * @param object $ml
 *   The membership objects that are being altered.
 */
function hook_paypal_roles_memberships_alter($ml) {

}

/**
 * This hook is invoked from paypal_roles_memberships_save() before the
 * membership is saved to the database.
 *
 * @param object $ml
 *   The membership that is being inserted or updated.
 */
function hook_paypal_roles_memberships_presave($ml) {
  
}

/**
 * This hook is invoked from paypal_roles_memberships_save() after the database
 * query that will update membership in the paypal_roles_memberships table is
 * scheduled for execution.
 *
 * @param object $ml
 *   The membership that is being updated.
 */
function hook_paypal_roles_memberships_update($ml) {
  
}

/**
 * This hook is invoked from paypal_roles_memberships_save() after the database
 * query that will insert membership in the paypal_roles_memberships table is
 * scheduled for execution.
 *
 * @param object $ml
 *   The membership that is being inserted.
 */
function hook_paypal_roles_memberships_insert($ml) {
  
}

/**
 * This hook is invoked from paypal_roles_payments_save() before the payment is
 * saved to the database.
 *
 * @param object $pmt
 *   The payment that is being inserted or updated.
 */
function hook_paypal_roles_payments_presave($pmt) {
  
}

/**
 * This hook is invoked from paypal_roles_payments_save() after the database
 * query that will update payment in the paypal_roles_payments table is
 * scheduled for execution.
 *
 * @param object $pmt
 *   The payment that is being updated.
 */
function hook_paypal_roles_payments_update($pmt) {
  
}

/**
 * This hook is invoked from paypal_roles_payments_save() after the database
 * query that will insert payment in the paypal_roles_payments table is
 * scheduled for execution.
 *
 * @param object $pmt
 *   The payment that is being inserted.
 */
function hook_paypal_roles_payments_insert($pmt) {
  
}

/**
 * This hook is invoked from paypal_roles_custom_payments_save() before the
 * payment is saved to the database.
 *
 * @param object $pmt
 *   The payment that is being inserted or updated.
 */
function hook_paypal_roles_custom_payments_presave($pmt) {
  
}

/**
 * This hook is invoked from paypal_roles_custom_payments_save() after the
 * database query that will update payment in the paypal_roles_custom_payments
 * table is scheduled for execution.
 *
 * @param object $pmt
 *   The payment that is being updated.
 */
function hook_paypal_roles_custom_payments_update($pmt) {
  
}

/**
 * This hook is invoked from paypal_roles_custom_payments_save() after the
 * database query that will insert payment in the paypal_roles_custom_payments
 * table is scheduled for execution.
 *
 * @param object $pmt
 *   The payment that is being inserted.
 */
function hook_paypal_roles_custom_payments_insert($pmt) {
  
}

/**
 * This hook is invoked from paypal_roles_paypal_roles_add_roles_to_account() before the
 * roles are added to the users role set.
 *
 * @param array $roles
 *   The roles that that are added to the user
 */

function hook_paypal_roles_add_roles_to_account_alter($roles, $uid, $account){

}

/**
 * This hook is invoked from paypal_roles_paypal_roles_remove_roles_from_account() after the
 * roles are removed to the users role set.
 *
 * @param array $roles
 *   The roles that that are removed from the user
 */

function hook_paypal_roles_remove_roles_from_account_alter($roles, $uid, $account){

}

/**
 * @} End of "addtogroup hooks".
 */