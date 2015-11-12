<?php

/**
 * @file
 * Theme functions and preprocess functions for Magento Customer integration.
 */

/**
 * Show a shopping cart and checkout button.
 */
function theme_magento_cart_links() {
  global $user;
  $build = array();

  $cart_count = NULL;
  $cache = cache_get('magento_cart_count:' . $user->uid);
  if (isset($cache->data)) {
    $cart_count = $cache->data;
  }

  $links = array();
  $store_url = variable_get('magento_store_url');

  // We check for a customer ID even though getCartCount() does it to avoid the
  // overhead of establishing a connection to the Magento API, which is super
  // slow.
  if (!$cart_count && user_is_logged_in() && ($account = user_load($user->uid)) && !empty($account->data['magento_customer_id'])) {
    module_load_include('class.inc', 'magento_customers');
    $magento_connection = new MagentoConnection();
    $magento = new MagentoCustomers($magento_connection);
    $cart_count = $magento->getCartCount();
  }

  // If we still don't have a cart count assume the cart is empty so that we
  // show something logical to the user.
  if (!$cart_count) {
    $cart_count = 0;
  }
  cache_set('magento_cart_count:' . $user->uid, $cart_count, 'cache', time() + (60*5));

  $cart_text = t('<div class="cart-name">Cart</div> <div class="shopping-cart-icon"></div><div class="count">(@count)</div>', array(
    '@count' => $cart_count,
  ));
  $links[] = l($cart_text, $store_url . '/checkout/cart', array(
    'attributes' => array('class' => array('store-cart')),
    'html' => TRUE,
  ));
  $links[] = l(t('Checkout'), $store_url . '/checkout', array(
    'attributes' => array('class' => array('checkout')),
  ));

  $build = array(
    '#theme' => 'item_list',
    '#items' => $links,
    '#attributes' => array('class' => array('magento-menu')),
  );

  return $build;
}
