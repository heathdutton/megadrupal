<?php

/**
 * @author "Ahmad Hesamzadeh" <mngafa@gmail.com>
 * @file Access handler which is based on Ubarcart orders
 *
 * Provides ability to set access control according to ubercart orders.
 * So you can sell files. Only users that have ordered the product, can request download link
 */

/**
 * Access handler callback for handler "ucorder"
 *
 * If the current user has purchased the given Ubercart product, return TRUE. Otherwise return FALSE.
 * In case of errors, it will also return FALSE.
 *
 * @param $product_nid The nid of the product
 *
 * @return bool
 */
function ngsl_access_byfile_handler_ucorder($product_nid) {
  // This handler requires the uc_order.module
  // So if it's not present, we don't have anything to do.
  if (!module_exists('uc_order')) {
    watchdog('nginx_securelink', 'ngsl_access_handler_ucorder: Module "uc_order" is not installed. You need this module for granting access to files based on ubercart orders. If yu don\'t install uc_order, all access request that are passed to this handler, will be rejected.', array(), WATCHDOG_WARNING);

    return FALSE;
  }

  global $user;

  // Anonymous users can't have purchased products.
  // So we ignore them here to reduce database traffic.
  if ($user->uid == 0) {
    return FALSE;
  }

  // Order should be placed not before this time
  // Set it to zero: All order will be valid.
  $oldest_valid_time = 0;

  // List all orders of the given product which are placed by the current user
  // We wan't to check if th current user has purchased this product or not.
  $query = db_select('uc_orders', 'uo');
  $query->fields('uo', array('order_id'));

  // Join to uc_order_products table for product's nid field.
  $uop_alias = $query->innerJoin('uc_order_products', 'uop', '%alias.order_id = uo.order_id');

  $query->condition("{$uop_alias}.nid", $product_nid);
  $query->condition('uo.uid', $user->uid);
  $query->condition('uo.created', $oldest_valid_time, '>=');

  // Only completed (paid) orders
  $query->condition('uo.order_status', 'completed');

  $result = $query->execute()->fetchAll();

  return !empty($result);
}
