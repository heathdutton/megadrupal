<?php

/**
 * @file
 * Menu items.
 */

/**
 * Finalizes W1 transaction. (W1 API).
 *
 * @param int $cart_id
 *   Users cart id. It needed to clear users cart after success payment.
 *
 * @see uc_w1_menu()
 *
 * @link http://www.walletone.com/ru/merchant/documentation/
 */
function uc_w1_payment_check($cart_id = 0) {
  if (empty($_POST)) {
    watchdog('uc_w1', 'Post data is empty', array(), WATCHDOG_ERROR);
    return;
  }
  if (!isset($_POST['WMI_SIGNATURE'])) {
    watchdog('uc_w1', 'NO WMI_SIGNATURE IN POST', array(), WATCHDOG_ERROR);
    uc_w1_print_answer('Retry', 'NO WMI_SIGNATURE IN POST');
  }
  if (!isset($_POST['WMI_PAYMENT_NO'])) {
    watchdog('uc_w1', 'NO WMI_PAYMENT_NO IN POST', array(), WATCHDOG_ERROR);
    uc_w1_print_answer('Retry', 'NO WMI_PAYMENT_NO IN POST');
  }
  if (!isset($_POST['WMI_ORDER_STATE'])) {
    watchdog('uc_w1', 'NO WMI_ORDER_STATE IN POST', array(), WATCHDOG_ERROR);
    uc_w1_print_answer('Retry', 'NO WMI_ORDER_STATE IN POST');
  }
  // If WMI_SIGNATURE, WMI_PAYMENT_NO, WMI_ORDER_STATE isset in $_POST then...
  $signature_post = check_plain($_POST['WMI_SIGNATURE']);
  $order_id = intval(check_plain($_POST['WMI_PAYMENT_NO']));
  $params = $_POST;
  unset($params['WMI_SIGNATURE']);
  uksort($params, 'strcasecmp');
  $values = implode('', $params);
  $order = uc_order_load($order_id);
  $signature = (variable_get('uc_w1_signature_type') == 'MD5') ? base64_encode(pack('H*', md5($values . variable_get('uc_w1_secret_key', '')))) : base64_encode(pack('H*', sha1($values . variable_get('uc_w1_secret_key', ''))));
  if ($signature_post == $signature && variable_get('uc_w1_merchant_id') == $params['WMI_MERCHANT_ID']) {
    if (drupal_strtoupper(check_plain($_POST['WMI_ORDER_STATE'])) == 'ACCEPTED') {
      $description = t('A payment from Wallet One was accepted.');
      uc_payment_enter($order_id, 'uc_w1', check_plain($_POST['WMI_PAYMENT_AMOUNT']), 0, NULL, $description);
      uc_cart_complete_sale($order, variable_get('uc_new_customer_login', FALSE));
      uc_order_comment_save($order_id, 0, $description, 'admin');
      uc_cart_empty($cart_id);
      watchdog('uc_w1', $description, array(), WATCHDOG_INFO);
      uc_w1_print_answer('Ok', t('Order#') . ' ' . check_plain($_POST["WMI_PAYMENT_NO"]) . ' ' . t('paid!'));
    }
    else {
      watchdog('uc_w1', 'Bad order state', array(), WATCHDOG_ERROR);
      uc_w1_print_answer('Retry', t('Bad order state') . ' ' . check_plain($_POST['WMI_ORDER_STATE']));
    }
  }
  else {
    watchdog('uc_w1', 'Bad signature', array(), WATCHDOG_ERROR);
    uc_w1_print_answer('Retry', t('Bad signature') . '' . check_plain($_POST['WMI_SIGNATURE']));
  }
}

/**
 * W1 answer. (W1 API).
 *
 * @param string $result
 *   Result command for wallet one API. Ok or Retry.
 * @param string $description
 *   Description for wallet one.
 *
 * @see uc_w1_payment_check()
 *
 * @link http://www.walletone.com/ru/merchant/documentation/
 */
function uc_w1_print_answer($result, $description) {
  print 'WMI_RESULT=' . drupal_strtoupper($result) . "&";
  print 'WMI_DESCRIPTION=' . urlencode($description);

  drupal_exit();
}

/**
 * Redirect customer to checkout complete page or cart page.
 *
 * @param string $type
 *   If page argument - success go to complete page.
 *
 * @see uc_w1_menu()
 */
function uc_w1_payment_end($type) {
  if ($type == 'success') {
    if (isset($_SESSION['cart_order'])) {
      $_SESSION['uc_checkout'][$_SESSION['cart_order']]['do_complete'] = TRUE;
      drupal_goto('cart/checkout/complete');
    }
    else {
      watchdog('uc_w1', 'Your session has been expired.', array(), WATCHDOG_ERROR);
      drupal_set_message(t('Your session has been expired.'));
      drupal_goto('cart');
    }
  }
  else {
    watchdog('uc_w1', 'Your payment has been declined.', array(), WATCHDOG_ERROR);
    drupal_set_message(t('Your payment has been declined.'));
    drupal_goto('cart');
  }
}
