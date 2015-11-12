<?php

/**
 * Handles a complete sale.
 */
function uc_mercadopago_complete($order) {
  // If the order ID specified in the return URL is not the same as the one in
  // the user's session, we need to assume this is either a spoof or that the
  // user tried to adjust the order on this side while at PayPal. If it was a
  // legitimate checkout, the IPN will still come in from PayPal so the order
  // gets processed correctly. We'll leave an ambiguous message just in case.
  if (!isset($_SESSION['cart_order']) || intval($_SESSION['cart_order']) != $order->order_id) {
    drupal_set_message(t('Thank you for your order! PayPal will notify us once your payment has been processed.'));
    drupal_goto('cart');
  }

  // This lets us know it's a legitimate access of the complete page.
  $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
  drupal_goto('cart/checkout/complete');
}

/**
 * Handles a canceled sale.
 */
function uc_mercadopago_cancel() {
  unset($_SESSION['cart_order']);

  drupal_set_message(t('Your MercadoPago payment was canceled. Please feel free to continue shopping or contact us for assistance.'));

  drupal_goto(variable_get('uc_mercadopago_cancel_return_url', 'cart'));
}


/**
 * Processes Instant Payment Notifiations from PayPal.
 */
function uc_mercadopago_ipn() {
  global $user;

  if (!isset($_GET['topic']) || $_GET['topic'] != 'payment' || !isset($_GET['id'])) {
    watchdog('uc_mercadopago', 'IPN attempted with invalid request.', array(), WATCHDOG_ERROR);
    return;
  }

  libraries_load('mercadopago');

  $mp = new MP(variable_get('uc_mercadopago_client_id', ''), variable_get('uc_mercadopago_client_secret', ''));

  if (isset($_GET['sandbox']) && $_GET['sandbox'] == "true") {
    $mp->sandbox_mode(TRUE);
  }
  else {
    $mp->sandbox_mode(FALSE);
  }

  try {
    $paymentInfo = $mp->get_payment_info($_GET["id"]);
    $payment = $paymentInfo['response']['collection'];
  }
  catch ( Exception $e ) {
    watchdog('uc_mercadopago', 'IPN attempted with invalid request - @error.', array("@error" => 'exception with message "' . $e->getMessage() . '"'), WATCHDOG_ERROR);
    return;
  }

  $order_id = $payment['external_reference'];
  $order = uc_order_load($order_id);

  $link = NULL;
  if ($order != FALSE) {
    $link = l(t('view'), 'admin/store/orders/' . $order->order_id);
  }

  watchdog('uc_mercadopago', 'Receiving IPN at URL for order @order_id. <pre>@debug</pre>', array('@order_id' => $order_id, '@debug' => variable_get('uc_mercadopago_debug_ipn', FALSE) ? print_r($paymentInfo, TRUE) : ''), WATCHDOG_NOTICE, $link);

  if ($order == FALSE) {
    watchdog('uc_mercadopago', 'IPN attempted for non-existent order @order_id.', array('@order_id' => $order_id), WATCHDOG_ERROR);
    return;
  }

  $comments = array();
  $comment_line = array();
  if (isset($payment['status'])) {
    $comment_line[] = $payment['status'];
  }
  if (isset($payment['status_detail'])) {
    $comment_line[] = $payment['status_detail'];
  }
  if (count($comment_line)) {
    $comments[] = t('Status') . ': ' . implode(' - ', $comment_line);
  }
  if (isset($payment['id'])) {
    $comments[] = t('ID') . ': ' . $payment['id'];
  }
  $comment_line = array();
  if (isset($payment['payer']['first_name'])) {
    $comment_line[] = $payment['payer']['first_name'];
  }
  if (isset($payment['payer']['last_name'])) {
    $comment_line[] = $payment['payer']['last_name'];
  }
  if (count($comment_line)) {
    $comments[] = t('Payer name') . ': ' . implode(' ', $comment_line);
  }
  $comment_line = array();
  if (isset($payment['payer']['phone']['area_code'])) {
    $comment_line[] = $payment['payer']['phone']['area_code'];
  }
  if (isset($payment['payer']['phone']['number'])) {
    $comment_line[] = $payment['payer']['phone']['number'];
  }
  if (isset($payment['payer']['phone']['area_code'])) {
    $comment_line[] = 'ext. ' . $payment['payer']['phone']['extension'];
  }
  if (count($comment_line)) {
    $comments[] = t('Payer phone') . ': ' . implode(' ', $comment_line);
  }
  if (isset($payment['payer']['email'])) {
    $comments[] = t('Payer email') . ': ' . $payment['payer']['email'];
  }
  $comment_line = array();
  if (isset($payment['payer']['identification']['type'])) {
    $comment_line[] = $payment['payer']['identification']['type'];
  }
  if (isset($payment['payer']['identification']['number'])) {
    $comment_line[] = $payment['payer']['identification']['number'];
  }
  if (count($comment_line)) {
    $comments[] = t('Payer ID') . ': ' . implode(' ', $comment_line);
  }
  if (isset($payment['payer']['nickname'])) {
    $comments[] = t('Payer nickname') . ': ' . $payment['payer']['nickname'];
  }

  switch ( $payment['status']) {

    case 'approved':
      if (abs($payment['transaction_amount'] - $order->order_total) > 0.01) {
        watchdog('uc_mercadopago', 'Payment @txn_id for order @order_id did not equal the order total.', array('@txn_id' => $payment['id'], '@order_id' => $order->order_id), WATCHDOG_WARNING, l(t('view'), 'admin/store/orders/' . $order->order_id));
      }
      $comment = t('MercadoPago transaction ID: @txn_id', array('@txn_id' => $payment['id']));
      uc_payment_enter($order_id, 'mercadopago', $payment['transaction_amount'], $order->uid, NULL, $comment);
      uc_cart_complete_sale($order);
      array_unshift($comments, t('Net received amount: @amount @currency.', array('@amount' => uc_currency_format($payment['net_received_amount'], FALSE), '@currency' =>  $payment['currency_id'])));
      array_unshift($comments, t('MercadoPago IPN reported a total payment of @amount @currency.', array('@amount' => uc_currency_format($payment['transaction_amount'], FALSE), '@currency' =>  $payment['currency_id'])));
      break;

    case 'pending':
      uc_order_update_status($order_id, 'pending');
      array_unshift($comments, t('Payment is pending at MercadoPago: @reason', array('@reason' => $payment['status_detail'])));
      break;

    case 'in_process':
      // do nothing
      break;

    case 'rejected':
      array_unshift($comments, t("The customer's attempted payment from MercadoPago failed: @reason", array('@reason' => $payment['status_detail'])));
      break;

    // You, the merchant, refunded the payment.
    case 'refunded':
      array_unshift($comments, t('MercadoPago payment refunded.: @txn_id', array('@txn_id' => $payment['id'])));
      // @TODO: test
      //uc_payment_enter($order_id, 'mercadopago', $payment_amount, $order->uid, NULL, $comment);
      break;

    case 'cancelled':
      uc_order_update_status($order_id, 'canceled');
      array_unshift($comments, t('The payment has been cancelled.'));
      break;

    case 'in_mediation':
      // do nothinig
      break;

  }

  uc_order_comment_save($order->order_id, $user->uid, implode('<br/>', $comments));

}
