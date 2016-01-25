<?php

/**
 * @file
 * Callbacks used by hook_commerce_payment_method_info().
 */

/**
 * Set up the Blockchain Bitcoin payment method settings form.
 */
function commerce_blockchain_settings_form($settings = array(), $payment_method = NULL) {
  global $base_url;

  /*
  The callback URL is limited to 255 characters, so this can't be an insane
  length because it gets added to the callback.
  */
  if (empty($settings['secret'])) {
    $settings['secret'] = _commerce_blockchain_generate_secret(microtime() . mt_rand(), $base_url);
  }

  $settings += array(
    'payment_hash' => '1GHMVzFKQBN4CBZuxRC3bVaV8DxTmEPEnB',
    'blockchain_mixer' => 'false',
    'confirmations' => 6,
    'callback_url' => $base_url . '/blockchain/callback/' . $settings['secret'],
  );

  $form['payment_hash'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Bitcoin payment address'),
    '#default_value' => $settings['payment_hash'],
    '#description' => t('Bitcoin address for receiving payments.'),
  );
  $form['confirmations'] = array(
    '#type' => 'select',
    '#title' => t('Blockchain confirmation threshold'),
    '#default_value' => $settings['confirmations'],
    '#options' => drupal_map_assoc(array(0, 1, 2, 3, 4, 5, 6)),
    '#description' => t('Minimum number of blockchain confirmations before a transaction switches from pending to complete.'),
  );
  $form['blockchain_mixer'] = array(
    '#type' => 'radios',
    '#title' => t('Anonymize Bitcoin payment through blockchain mixer'),
    '#default_value' => $settings['blockchain_mixer'],
    '#options' => array('true' => t('Yes'), 'false' => t('No')),
  );
  /*
  Intentionally make secret hard to change. Payment confirmation callbacks will
  happen for hundreds of confirmations. Changing this would cause 403 and 404
  errors in the logs.
  */
  $form['secret'] = array(
    '#type' => 'value',
    '#value' => $settings['secret'],
  );
  $form['callback_url'] = array(
    '#type' => 'value',
    '#value' => $settings['callback_url'],
  );

  return $form;
}

/**
 * Payment method callback: submit form.
 */
function commerce_blockchain_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $transaction_info = db_query("SELECT transaction_id FROM {commerce_payment_transaction} WHERE payment_method = 'blockchain' AND order_id = :id", array(':id' => $order->order_id))->fetchAssoc();

  // Generating a bunch of invoice IDs could get the site banned by Blockchain.
  if (!$transaction_info['transaction_id']) {
    $wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_total = $wrapper->commerce_order_total->value();
    $total = commerce_currency_amount_to_decimal($wrapper->commerce_order_total->amount->value(), $order_total['currency_code']);

    $amount = file_get_contents("https://blockchain.info/tobtc?currency=" . $order_total['currency_code'] . "&value=" . $total);
    if (is_numeric($amount)) {
      // https://blockchain.info/api/api_receive requires minimum of 0.001.
      if ($amount < 0.001) {
        $amount = 0.001;
      }
    }
    else {
      watchdog('commerce_blockchain', "Currency conversion didn't return a number: @amount", array('@amount' => $amount), WATCHDOG_ERROR);
    }

    $transaction = commerce_payment_transaction_new('blockchain', $order->order_id);

    $root_url = 'https://blockchain.info/api/receive';

    $parameters = 'method=create&address=' . $payment_method['settings']['payment_hash'] .'&shared='. $payment_method['settings']['blockchain_mixer'] . '&callback='. urlencode($payment_method['settings']['callback_url']) . '/' . $order->order_id;

    $response = file_get_contents($root_url . '?' . $parameters);

    $object = json_decode($response);

    $transaction->instance_id = 'blockchain|commerce_payment_blockchain';
    $transaction->remote_id = $object->input_address;
    $transaction->amount = commerce_currency_decimal_to_amount($amount, 'BTC');
    $transaction->currency_code = 'BTC';
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->message = 'Payment requested during checkout';
    $transaction->message_variables = array();

    commerce_payment_transaction_save($transaction);
  }
  else {
    $transaction = commerce_payment_transaction_load($transaction_info['transaction_id']);
    $amount = commerce_currency_amount_to_decimal($transaction->amount, 'BTC');
  }

  $form['bitcoin_payment_address'] = array(
    '#type' => 'container',
    '#prefix' => t('Send @amount BTC to: @address', array('@amount' => $amount, '@address' => $transaction->remote_id)),
    '#suffix' => t('Please wait a few seconds for your payment to be seen by blockchain.info before proceeding.', array('@amount' => $amount, '@address' => $transaction->remote_id)),
  );
  $form['transaction'] = array(
    '#type' => 'value',
    '#value' => $transaction,
  );

  return $form;
}

/**
 * Payment method callback: submit form validation.
 *
 * Validates data inputted via the payment details form elements and returns
 * TRUE or FALSE indicating whether or not all the data passed validation.
 */
function commerce_blockchain_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  $transaction = commerce_payment_transaction_load($pane_values['transaction']->transaction_id);
  if ($transaction->remote_status === '') {
    form_set_error('bitcoin_payment_address', t('The blockchain has not yet seen your payment. Please wait a few seconds before attempting to proceed again.'));
    return FALSE;
  }
}

/**
 * Payment method callback: submit form submission.
 *
 * Processes payment as necessary using data inputted via the payment details
 * form elements on the form, resulting in the creation of a payment transaction.
 */
function commerce_blockchain_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
}

/**
 * Borrowed from the BitPay library to generate a callback secret.
 */
function _commerce_blockchain_generate_secret($data, $key) {
  $hmac = base64_encode(hash_hmac('sha256', $data, $key, TRUE));
  return strtr($hmac, array('+' => '-', '/' => '_', '=' => ''));
}


/**
 * Interpret updates from blockchain.info; update relevant payment transactions.
 */
function commerce_blockchain_verify_callback($order) {
  watchdog('commerce_blockchain', 'Callback: @serial', array('@serial' => serialize($_GET)), WATCHDOG_DEBUG);

  $transaction_hash = $_GET['transaction_hash'];
  $input_transaction_hash = $_GET['input_transaction_hash'];
  $value_in_satoshi = $_GET['value'];

  if (!empty($_GET['test'])) {
    return;
  }

  $payment_method = commerce_payment_method_instance_load('blockchain|commerce_payment_blockchain');

  if ($_GET['destination_address'] == $payment_method['settings']['payment_hash']) {

    $transaction_id = db_query("SELECT transaction_id FROM {commerce_payment_transaction} WHERE order_id=:id AND remote_id=:remote", array(':id' => $order->order_id, ':remote' => $_GET['input_address']))->fetchField();
    $transaction = commerce_payment_transaction_load($transaction_id);

    if ($_GET['confirmations'] >= $payment_method['settings']['confirmations']) {
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = t('Payment confirmed.');
    }
    else {
      $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
      $transaction->message = t("Paid, waiting for blockchain confirmation.");
    }
    /*
    The customer could have paid anything, exact, above, or below requested.
    Record what was paid and let the order module handle reporting a pending
    balance instead of worrying about it here.
    */
    $transaction->amount = $_GET['value'];
    $transaction->remote_status = t('@confirmations confirmations', array('@confirmations' => $_GET['confirmations']));

    commerce_payment_transaction_save($transaction);
  }
  else {
    watchdog('commerce_blockchain', 'Callback destination hash @hash did not match the configured destination.', array('@hash' => $_GET['destination_address']), WATCHDOG_WARNING);
  }
}
