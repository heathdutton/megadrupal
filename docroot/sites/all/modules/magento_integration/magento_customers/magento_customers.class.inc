<?php

/**
 * @file
 * Classes for interacting with the Magento Core API for Customers.
 */

require_once __DIR__ . '/../magento.class.inc';

/**
 * Extends the core Magento class to add methods for interacting with customers.
 */
class MagentoCustomers extends Magento {

  /**
   * Get the Magento Customer ID for a Drupal account.
   *
   * @param mixed $account
   *   Either uid or fully loaded user object. If not provided the currently
   *   logged in user will be used.
   */
  public function getCustomerId($account = NULL) {
    $customer = $this->getCustomer($account);
    return isset($customer['customer_id']) ? $customer['customer_id'] : FALSE;
  }

  /**
   * Get the Magento Customer for a Drupal account.
   *
   * @param mixed $account
   *   Either Drupal uid or fully loaded Drupal user object. If not provided the
   *   currently logged in user will be used.
   */
  public function getCustomer($account = NULL) {
    if (!$account) {
      global $user;
      $account = $user;
    }
    if (is_numeric($account)) {
      $account = user_load($account);
    }

    $customers = &drupal_static('magento_customers');
    if (!empty($customers[$account->uid])) {
      return $customers[$account->uid];
    }

    // We can't lookup Magento when we don't know the Drupal user.
    if (user_is_anonymous()) {
      return FALSE;
    }

    $response = $this->call('customer.list', array(
      'filters' => array('email' => $account->mail)
    ));
    $customers[$account->uid] = isset($response[0]) ? $response[0] : FALSE;
    return $customers[$account->uid];
  }

  /**
   * Get the total number of items in a cart for a customer.
   *
   * This API call uses a custom extension to extend the API.
   */
  public function getCartCount() {
    global $user;

    // We can't look up anonymous users in Magento.
    if (user_is_anonymous()) {
      return 0;
    }

    $account = user_load($user->uid);
    if (empty($account->data['magento_customer_id'])) {
      return 0;
    }
    $magento_customer_id = $account->data['magento_customer_id'];

    $cart_counts = &drupal_static('magento_customers_cart_count');
    if (!empty($cart_counts[$magento_customer_id])) {
      return $cart_counts[$magento_customer_id];
    }

    $response = $this->call('drupalcart.total_items', $magento_customer_id);
    if (is_numeric($response)) {
      $cart_counts[$magento_customer_id] = (int) $response;
      return $cart_counts[$magento_customer_id];
    }

    return 0;
  }
}

