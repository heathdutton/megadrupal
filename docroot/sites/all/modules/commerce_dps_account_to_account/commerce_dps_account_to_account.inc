<?php

/**
 * @file
 * DPS lib functions.
 */

/**
 * Generate request; if valid, return URL to redirect to.
 */
function commerce_dps_account_to_account_generate_request($transaction) {
  // Ensure amount is dd.cc
  $transaction['AmountInput'] = number_format($transaction['AmountInput'], 2, '.', '');

  $xml = commerce_dps_account_to_account_generate_xml('<GenerateRequest/>', $transaction);

  if ($transaction['log']) {
    watchdog('commerce_dps_account_to_account', 'GenerateRequest: @reference, @xml', array('@reference' => $transaction['MerchantReference'], '@xml' => $xml), WATCHDOG_DEBUG);
  }

  $url = $transaction['server'];
  $params = array('method' => 'POST', 'data' => $xml);
  $response = drupal_http_request($url, $params);

  if ($transaction['log']) {
    watchdog(
      'commerce_dps_account_to_account',
      'GenerateRequest response: @reference, HTTP @code, @xml',
      array(
        '@reference' => $transaction['MerchantReference'],
        '@code' => $response->code,
        '@xml' => isset($response->data) ? $response->data : '',
      ),
      WATCHDOG_DEBUG
    );
  }

  // If response from PX Pay is ok, extract and return
  // Hosted Payment Page URI to redirect user to.
  if ($response->code == 200) {
    if ($xml = simplexml_load_string($response->data)) {
      foreach ($xml->attributes() as $attribute => $value) {
        if ($attribute == 'valid' && $value == '1') {
          return $xml->URI;
        }
        else {
          watchdog('commerce_dps_account_to_account',
            'GenerateRequest invalid: @reference, XML: @xml',
            array(
              '@reference' => $transaction['MerchantReference'],
              '@xml' => isset($response->data) ? $response->data : '',
            ),
            WATCHDOG_ERROR
          );
        }
      }
    }
  }

  watchdog(
    'commerce_dps_account_to_account',
    'GenerateRequest: @reference, HTTP @code, XML: @xml',
    array(
      '@reference' => $transaction['MerchantReference'],
      '@code' => $response->code,
      '@xml' => isset($response->data) ? $response->data : '',
    ),
    WATCHDOG_ERROR
  );
  return FALSE;
}

/**
 * Generate XML for PX Pay Process Response.
 *
 * @param array $transaction
 *   Details of transaction.
 *
 * @return array
 *   Transaction data
 */
function commerce_dps_account_to_account_process_response($transaction) {
  $transaction['Response'] = $transaction['result'];
  $xml = commerce_dps_account_to_account_generate_xml('<ProcessResponse/>', $transaction);
  $url = variable_get('commerce_dps_account_to_account_server', COMMERCE_DPS_ACCOUNT_TO_ACCOUNT_PXPAY_SERVER);
  $response = drupal_http_request($url, array('method' => 'POST', 'data' => $xml));

  if ($transaction['log']) {
    watchdog('commerce_dps_account_to_account', 'ProcessResponse response: HTTP @code, @xml', array('@code' => $response->code, '@xml' => $response->data), WATCHDOG_DEBUG);
  }

  // If response XML from PX Pay is good, extract and return transaction data.
  if ($response->code == 200) {
    $xml = simplexml_load_string($response->data);
    // Build data array.
    foreach ($xml->attributes() as $attribute => $value) {
      if ($attribute == 'valid' && $value == '1') {
        $data = array();
        // Iterate over (flat) XML elements and gather values.
        foreach ($xml->children() as $child) {
          $data[$child->getName()] = (string) $child;
        }
        return $data;
      }
      else {
        // @TODO: Send to watchdog.
      }
    }
  }
  return FALSE;
}

/**
 * Returns an array of possible currency codes.
 */
