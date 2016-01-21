<?php
/**
 * @file
 * Methods used in processing Direct Post Method transactions.
 */

/**
 * Relay page for iATS Direct Post response.
 *
 * This function creates the page that receives Direct Post payment
 * responses from the iATS API.
 *
 * Processes the response, creates a payment transaction and produces
 * the client-side JavaScript that iATS uses to redirect the user
 * back into the checkout process.
 */
function commerce_iats_direct_post_relay() {
  global $base_url;

  if (isset($_POST['IATS_Comment'])) {
    $comment = $_POST['IATS_Comment'];
  }
  elseif (isset($_POST['IATS_comment'])) {
    $comment = $_POST['IATS_comment'];
  }

  if (isset($comment)) {
    $comment_parts = explode('|', $comment);

    // Parse the order ID and payment method instance ID from the comment
    // property. Index 0 is the comment prefix, which is not needed here.
    $order_id = $comment_parts[1];
    $payment_method_instance_id = $comment_parts[2] . '|' . $comment_parts[3];

    $success = commerce_iats_direct_post_process_response($_POST, $order_id, $payment_method_instance_id);

    $target_url = $base_url . '/checkout/' . $order_id;
    // Send the user to the appropriate checkout page,
    // based on transaction success.
    $target_url .= ($success) ? '/complete' : '/review';
  }
  else {
    $target_url = $base_url . '/checkout';
  }

  $markup = "<html>\n"
    . "<head>\n"
    . "<script type='text/javascript' charset='utf-8'>\n"
    . "window.location = '{$target_url}';\n"
    . "</script>\n"
    . "</head>\n"
    . "</html>\n";

  echo $markup;
  exit();
}

/**
 * Processes a Direct Post response.
 *
 * Creates payment transaction and completes order on payment success.
 *
 * @param array $response
 *   Associative array as described below:
 *
 * $response['IATS_AccountNumber'] = '2220';
 * $response['IATS_MOP'] = 'VISA';
 * $response['IATS_PaymentType'] = 'CreditCard';
 * $response['IATS_Result'] = 'OK: 678594:';
 * $response['IATS_TransID'] = 'A70FC33C';
 * $response['IATS_Address'] = '123 Street Name';
 * $response['IATS_City'] = 'City';
 * $response['IATS_Comment'] = '';
 * $response['IATS_Country'] = 'US';
 * $response['IATS_EMail'] = '';
 * $response['IATS_FirstName'] = 'First Name';
 * $response['IATS_LastName'] = 'Last Name';
 * $response['IATS_Phone'] = '';
 * $response['IATS_Fax'] = '';
 * $response['IATS_Phone2'] = '';
 * $response['IATS_State'] = 'State';
 * $response['IATS_ZipCode'] = '00000';
 * $response['IATS_Amount'] = '10.00';
 * $response['IATS_Payment_AgencyCodeHash'] = '984816FD329622876E14907634264E6F332E9FB3';
 * $response['IATS_ExpiryMonth'] = '04';
 * $response['IATS_ExpiryYear'] = '2016';
 * $response['IATS_Payment_SubCode'] = '88';
 *
 * @param int $order_id
 *   The ID of the order being processed.
 * @param string $payment_method_instance_id
 *   The instance ID of the payment method used in this transaction.
 *
 * @return bool
 *   TRUE if the transaction was successful.
 */
function commerce_iats_direct_post_process_response($response, $order_id, $payment_method_instance_id) {
  if (!empty($response['IATS_Result'])) {
    $order = commerce_order_load($order_id);

    $currency_code = $order->commerce_order_total['und'][0]['currency_code'];

    $payment_method = commerce_payment_method_instance_load($payment_method_instance_id);

    if ($payment_method['settings']['log']['response'] === 'response') {
      commerce_iats_log_response($payment_method, $response);
    }

    $success = FALSE;
    $result = $response['IATS_Result'];
    $transaction_id = (isset($response['IATS_TransID'])) ? $response['IATS_TransID'] : NULL;
    $customer_code = ($result == COMMERCE_IATS_TOKEN_CREATED) ? $_POST['IATS_ResultDetail'] : NULL;

    // Creating a recurring payment does not result in a transaction ID.
    // Add custom transaction ID here for reference.
    if (($transaction_id == NULL)
      && (isset($payment_method['settings']['allow_recurring'])
        && $payment_method['settings']['allow_recurring'] == 1)) {
      $transaction_id = COMMERCE_IATS_RECURRING_PAYMENT_TRANSACTION_ID;
    }

    // Build the remote ID from the transaction ID and customer code.
    $remote_id = $transaction_id;
    if ($customer_code != NULL) {
      $remote_id .= '|' . $customer_code;
    }

    $amount = commerce_currency_decimal_to_amount($response['IATS_Amount'], $currency_code);

    // Prepare a transaction object to log the API response.
    $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order_id);
    $transaction->uid = $order->uid;
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->remote_id = $remote_id;
    $transaction->amount = $amount;
    $transaction->currency_code = $currency_code;

    // Check for success or failure result.
    if (($result == COMMERCE_IATS_TOKEN_CREATED) || (substr($result, 0, 2) == COMMERCE_IATS_TXN_OK)) {
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $message = array('Transaction successful.');
      if (!is_null($customer_code)) {
        $message[] = 'Customer code created: ' . $customer_code;
      }
      if ($payment_method['method_id'] == 'iats_ach_eft') {
        $message[] = 'ACH/EFT payment pending.';
      }

      $success = TRUE;
    }
    else {
      // Create a failed transaction with the error message.
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $message = array('Transaction failed.');
    }

    $transaction->message = implode('<br />', $message);

    // Save the transaction information.
    commerce_payment_transaction_save($transaction);

    // If the payment failed, display an error and rebuild the form.
    if (!$success) {
      if ($result == COMMERCE_IATS_TOKEN_FAILED) {
        $result_message = $_POST['IATS_ResultDetail'];
      }
      else {
        $result_message = $result;
      }

      $order->data['iats_error'] = t('We received the following error while attempting to process your payment: @reason',
        array('@reason' => $result_message));

      commerce_order_save($order);
      return FALSE;
    }

    commerce_checkout_complete($order);

    // Store card on file if a customer code is returned.
    if ($success && ($customer_code != NULL)) {
      commerce_iats_direct_post_create_cardonfile($response, $order, $payment_method, $customer_code);
    }

    return TRUE;
  }
  return FALSE;
}

/**
 * Creates a card on file record using data from a Direct Post transaction.
 *
 * @param array $response
 *   The Direct Post response.
 * @param object $order
 *   The order being processed.
 * @param string $payment_method
 *   The payment method used in this transaction.
 * @param string $customer_code
 *   The iATS customer code created by this transaction.
 */
function commerce_iats_direct_post_create_cardonfile($response, $order, $payment_method, $customer_code) {
  if (function_exists('commerce_iats_direct_post_create_cardonfile')) {
    $card_data = array(
      'number' => $response['IATS_AccountNumber'],
      'exp_month' => $response['IATS_ExpiryMonth'],
      'exp_year' => $response['IATS_ExpiryYear'],
      'status' => 1,
      'default' => 0,
    );

    commerce_iats_cardonfile_create_cardonfile($order, $payment_method,
      $card_data, $customer_code, strtolower($response['IATS_PaymentMethod']));
  }
}
