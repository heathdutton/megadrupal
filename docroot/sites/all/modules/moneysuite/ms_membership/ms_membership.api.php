<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup ms_membership_api MS Membership API
 * @{
 * Hooks and functions that are used by Membership Suite.
 *
 * Various hooks that are useful to act on membership events such as signup,
 * cancellation, expiration, etc.
 *
 
 * @}
 */

/**
 * Allows you to take action when a membership signup occurs
 *
 * This gets called whenever a new membership is created. You can use this hook
 * to save data to external tables, trigger extra actions, etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $m_plan
 *   The membership plan object.
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_signup($account, $membership, $m_plan) {
  // No example.
}

/**
 * Allows you to take action when a membership is renewed
 *
 * This gets called whenever a membership that was canceled or expired is renewed.
 * You can use this hook to save data to external tables, trigger extra actions, etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $m_plan
 *   The membership plan object.
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_renewal($account, $membership, $m_plan) {
  // No example.
}

/**
 * Allows you to take action when a membership changes from one plan to another
 *
 * This gets called whenever a membership is modified from one plan to another.
 * You can use this hook to save data to external tables, trigger extra actions,
 * etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $new_plan
 *   The new membership plan object.
 * @param MsProductsPlan $old_plan
 *   The old membership plan object
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_modification($account, $membership, $new_plan, $old_plan) {
  // No example.
}

/**
 * Allows you to take action when a membership is marked as expiring soon
 *
 * This gets called whenever a membership passes the expiring soon threshold.
 * You can use this hook to save data to external tables, trigger extra actions,
 * etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $m_plan
 *   The membership plan object.
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_expiring_soon($account, $membership, $m_plan) {
  // No example.
}

/**
 * Allows you to take action when a membership expires
 *
 * This gets called whenever a membership expires. You can use this hook
 * to save data to external tables, trigger extra actions, etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $m_plan
 *   The membership plan object.
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_expiring($account, $membership, $m_plan) {
  // No example.
}

/**
 * Allows you to take action when a membership cancellation occurs
 *
 * This gets called whenever a membership is canceled. You can use this hook
 * to save data to external tables, trigger extra actions, etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $m_plan
 *   The membership plan object.
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_cancel($account, $membership, $m_plan) {
  // No example.
}

/**
 * Allows you to take action when a membership payment occurs
 *
 * This gets called whenever a payment is registered for a membership. You can
 * use this hook to save data to external tables, trigger extra actions, etc.
 *
 * @param object $account
 *   The user object for the account associated with the membership.
 * @param object $membership
 *   The membership object.
 * @param MsProductsPlan $m_plan
 *   The membership plan object.
 *
 * @ingroup ms_membership_api
 */
function hook_ms_membership_payment($account, $membership, $m_plan) {
  // No example.
}
