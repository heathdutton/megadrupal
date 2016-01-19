<?php

/**
 * @file
 * Process payment redirected from Everyday Online Payment service.
 */

/**
 * Finalizes Everyday transaction.
 */
function uc_everyday_complete($cart_id = 0) {
  // Load order.
  if (arg(2) == 'complete' && arg(4)) {
    watchdog('uc_everyday', 'Receiving new order notification for order @order_id.', array('@order_id' => arg(4)));
    $hash_check = _uc_everyday_hash_check();

    $order = uc_order_load(arg(4));
  }

  if ($order === FALSE || uc_order_status_data($order->order_status, 'state') != 'in_checkout' || $hash_check != check_plain($_GET['OPR_RETURN_MAC'])) {
    watchdog('uc_everyday', '/cart/everyday/complete attempted for non-existent order.', array(), WATCHDOG_ERROR);

    drupal_set_message(t('An error has occurred during payment. Please contact us to ensure your order has submitted.'), 'ERROR');
    drupal_goto('cart/');

    exit();
  }

  // Save changes to order without it's completion - it will be on
  // finalization step. See http://drupal.org/node/1332130
  uc_order_save($order);

  if ($order) {

    $context = array(
      'revision' => 'formatted-original',
      'type' => 'amount',
    );

    $options = array(
      'sign' => FALSE,
      'dec' => '.',
      'thou' => FALSE,
    );

    // Define local variables.
    $opr_reference_nbr = check_plain($_GET['OPR_RETURN_PAID']);

    // Add payment and comments.
    $comment = t('Everyday reference number: @txn_id', array('@txn_id' => $opr_reference_nbr));
    uc_payment_enter($order->order_id, 'everyday', number_format($order->order_total, 2), $order->uid, NULL, $comment);
    uc_order_comment_save($order->order_id, 0,
      t('Payment of @amount @currency submitted through Everyday.', array(
        '@amount' => number_format($order->order_total, 2),
        '@currency' => 'EUR')),
      'order', 'payment_received');
    uc_order_comment_save($order->order_id, 0,
      t('Everyday reported a payment of @amount @currency.', array(
        '@amount' => number_format($order->order_total, 2),
        '@currency' => 'EUR')),
      'admin');
  }
  else {
    drupal_set_message(t('Errors in response parameters from Everday service has been detected. Please contact the site administrator.'));
  }

  // Add a comment to let sales team know this came in through the site.
  uc_order_comment_save($order->order_id, 0, t('Order created through Everyday Online Payment website.'), 'admin');

  // Update order details to enable checkout completion for new logged in users
  // (if enabled).
  $order = uc_order_load($order->order_id);

  // From uc_cart_checkout_complete().
  $build = uc_cart_complete_sale($order, variable_get('uc_new_customer_login', FALSE));

  $page = variable_get('uc_cart_checkout_complete_page', '');

  // Empty that cart...
  uc_cart_empty(uc_cart_get_id());

  if (!empty($page)) {
    drupal_goto($page);
  }

  return $build;
}

/**
 * Handles a cancelled payment from Everyday.
 */
function uc_everyday_cancel() {
  drupal_set_message(t('Payment through Everyday Online Payment service has been cancelled. Your order is not completed. Please try again or contact us.'), 'ERROR');
  drupal_goto('cart/');
}

/**
 * Handles an error in payment from Everyday.
 */
function uc_everyday_reject() {
  drupal_set_message(t('Payment has been rejected by Everyday Online Payment service. Please select another payment method to complete your order.'), 'ERROR');
  drupal_goto('cart/');
}


/*******************************************************************************
 * Module and Helper Functions
 ******************************************************************************/

/**
 * Calculate hash based on returned values - checked against returned hash.
 */
function _uc_everyday_hash_check() {
  // Validate response fields with hash.
  $hash_elements = array(
    'OPR_RETURN_VERSION',
    'OPR_RETURN_STAMP',
    'OPR_RETURN_REF',
    'OPR_RETURN_PAID',
  );

  foreach ($hash_elements as $key => $element) {
    $hash_elements[$element] = check_plain($_GET[$element]);
    unset($hash_elements[$key]);
  }

  if (variable_get('uc_everyday_mode', '1') == '1') {
    $everyday_secretkey = variable_get('uc_everyday_test_secret_key', 'NGNhODg0ZjA0NjYxNzllZmQxNWRhZA');
  }
  else {
    $everyday_secretkey = variable_get('uc_everyday_secret_key', '');
  }

  $hash_elements['secret_key'] = $everyday_secretkey;

  // Generate hash with md5, convert to uppercase.
  $hash = strtoupper(md5(implode('&', $hash_elements) . '&'));

  return $hash;
}
