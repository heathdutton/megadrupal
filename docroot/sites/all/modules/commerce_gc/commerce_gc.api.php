<?php

/**
 * Determine whether a giftcard already is attached to an order is still valid.
 * Similar to hook_commerce_coupon_coupon_still_valid_alter, but this one runs
 * during the commerce_cart_order_refresh implementation of Commerce GC when we
 * are deciding which giftcard usage line items are still valid.
 * 
 * @param boolean $valid
 *  Whether or not the coupon may still apply.
 * 
 * @param EntityDrupalWrapper $coupon_wrapper
 *  The giftcard coupon being evaluated
 * 
 * @param EntityDrupalWrapper $order_wrapper
 *  The order that the giftcard is attached to
 */
function commerce_gc_giftcard_still_valid_alter(&$valid, $coupon_wrapper, $order_wrapper) {
  // Invalidate giftcard coupons on orders older than a day
  if ($order_wrapper->created->value() + 86400 < REQUEST_TIME) {
    $outcome = FALSE;
  }
}