<?php

/**
 * @file
 * Hosted payment functions.
 */

/**
 * Implements hook_redirect_form().
 */
function commerce_smartpay_form_redirect_form($form, &$form_state, $order, $payment_method) {
  // Return an error if the enabling action's settings haven't been configured.
  if (!variable_get(COMMERCE_SMARTPAY_MERCHANT_ACCOUNT)) {
    drupal_set_message(t('Smartpay hosted payment is not configured for use. !url', array('!url' => '<a href=\"/admin/commerce/config/smartpay\">Set a merchant account on the settings page.</a>')), 'error');
    return array();
  }

  $settings = array(
    // Return to the previous page when payment is canceled.
    'cancel_return' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    // Return to the payment redirect page for processing successful payments.
    'return' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    // Specify the current payment method instance ID in the notify_url.
    'payment_method' => $payment_method['instance_id'],
  );

  return commerce_smartpay_form_order_form($form, $form_state, $order, $settings);
}

/**
 * Implements hook_redirect_form_validate().
 */
function commerce_smartpay_form_redirect_form_validate($order, $payment_method) {

  $auth_result = check_plain($_GET['authResult']);
  $merchant_reference = check_plain($_GET['merchantReference']);
  // For PENDING, ERROR or CANCELLED results, pspReference is not yet known
  // and is therefore empty.
  if (empty($_GET['pspReference'])) {
    $psp_reference = '';
  }
  else {
    $psp_reference = check_plain($_GET['pspReference']);
  }
  $merchant_sig = check_plain($_GET['merchantSig']);

  $tokens = array(
    'auth_result' => $auth_result,
    'merchant_reference' => $merchant_reference,
    'psp_reference' => $psp_reference,
    'merchant_sig' => $merchant_sig,
  );

  // Default no charge for failed transactions.
  $default_charge = array('amount' => 0, 'currency_code' => '');

  // Check for a valid status callback.
  switch ($tokens['auth_result']) {
    case 'ERROR':
      watchdog('commerce_smartpay', 'ERROR result from Smartpay for order %order_id', array('%order_id' => $tokens['merchant_reference']), WATCHDOG_ALERT);
      commerce_smartpay_transaction($payment_method, $order, $default_charge, $tokens, COMMERCE_PAYMENT_STATUS_FAILURE, COMMERCE_SMARTPAY_REMOTE_STATUS_FAIL);
      commerce_payment_redirect_pane_previous_page($order);
      drupal_set_message(t('Your Smartpay transaction was aborted.'), 'error');
      return FALSE;

    case 'REFUSED':
      watchdog('commerce_smartpay', 'REFUSED result from Smartpay for order %order_id', array('%order_id' => $tokens['merchant_reference']), WATCHDOG_ALERT);
      commerce_smartpay_transaction($payment_method, $order, $default_charge, $tokens, COMMERCE_PAYMENT_STATUS_FAILURE, COMMERCE_SMARTPAY_REMOTE_STATUS_FAIL);
      commerce_payment_redirect_pane_previous_page($order);
      drupal_set_message(t('Your transaction was refused by Smartpay.'), 'error');
      return FALSE;

    case 'CANCELLED':
      watchdog('commerce_smartpay', 'CANCELLED result from Smartpay for order %order_id', array('%order_id' => $tokens['merchant_reference']), WATCHDOG_ALERT);
      commerce_smartpay_transaction($payment_method, $order, $default_charge, $tokens, COMMERCE_PAYMENT_STATUS_FAILURE, COMMERCE_SMARTPAY_REMOTE_STATUS_CANCELLED);
      commerce_payment_redirect_pane_previous_page($order);
      drupal_set_message(t('Your Smartpay transaction was cancelled.'), 'error');
      return FALSE;

    case 'AUTHORISED':
      watchdog('commerce_smartpay', 'AUTHORISED Payment callback received from Smartpay for order %order_id', array('%order_id' => $tokens['merchant_reference']));
      $remote_status = COMMERCE_SMARTPAY_REMOTE_STATUS_OK;
      break;

    case 'PENDING':
      watchdog('commerce_smartpay', 'PENDING Payment callback received from Smartpay for order %order_id', array('%order_id' => $tokens['merchant_reference']));
      $remote_status = COMMERCE_SMARTPAY_REMOTE_STATUS_PENDING;
      break;

    default:
      // Status code is anything other than those above.
      watchdog('commerce_smartpay', 'Unrecognised Status response from Smartpay for order %order_id', array('%order_id' => $tokens['merchant_reference']), WATCHDOG_ERROR);
      return FALSE;
  }

  // Validation passed, create a transaction.
  $charge = array();
  $charge['amount'] = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
  $charge['currency_code'] = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];

  commerce_smartpay_transaction($payment_method, $order, $charge, $tokens, COMMERCE_PAYMENT_STATUS_SUCCESS, $remote_status);

  return TRUE;
}

