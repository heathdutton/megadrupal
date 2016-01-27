<?php

/**
 * @file
 * Supporting functions for Fat Zebra payment method for Drupal Commerce.
 */

/**
 * Makes a purchase request and returns a purchase request response object.
 *
 * @param array $request_values
 *   An associative array containing:
 *     - amount: A float value of the purchase amount i.e. 142.33
 *     - reference: A string containing a unique reference for this transaction
 *     - card_holder: A string containing the name of the card holder
 *     - card_number: A string containing the card number
 *     - card_expiry: A string containing the cvv code
 *
 * @return object
 *   An object representing the JSON response received from the Fat Zebra API
 */
function commerce_fatzebra_purchase($request_values, $settings) {

  libraries_load('fatzebra_php');

  $purchase_request = new FatZebra\PurchaseRequest(
    $request_values['amount'],
    $request_values['reference'],
    $request_values['card_holder'],
    $request_values['card_number'],
    $request_values['card_expiry'],
    $request_values['cvv']
  );

  // For any non-live setting, use true (test mode).
  $transaction_mode = ($settings['txn_mode'] == COMMERCE_FATZEBRA_TXN_MODE_LIVE) ? FALSE : TRUE;

  $gw = new FatZebra\Gateway(
    $settings['username'],
    $settings['token'],
    $transaction_mode,
    $settings['gateway']
  );
  $gw->timeout = 30;
  return $gw->purchase($purchase_request);
}

/**
 * Processes a Fat Zebra purchase response and create a transaction record.
 *
 * @param object $response
 *   An object representing JSON response from a Fat Zebra API purchase request
 *     - order_id: must be added manually in order to correctly record the
 *       transaction against an order
 *
 * @return true|string
 *   true if the purchase is successful, a string containing an error message
 *   if unsuccessful
 */
function commerce_fatzebra_purchase_response_process($response) {

  $process_result = TRUE;

  // Create a new transaction object for the current order.
  $transaction = commerce_payment_transaction_new('commerce_fatzebra', $response->order_id);

  // Set the message that will be recorded for this transaction.
  $transaction->message = 'Transaction @status: @statusdetail.';
  $transaction->message_variables = array();

  // Set the transaction status and text based on response success values.
  if ($response->successful && $response->response->successful) {
    // Our payment was successful.
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->message_variables['@status'] = 'success';
  }
  else {
    // Our payment was unsuccessful, or we received an API error.
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message_variables['@status'] = 'failed';
  }

  if ($response->successful === FALSE) {

    // We have an API error, set an error message to be displayed to the user.
    $transaction->message_variables['@statusdetail'] = implode(', ', $response->errors);
    $process_result = t("There was a problem communicating with the payment gateway. Please try again or contact @site_mail for help.", array('@site_mail' => variable_get('site_mail', '')));
    watchdog('commerce_fatzebra', 'API Error: @response', array('@response' => print_r($response, TRUE)), WATCHDOG_ERROR);

  }
  else {
    // API call was successful, so $response->response will be populated.
    $transaction->message_variables['@statusdetail'] = $response->response->message;

    $transaction->amount = $response->response->amount;
    $transaction->currency_code = $response->response->currency;
    $transaction->remote_id = $response->response->id;

    // Set the transaction message and tokens that will be recorded as a
    // transaction against this order.
    $transaction->message .= ' Card: @cardnumber, Reference: @reference';
    $transaction->message_variables['@cardnumber'] = $response->response->card_number;
    $transaction->message_variables['@reference'] = $response->response->reference;

    // If the transaction was not successful, set an error message to be
    // displayed to the user.
    if ($response->response->successful === FALSE) {
      $process_result = t("There was a problem with your transaction: @message. Please try again or contact @site_mail for help.", array(
        '@site_mail' => variable_get('site_mail', ''),
        '@message' => $response->response->message)
      );
      watchdog('commerce_fatzebra', 'Transaction Error: @response', array('@response' => print_r($response, TRUE)), WATCHDOG_ERROR);
    }

  }

  commerce_payment_transaction_save($transaction);

  return $process_result;
}
