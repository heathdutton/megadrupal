<?php

/**
 * @file
 * RCCPagos pages.
 */


/**
 * RCCPagos error url.
 *
 * This url is called when the payment process in RCCPagos site was not
 * successful.
 */
function uc_rccpagos_error($verif, $error_code, $order) {
  // If it's a valid response (the checksum is correct) ew log the RCCPagos
  // error.
  if (_uc_rccpagos_valid_verification_code($verif, $order->order_id)) {
    $msg = _uc_rccpagos_get_error_msg($error_code);
    $params = array(
      '@code' => $error_code,
      '@error' => $msg,
      '@order_id' => $order->order_id,
    );
    uc_order_comment_save($order->order_id, 0, t('RCCPagos returned error code: @code (@error) processing order @order_id', $params));
    drupal_set_message(t('There was an error processing your payment: @error', array('@error' => $msg)), 'error');
    watchdog('uc_rccpagos', 'RCCPagos returned error code: @code (@error) processing order @order_id', $params, WATCHDOG_ERROR);
  }
  // Invalid checksum, show a generic error to the user and log the event.
  else {
    watchdog('uc_rccpagos', 'Invalid verification string: @verif for order @order_id.', array('@verif' => $verif, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    drupal_set_message('There was an error and we couldn\'t process your payment, please contact support.', 'error');
  }
  // TODO: add error redirect option to config.
  drupal_goto(variable_get('uc_rccpagos_error_redirect', 'cart/checkout/review'));
}

/**
 * RCCPagos success url.
 *
 * This url is called when the payment process in RCCPagos was successful.
 */
function uc_rccpagos_success($verif, $order) {
  // If the checksum is correct process finish the order.
  if (_uc_rccpagos_valid_verification_code($verif, $order->order_id)) {
    uc_cart_empty(uc_cart_get_id());

    // With imprimoYpago we don't actually know if the payment has been made.
    if ($order->rcc_payment_method == 'imprimo_pago') {
      uc_order_update_status($order->order_id, 'rccpagos_pending');
      uc_order_comment_save($order->order_id, 0, t('Order @id reported pending in RCCPagos (payment method is imprimoYpago).', array('@id' => $order->order_id)));
      drupal_set_message(t('Your order is pending in RCCPagos. Please contact support when you make the payment.'));
    }
    // The payment has been made so we need to update the order and enter the
    // payment.
    else {
      $methods = _uc_rccpagos_methods();
      $comment = t('Order @id paid in RCCPagos with @method', array('@id' => $order->order_id, '@method' => $methods[$order->rcc_payment_method]));
      uc_payment_enter($order->order_id, 'rccpagos', $order->order_total, 0, NULL, $comment);
      uc_cart_complete_sale($order);
      uc_order_comment_save($order->order_id, 0, $comment, 'order', 'payment_received');
    }

    // This lets us know it's a legitimate access of the complete page.
    $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
    drupal_goto('cart/checkout/complete');
  }
  else {
    watchdog('uc_rccpagos', 'Invalid verification string: @verif for order @order_id.', array('@verif' => $verif, '@order_id' => $order->order_id), WATCHDOG_ERROR);
    drupal_set_message('There was an error and we couldn\'t process your payment, please contact support.', 'error');
  }
}

/**
 * Process the error code returned by RCCPagos and returns the correct message.
 */
function _uc_rccpagos_get_error_msg($error_code) {
  // Error code to message mapping. Error codes 8 to 17 are too technical to
  // show in the page, so we default to a generic message.
  switch ($error_code) {
    case '3':
      return t('Exceeds card limit');

    case '4':
      return t('Expired card');

    case '5':
      return t('Invalid security code');

    case '6':
      return t('Invalid card');

    case '7':
      return t('The processor rejected the transaction');

    default:
      return t('Error processing the transaction');
  }
}

/**
 * Returns TRUE if the verification code is correct and FALSE otherwise.
 *
 * The verification code is md5(rccpagosVerificacion + secret_key).
 */
function _uc_rccpagos_valid_verification_code($verif, $order_id) {
  $checksum = _uc_rccpagos_get_checksum($order_id);
  $local_verif = strtolower(md5($checksum . variable_get('uc_rccpagos_secret_key', '')));
  return $local_verif == $verif;
}
