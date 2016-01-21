<?php

/**
 * @file
 * Functions for returning Ajax content for Magento integration.
 */

/**
 * Returns customer cart count.
 */
function magento_customers_cart_count() {
  global $user;
  $build = array();
  $links = array();
  $store_url = variable_get('magento_store_url');

  module_load_include('class.inc', 'magento_customers');
  $magento_connection = new MagentoConnection();
  $magento = new MagentoCustomers($magento_connection);
  $cart_count = (int) $magento->getCartCount();

  cache_set('magento_cart_count:' . $user->uid, $cart_count, 'cache', time() + (60*5));

  $package = array('cart_count' => $cart_count);

  return drupal_json_output($package);
}
