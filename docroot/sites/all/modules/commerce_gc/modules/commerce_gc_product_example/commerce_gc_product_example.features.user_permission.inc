<?php
/**
 * @file
 * commerce_gc_product_example.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function commerce_gc_product_example_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'redeem non user specific coupons of type giftcard_coupon'.
  $permissions['redeem non user specific coupons of type giftcard_coupon'] = array(
    'name' => 'redeem non user specific coupons of type giftcard_coupon',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'commerce_coupon_user',
  );

  // Exported permission: 'redeem received coupons of type giftcard_coupon'.
  $permissions['redeem received coupons of type giftcard_coupon'] = array(
    'name' => 'redeem received coupons of type giftcard_coupon',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'commerce_coupon_user',
  );

  // Exported permission: 'view non user specific coupons of type giftcard_coupon'.
  $permissions['view non user specific coupons of type giftcard_coupon'] = array(
    'name' => 'view non user specific coupons of type giftcard_coupon',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'commerce_coupon_user',
  );

  // Exported permission: 'view own giftcard transactions'.
  $permissions['view own giftcard transactions'] = array(
    'name' => 'view own giftcard transactions',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'commerce_gc',
  );

  // Exported permission: 'view received coupons of type giftcard_coupon'.
  $permissions['view received coupons of type giftcard_coupon'] = array(
    'name' => 'view received coupons of type giftcard_coupon',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'commerce_coupon_user',
  );

  return $permissions;
}
