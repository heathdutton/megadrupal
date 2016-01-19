<?php

/**
 * Validate that an argument user can access a view that displays giftcards
 * that reference a user as a recipient.
 */
class commerce_gc_views_plugin_argument_validate_user_giftcards extends views_plugin_argument_validate {
  function validate_argument($arg) {
    global $user;
    
    // If the person is viewing their own page, make sure they are allowed to 
    // view own giftcards.
    if ($user->uid == $arg) {
      return user_access('view received coupons of type giftcard_coupon');
    }
    
    // Otherwise, the user must be able to view all giftcards.
    return user_access('view any commerce_coupon entity of bundle giftcard_coupon') || user_access('view any commerce_coupon entity') || user_access('administer commerce_coupon entities');
  }
}