/**
 * Builds a Smartpay payment form from an order object.
 */
function commerce_smartpay_form_order_form($form, &$form_state, $order, $settings) {

  // Wrap the order for easier access to data.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  $skin_code = variable_get(COMMERCE_SMARTPAY_SKIN_CODE);
  $merchant_account = variable_get(COMMERCE_SMARTPAY_MERCHANT_ACCOUNT);

  // Get the test or live server URL and shared secret.
  if (variable_get(COMMERCE_SMARTPAY_TRANSACTION_MODE) == COMMERCE_SMARTPAY_MODE_TEST) {
    $shared_secret = variable_get(COMMERCE_SMARTPAY_SHARED_SECRET_TEST);
    $server_url = COMMERCE_SMARTPAY_SERVER_TEST;
  }
  elseif (variable_get(COMMERCE_SMARTPAY_TRANSACTION_MODE) == COMMERCE_SMARTPAY_MODE_LIVE) {
    $shared_secret = variable_get(COMMERCE_SMARTPAY_SHARED_SECRET_LIVE);
    $server_url = COMMERCE_SMARTPAY_SERVER_LIVE;
  }

  // The transaction reference you assign to the payment.
  $merchant_reference = $order->order_id;
  $payment_amount = $wrapper->commerce_order_total->amount->value();
  // 3 Digit ISO Currency Code.
  $currency_code = variable_get('commerce_default_currency', 'GBP');
  // Example: ship in 5 days.
  $ship_before_date = date("Ymd", mktime(date("H"), date("i"), date("s"), date("m"), date("j") + 5, date("Y")));
  $shopper_locale = "en_GB";
  // A description of the payment which is displayed to shoppers.
  $order_data_raw = "Payment for order #" . $order->order_id;
  // Example: shopper has one hour to complete.
  $session_validity = date(DATE_ATOM, mktime(date("H") + 2, date("i"), date("s"), date("m"), date("j"), date("Y")));
  $shopper_reference = ($order->uid != '0') ? $order->uid : '';
  $shopper_email = $order->mail;

  // GZIP and base64 encode the order_data.
  $order_data = base64_encode(gzencode($order_data_raw));
  // Concatenate all the data needed to calculate the HMAC-string.
  $hmac_data = $payment_amount . $currency_code . $ship_before_date . $merchant_reference . $skin_code
          . $merchant_account . $session_validity . $shopper_email . $shopper_reference;

  $merchant_sig = base64_encode(hash_hmac('sha1', $hmac_data, $shared_secret, TRUE));

  // Build the data array that will be translated into hidden form values.
  $data = array(
    'merchantReference' => $merchant_reference,
    'paymentAmount' => $payment_amount,
    'currencyCode' => $currency_code,
    'shipBeforeDate' => $ship_before_date,
    'skinCode' => $skin_code,
    'merchantAccount' => $merchant_account,
    'shopperLocale' => $shopper_locale,
    'orderData' => $order_data,
    'sessionValidity' => $session_validity,
    'merchantSig' => $merchant_sig,
    'shopperEmail' => $shopper_email,
    'shopperReference' => $shopper_reference,
    'resURL' => $settings['cancel_return'],
  );

  $form['#action'] = $server_url;

  foreach ($data as $name => $value) {
    if (!empty($value)) {
      $form[$name] = array('#type' => 'hidden', '#value' => $value);
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to Smartpay'),
  );

  return $form;
}