function commerce_dps_account_to_account_currencies() {
  return array(
    'CAD' => 'Canadian Dollar',
    'CHF' => 'Swiss Franc',
    'EUR' => 'Euro',
    'FRF' => 'French Franc',
    'GBP' => 'United Kingdom Pound',
    'HKD' => 'Hong Kong Dollar',
    'JPY' => 'Japanese Yen',
    'NZD' => 'New Zealand Dollar',
    'SGD' => 'Singapore Dollar',
    'USD' => 'United States Dollar',
    'ZAR' => 'Rand',
    'AUD' => 'Australian Dollar',
    'WST' => 'Samoan Tala',
    'VUV' => 'Vanuatu Vatu',
    'TOP' => 'Tongan Pa\'anga',
    'SBD' => 'Solomon Islands Dollar',
    'PNG' => 'Papua New Guinea Kina',
    'MYR' => 'Malaysian Ringgit',
    'KWD' => 'Kuwaiti Dinar',
    'FJD' => 'Fiji Dollar',
  );
}

/**
 * Create a transaction and associate it with an order.
 *
 * @param object $order
 *   The order object being processed.
 * @param array  $payment
 *   Payment details from DPS.
 * @param string $status
 *   The payment status to use for the transaction.
 */
function commerce_dps_account_to_account_order_transaction($order, $payment, $status) {
  // Create a new payment transaction for the order.
  $transaction = commerce_payment_transaction_new('commerce_dps_account_to_account', $order->order_id);
  $transaction->instance_id = $order->data['payment_method'];
  $transaction->remote_id = $payment['TxnId'];
  $transaction->amount = commerce_currency_decimal_to_amount($payment['AmountSettlement'], $payment['CurrencyInput']);
  $transaction->currency_code = $payment['CurrencyInput'];
  $transaction->payload[REQUEST_TIME] = $payment;
  $transaction->uid = $order->uid;

  // Set the transaction's statuses based on the payment_status.
  $transaction->remote_status = $payment['ResponseText'];
  $transaction->status = $status;
  $transaction->message = 'Status @status, @statusdetail. Email: @email. Auth Code=@authcode.';

  $transaction->message_variables = array(
    '@status' => $payment['Success'],
    '@statusdetail' => $payment['ResponseText'],
    '@email' => $payment['EmailAddress'],
    '@authcode' => $payment['AuthCode'],
  );

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);
}

/**
 * Get all the transaction remote ids for a given order number.
 *
 * @param int $order_id
 *   The order id the check against.
 *
 * @return array
 *   This array contains all the the remote_ids
 */
function commerce_dps_account_to_account_get_remote_ids($order_id) {
  $transaction_ids = commerce_dps_account_to_account_get_transaction_ids($order_id);
  $transactions = commerce_payment_transaction_load_multiple($transaction_ids);
  $remote_ids = array();

  foreach ($transactions as $transaction) {
    $remote_ids[] = $transaction->remote_id;
  }

  return $remote_ids;
}

/**
 * Get all the transaction ids for a given order number.
 *
 * @param int $order_id
 *   The order id the check against.
 *
 * @return array
 *   This array contains all the transaction ids for the order
 */
function commerce_dps_account_to_account_get_transaction_ids($order_id) {
  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'commerce_payment_transaction')
    ->propertyCondition('order_id', $order_id, '=');

  $transactions = $query->execute();
  $transaction_ids = array();

  if (array_key_exists('commerce_payment_transaction', $transactions)) {
    foreach ($transactions['commerce_payment_transaction'] as $transaction) {
      $transaction_ids[] = $transaction->transaction_id;
    }
  }

  return $transaction_ids;
}

/**
 * Generate an xml string from an array.
 *
 * @param string $parent_element
 *   The main wrapper element.
 * @param array $data
 *   The data array to convert.
 *
 * @return string
 *   The converted xml string
 */
function commerce_dps_account_to_account_generate_xml($parent_element, $data) {
  $result = '';
  if (is_array($data) AND !empty($data)) {
    $xml = new SimpleXMLElement($parent_element);
    foreach ($data as $key => $value) {
      $xml->addChild($key, $value);
    }
    $result = $xml->asXML();
  }
  return $result;
}
