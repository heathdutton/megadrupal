<?php

/**
 * @file
 * Callbacks for the uc_pnag module.
 */

/**
 * Menu callback to take action if the customer gets redirected back from SOFORT
 * Banking or if a notification from the SOFORT API was received.
 *
 * The request gets validated with a combination of tools: if the callback is
 * a notification, the transaction ID gets extracted from the request and checked
 * against the database, if that transaction ID is valid together with the given
 * token from the URL query. In all other cases, the validation also takes the
 * order ID into consideration which is available in the session cookie of the
 * browser that just got redirected to the shop.
 *
 * If the request got validated successfully, the request gets executed and
 * the payment status gets changed accordingly.
 *
 * @param $mode
 *   The values 'success', 'cancel', 'timeout' and 'notify' are supported.
 * @return mixed
 *   Sends back a http_code in the HTTP result header depending on the success
 *   of this callback and redirects the customer to the appropriate page in the
 *   shop.
 */
function uc_pnag_callback($mode) {
  if ($mode == 'notify') {
    $sofort = new SofortLib_Notification();
    uc_pnag_setup_logger($sofort);
    list($order_id, $transaction_id) = _uc_pnag_get_ids($sofort->getNotification());
    if (empty($order_id)) {
      watchdog('SOFORT', 'Illegal notification received.', array(), WATCHDOG_CRITICAL);
      $result = 500;
    }
    else {
      _uc_pnag_check_status($order_id, $transaction_id);
      $result = 200;
    }
    header('http_code: ' . $result, TRUE, $result);
    print $result;
    exit;
  }

  if (!drupal_valid_token(check_plain($_GET['cart_token']))) {
    watchdog('SOFORT', 'Invalid callback. Token does not match.', array(), WATCHDOG_ALERT);
    drupal_goto('cart/checkout/review');
  }

  list($order_id, $transaction_id) = _uc_pnag_get_ids();
  if (empty($transaction_id)) {
    watchdog('SOFORT', 'Invalid parameters in callback.', array(), WATCHDOG_CRITICAL);
    drupal_goto('cart/checkout/review');
  }
  _uc_pnag_check_status($order_id, $transaction_id);

  switch ($mode) {
    case 'success':
      drupal_set_message(sofortbanking_strings(1101, array('!transactionid' => $transaction_id)));
      $_SESSION['uc_checkout'][$order_id]['do_complete'] = TRUE;
      drupal_goto('cart/checkout/complete');
      break;

    case 'cancel':
      drupal_set_message(sofortbanking_strings(1201, array('!transactionid' => $transaction_id)), 'warning');
      drupal_goto('cart/checkout/review');
      break;

    case 'timeout':
      drupal_set_message(sofortbanking_strings(1202, array('!transactionid' => $transaction_id)), 'warning');
      drupal_goto('cart/checkout/review');
      break;

    default:
      drupal_goto('cart/checkout/review');

  }
}

/**
 * Helper function called by uc_pnag_callback() to determine the latest status
 * of the payment transaction at SOFORT Banking and take appropriate action in
 * the shop repending on the previous and current status of the transaction.
 *
 * This function makes sure, that if the transaction status hasn't changed,
 * that no changes will be made to the database records of the shop. In other
 * words, if multiple notifications with the same status got submitted to the
 * shop, they would *not* result in multiple database records.
 *
 * As it may happen, that the success redirect and the notification get fired
 * at the same or similar time, this function takes care of multi-threading
 * issues and executes only one request after the other.
 *
 * @param integer $order_id
 *   The order ID of the Ubercart shop.
 * @param string $transaction_id
 *   The transaction ID from SOFORT banking.
 */
function _uc_pnag_check_status($order_id, $transaction_id) {
  $sofort = new SofortLib_TransactionData(variable_get('uc_pnag_config_key', ''));
  uc_pnag_setup_logger($sofort);
  $sofort->setTransaction($transaction_id);
  $sofort->sendRequest();
  if ($sofort->isError()) {
    watchdog('SOFORT', 'Transaction status not available.', array(), WATCHDOG_WARNING);
    return;
  }

  $lock_key = 'uc_pnag_status_' . $transaction_id;
  while (!lock_acquire($lock_key)) {
    sleep(1);
  }

  $status = $sofort->getStatus();
  $payments = uc_payment_load_payments($order_id);
  if ($payments) {
    foreach ($payments as $payment) {
      if (!empty($payment->data) && is_string($payment->data)) {
        $payment->data = unserialize($payment->data);
      }
      if (!empty($payment->data['uc_pnag_transaction_id'])) {
        if ($payment->data['uc_pnag_transaction_id'] == $transaction_id) {
          if ($payment->data['uc_pnag_transaction_status'] == $status) {
            // Nothing to do, this payment receipt has already been captured.
            lock_release($lock_key);
            return;
          }
        }
      }
    }
  }

  $amount = $sofort->getAmount();
  $comment = FALSE;
  $order_status = FALSE;
  $order_completed = FALSE;
  switch ($status) {
    case 'untraceable':
      $comment = sofortbanking_strings(1802, array('!transactionid' => $transaction_id));
      $order_status = variable_get('uc_pnag_order_status_untraceable', 'payment_received');
      $order_completed = TRUE;
      break;

    case 'pending':
      $comment = sofortbanking_strings(1805, array('!transactionid' => $transaction_id));
      $order_status = variable_get('uc_pnag_order_status_pending', 'payment_received');
      $order_completed = TRUE;
      break;

    case 'received':
      $comment = sofortbanking_strings(1803, array('!transactionid' => $transaction_id));
      $order_status = variable_get('uc_pnag_order_status_payment_received', 'payment_received');
      $order_completed = TRUE;
      break;

    case 'loss':
      $comment = sofortbanking_strings(1804, array('!transactionid' => $transaction_id));
      $order_status = variable_get('uc_pnag_order_status_loss', 'pending');
      break;

    case 'refunded':
      $comment = sofortbanking_strings(1806, array('!transactionid' => $transaction_id));
      $order_status = variable_get('uc_pnag_order_status_refunded', 'payment_received');
      $order_completed = TRUE;
      break;

    default:
      $amount = 0;
      $comment = sofortbanking_strings(1804, array('!transactionid' => $transaction_id));
      break;

  }

  if ($comment) {
    uc_order_comment_save($order_id, 0, $comment);
  }

  if ($amount) {
    $data = array('uc_pnag_transaction_id' => $transaction_id, 'uc_pnag_transaction_status' => $status);
    $payment_comment = empty($comment) ? '' : $comment;
    uc_payment_enter($order_id, UC_PNAG_PAYMENT_METHOD_ID, $amount, 0, $data, $payment_comment);
  }

  if ($order_status) {
    uc_order_update_status($order_id, $order_status);
  }

  if ($order_completed) {
    $order = uc_order_load($order_id);
    uc_cart_complete_sale($order);
  }
  lock_release($lock_key);
}

/**
 * Helper function to validate a notification or redirect from SOFORT Banking.
 *
 * @param string|NULL $transaction_id
 * @return array
 */
function _uc_pnag_get_ids($transaction_id = NULL) {
  $query = db_select('uc_pnag_orders', 'o')
    ->fields('o', array('order_id', 'transaction_id'))
    ->condition('o.token', $_GET['cart_token']);
  if (empty($transaction_id)) {
    $query->condition('o.order_id', $_SESSION['cart_order']);
  }
  else {
    $query->condition('o.transaction_id', $transaction_id);
  }
  $result = $query->execute()->fetch();
  if ($result) {
    return array_values((array) $result);
  }
  return array(FALSE, FALSE);
}
