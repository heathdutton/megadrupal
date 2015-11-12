<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup ms_paypublish_api MS Pay to Publish API
 * @{
 * Hooks and functions that are used by Pay to Publish.
 *
 * Various hooks and functions that are useful to act on events such as
 * content paid for publishing, cancellation, etc.
 *
 * @}
 */

/**
 * Allows you to take action when a payment is received.
 *
 * This gets called whenever a payment is received for a node. You can use
 * this hook to save data to external tables, trigger extra actions, etc.
 *
 * @param object $account
 *   The user object.
 * @param object $pp_node
 *   The pay to publish node record.
 * @param object $plan
 *   The affiliate user.
 *
 * @ingroup ms_paypublish_api
 */
function hook_ms_paypublish_payment($account, $pp_node, $plan) {
  // No example.
}

/**
 * Allows you to take action when a purchase is made.
 *
 * @param object $account
 *   The user object.
 * @param object $pp_node
 *   The pay to publish node record.
 * @param object $plan
 *   The affiliate user.
 *
 * @ingroup ms_paypublish_api
 */
function hook_ms_paypublish_purchase($account, $pp_node, $plan) {
  // No example.
}

/**
 * Allows you to take action when a node is expiring.
 *
 * @param object $account
 *   The user object.
 * @param object $pp_node
 *   The pay to publish node record.
 * @param object $plan
 *   The affiliate user.
 *
 * @ingroup ms_paypublish_api
 */
function hook_ms_paypublish_expiring($account, $pp_node, $plan) {
  // No example.
}

/**
 * Allows you to take action when a node is canceled.
 *
 * @param object $account
 *   The user object.
 * @param object $pp_node
 *   The pay to publish node record.
 * @param object $plan
 *   The affiliate user.
 *
 * @ingroup ms_paypublish_api
 */
function hook_ms_paypublish_cancel($account, $pp_node, $plan) {
  // No example.
}