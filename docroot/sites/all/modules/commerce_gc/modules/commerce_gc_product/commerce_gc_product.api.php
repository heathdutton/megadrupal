<?php

/**
 * Alter a coupon right before it is saved as a result of someone buying a
 * giftcard product.
 * 
 * @param EntityDrupalWrapper $coupon_wrapper
 *  The new coupon, wrapped
 * 
 * @param EntityDrupalWrapper $line_item_wrapper
 *  The wrapped giftcard purchase line item
 */
function hook_commerce_gc_product_giftcard_coupon_presave_alter($coupon_wrapper, $line_item_wrapper) {
  // Set the coupon code to a static value
  $coupon_wrapper->code = 'static_code';
}

/**
 * If the purchasing customer has specified a new email address for the
 * giftcard recipient, this hook allows modules to alter the newly created user.
 * 
 * @param stdClass $user
 *  A newly created user that is about to be saved
 * 
 * @param CommerceCoupon $coupon
 *  The coupon that was created for the new user
 */
function hook_commerce_gc_product_new_recipient_user_alter($user, $coupon) {
  // Add a default role
  $user->roles[] = 'Recipient role';
}