<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * @defgroup ms_affiliates_api MS Affiliates API
 * @{
 * Hooks and functions that are used by Affiliates Suite.
 *
 * Various hooks that are useful to act on affiliates events such as signup,
 * commission, etc.
 * @}
 */

/**
 * Allows you to take action when an affiliate is granted commission.
 *
 * This gets called whenever an affiliate is granted commission. You can use
 * this hook to save data to external tables, trigger extra actions, etc.
 *
 * @param float $commission
 *   The commission amount that has been granted.
 * @param int $level
 *   The level the commission was granted for.
 * @param float $amount
 *   The base amount used for calculating the commission.
 * @param object $affiliate
 *   The affiliate user.
 *
 * @ingroup ms_affiliates_api
 */
function hook_ms_affiliates_commission($commission, $level, $amount, $affiliate) {
  // No example.
}

/**
 * Allows you to take action when an affiliate gains a new referral.
 *
 * This gets called whenever an affiliate gains a new referral. You can use
 * this hook to save data to external tables, trigger extra actions, etc.
 *
 * @param object $referral
 *   The referral user.
 * @param object $affiliate
 *   The affiliate user.
 *
 * @ingroup ms_affiliates_api
 */
function hook_ms_affiliates_referral($referral, $affiliate) {
  // No example.
}
