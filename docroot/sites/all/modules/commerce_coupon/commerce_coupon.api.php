<?php
/**
 * @file
 * Hooks provided by Commerce Coupon module.
 */

/**
 * @defgroup commerce_coupon_api_hooks Commerce Coupon Hooks
 * @{
 */

/**
 * This hook is called to check if a coupon type can be deleted.
 *
 * @param $coupon
 *   The coupon to be checked for deletion.
 */
function hook_commerce_coupon_can_delete($type) {
}

/**
 * Allow other modules to configure coupon types.
 *
 * @see commerce_coupon_type_configure().
 */
function hook_commerce_coupon_type_configure($bundle, $reset) {
}

/**
 * Alter the granted amount in the log-type coupon views.
 *
 * @param $amount
 *   Textual value representing the amount discounted.
 * @param $coupon
 *   Coupon applied
 * @param $order
 *   Order discounted
 */
function hook_commerce_coupon_granted_amount_alter($amount, $coupon, $order) {
}
