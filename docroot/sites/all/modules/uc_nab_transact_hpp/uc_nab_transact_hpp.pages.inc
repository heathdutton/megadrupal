<?php
/**
 * @file
 * Menu item callback functions.
 */

/**
 * Page callback to handle NAB reply.
 *
 * There does not seem to be a method to authenticate this request! The order
 * param IS a valid object by now.
 */
function uc_nab_transact_hpp_callback_complete($order) {

  // Order must be "in_checkout".
  if (uc_order_status_data($order->order_status, 'state') != 'in_checkout') {
    watchdog('uc_nab_transact_hpp', 'Attempt to callback Order @order_id when it was not "in_checkout".', array('@order_id' => $order->order_id), WATCHDOG_ERROR);
    drupal_exit();
  }

  // Payment method must be NAB HPP.
  if ($order->payment_method != 'uc_nab_transact_hpp') {
    watchdog('uc_nab_transact_hpp', 'Attempt to callback Order @order_id when it was not set to NAB HPP payment method.', array('@order_id' => $order->order_id), WATCHDOG_ERROR);
    drupal_exit();
  }

  watchdog('uc_nab_transact_hpp', 'NAB HPP reply_link_url access for order #@order_id <pre>@debug</pre>', array('@order_id' => $order->order_id, '@debug' => print_r($_GET, TRUE)));

  // Order must be valid to reach here. In a matter of speaking. NAB HPP does
  // not appear to provide any method to AUTHENTICATE that this reply came
  // from them. Anyone could pass anything to this callback as the details are
  // in plain sight for anyone to look at.
  // This is bonkers.
  $comment = t('Payment received from NAB HPP. Bank Reference #@bank_ref', array(
    '@bank_ref' => $_GET['bank_reference'],
  ));
  uc_payment_enter($order->order_id, 'uc_nab_transact_hpp', uc_currency_format($_GET['payment_amount'], FALSE, FALSE, '.'), 0, NULL, $comment);

  print 'OK';

  // Bypass theming engine by returning NULL.
  return NULL;
}

/**
 * Page callback to handle returning user.
 */
function uc_nab_transact_hpp_user_complete($order) {
  // If the order ID specified in the return URL is not the same as the one in
  // the user's session, we need to assume this is either a spoof or that the
  // user tried to adjust the order on this side while at NAB HPP. If it was a
  // legitimate checkout, the reply_link_url will be accessed by someone and
  // be processed correctly. We'll leave an ambiguous message just in case.
  if (!isset($_SESSION['cart_order']) || intval($_SESSION['cart_order']) != $order->order_id) {
    drupal_set_message(t('Thank you for your order! The NAB will notify us once your payment has been processed.'));
    drupal_goto('cart');
  }

  // Ensure the payment method is NAB HPP.
  if ($order->payment_method != 'uc_nab_transact_hpp') {
    drupal_goto('cart');
  }

  // This lets us know it's a legitimate access of the complete page.
  $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
  drupal_goto('cart/checkout/complete');
}
