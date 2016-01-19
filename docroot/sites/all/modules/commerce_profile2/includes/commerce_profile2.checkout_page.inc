<?php
/**
 * @file
 * Adds the profile2 checkout page.
 */

/**
 * Implements hook_commerce_checkout_page_info().
 *
 * This is where we define the name of the page and related information.
 *
 * @link http://www.drupalcommerce.org/node/224 explains hook_commerce_checkout_page_info @endlink
 */
function commerce_profile2_commerce_checkout_page_info() {

  $pages['profile2_checkout_page'] = array(
    'name' => t('Profile information'),
    'title' => t('Profile information'),

    // Determines the order of the checkout pages.
    'weight' => 0,

    // An order status is automatically created for each checkout page. It will
    // be named 'Checkout: ' + the name of the page.
    // If 'status_cart' is TRUE (default), then the 'cart' property of the order
    // status will be set to true, meaning that the commerce_cart module will
    // treat this the order as a cart at this point (price recalculations take
    // place on reload, and other cart-oriented functions may fire.)
    // If 'status_cart' is false, then prices are frozen on this page and
    // recalculation does not occur.
    'status_cart' => TRUE,

    // If 'buttons' is FALSE the next and previous buttons will be omitted.
    'buttons' => TRUE,

  );

  return $pages;
}
