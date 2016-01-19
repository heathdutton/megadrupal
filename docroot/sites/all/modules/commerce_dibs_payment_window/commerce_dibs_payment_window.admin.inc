<?php

/**
 * @file
 * Admin page callbacks.
 */

/**
 * Page callback: Called by DIBS when an customer pays.
 */
function commerce_dibs_payment_window_callback($order, $transaction) {

  // Store each callback request attempt to log.
  watchdog('commerce_dibs_payment_window', 'Callback: <pre>%pp</pre><pre>%op</pre><pre>%tp</pre>',
    array(
      '%pp' => print_r($_POST, TRUE),
      '%op' => print_r($order, TRUE),
      '%tp' => print_r($transaction, TRUE),
    )
  );

  try {

    if (empty($_POST)) {
      throw new Exception('DIBS Callback, no data provided from DIBS.');
    }
    $response = $_POST;

    // Save response data for later.
    $transaction->data['commerce_dibs_payment_window']['responses'][REQUEST_TIME] = $response;

    // Load inc.
    module_load_include('inc', 'commerce_dibs_payment_window', 'commerce_dibs_payment_window');

    // Load payment method instance.
    $payment_method = commerce_payment_method_instance_load($transaction->instance_id);
    if (empty($payment_method) || $payment_method['method_id'] !== 'commerce_dibs_payment_window') {
      throw new Exception('DIBS Callback, failed to load payment method. Aborted.');
    }

    // Load settings with default settings fallback.
    $settings = array_replace_recursive(_commerce_dibs_payment_window_get_settings_default(), $payment_method['settings']);

    // Skip next steps if transaction has been processed by form_validation
    // already.
    if (isset($response['captureStatus']) && $transaction->remote_status === COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_CAPTURED && $transaction->status != COMMERCE_DIBS_PAYMENT_WINDOW_STATUS_UNDEFINED) {
      return MENU_NOT_FOUND;
    }
    elseif (!isset($response['captureStatus']) && $transaction->remote_status === COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_AUTHORIZED && $transaction->status != COMMERCE_DIBS_PAYMENT_WINDOW_STATUS_UNDEFINED) {
      return MENU_NOT_FOUND;
    }

    // Check for MAC from DIBS.
    if (isset($response['MAC']) && _commerce_dibs_payment_window_should_add_mac_to_data($settings)) {
      $provided_mac = $response['MAC'];

      // Remove mac from response.
      unset($response['MAC']);
      $local_mac = _commerce_dibs_payment_window_create_hmac_from_data($response, $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac']);

      if ($local_mac !== $provided_mac) {
        throw new Exception(format_string(
          'DIBS Callback, local MAC and provided MAC does not match for order: %order_id and transaction: %transaction_id. This might be a sign of tampering! ',
          array(
            '%order_id' => $order->order_id,
            '%transaction_id' => $transaction->transaction_id,
          )
        ));
      }
    }
  }
  catch (Exception $e) {
    watchdog_exception('commerce_dibs_payment_window', $e);
    return MENU_NOT_FOUND;
  }

  try {
    $checks = array(
      's_localTransactionID' => $transaction->transaction_id,
      'orderId' => $order->order_id,
      'merchant' => $settings['commerce_dibs_payment_window_merchant_id'],
      'transaction' => NULL,
      'amount' => $transaction->amount,
    );
    _commerce_dibs_payment_window_validate_response($response, $checks);

    // Default statuses.
    $transaction->remote_id = check_plain($response['transaction']);
    $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_AUTHORIZED;
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->log = $transaction->message = 'Transaction authorized via callback';

    // Check for instant capture. ERROR or DECLINED should throw an Exception.
    if (isset($response['captureStatus']) && in_array($response['captureStatus'], array('ERROR', 'DECLINED'))) {
      throw new Exception(format_string(
        'DIBS Callback, failed to capture funds for order: %order_id and transaction: %transaction_id. Please consult your DIBS administration.',
        array(
          '%order_id' => $order->order_id,
          '%transaction_id' => $transaction->transaction_id,
        )
      ));
    }
    elseif (isset($response['captureStatus']) && $response['captureStatus'] == 'ACCEPTED') {
      $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_CAPTURED;
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->log = $transaction->message = 'Transaction captured via callback';
    }

    $transaction->remote_id = check_plain($response['transaction']);

    // Save response as a payload.
    $transaction->data['last_payload'] = $transaction->payload = $response;

    if (!empty($transaction->log)) {
      $transaction->revision = TRUE;
    }

    // Save transaction, should return SAVED_UPDATED.
    commerce_payment_transaction_save($transaction);

    return 'OK';
  }
  catch (Exception $e) {
    watchdog_exception('commerce_dibs_payment_window', $e);

    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->log = $transaction->message = $e->getMessage();
    $transaction->data['last_payload'] = $transaction->payload = $_POST;
    $transaction->revision = TRUE;
    commerce_payment_transaction_save($transaction);

    return MENU_NOT_FOUND;
  }
}

/**
 * Page callback: Tries to perform an operation on an transaction.
 *
 * Does not return anything renderable, will set a message and redirect.
 *
 * @param string $op
 *   Operation to perform on the transaction.
 * @param object $order
 *   Order associated with the transaction.
 * @param object $transaction
 *   The transaction
 */
function commerce_dibs_payment_window_transaction_operation($op, $order, $transaction) {

  try {
    // Load inc.
    module_load_include('inc', 'commerce_dibs_payment_window', 'commerce_dibs_payment_window');

    $payment_method = commerce_payment_method_instance_load($transaction->instance_id);

    if (!$payment_method) {
      drupal_set_message(t('Failure to load Payment method for the transaction.'));
      $op = NULL;
    }

    // Load settings.
    $settings = array_replace_recursive(_commerce_dibs_payment_window_get_settings_default(), $payment_method['settings']);

    $api_user = $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_username'];
    $api_pass = $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_password'];

    // Create data array.
    $data = _commerce_dibs_payment_window_create_data_for_api($order, $transaction, $settings);

    // Perform API call.
    $response = _commerce_dibs_payment_window_perform_api_call($data, $op, $api_user, $api_pass);

    // Save response data for later.
    $transaction->data['last_payload'] = $transaction->payload = $response;

    // Check if response is valid.
    _commerce_dibs_payment_window_validate_response($response, array('transact' => $transaction->remote_id));

    // Check if response is success.
    _commerce_dibs_payment_window_response_has_correct_values($response, _commerce_dibs_payment_window_expected_values($op, $response['status']));


    switch ($op) {
      case 'capture':
        $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_CAPTURED;
        $transaction->log = $transaction->message = 'Transaction captured';
        $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        break;

      case 'cancel':
        $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_CANCELED;
        $transaction->log = $transaction->message = 'Transaction cancelled';
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;

      case 'refund':
        $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_REFUNDED;
        $transaction->log = $transaction->message = 'Transaction refunded';
        $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
        break;
    }

    $transaction->revision = TRUE;
    commerce_payment_transaction_save($transaction);
    drupal_set_message(t('%op of transaction (%transaction_id) was successfull.', array('%transaction_id' => $transaction->transaction_id, '%op' => drupal_ucfirst($op))));
  }
  catch (Exception $e) {
    watchdog_exception('commerce_dibs_payment_window', $e);

    $transaction->message = '@op: %message';
    $transaction->message_variables = array(
      '@op' => drupal_ucfirst($op),
      '%message' => $e->getMessage(),
    );
    $transaction->log = format_string($transaction->message, $transaction->message_variables);
    $transaction->revision = TRUE;
    commerce_payment_transaction_save($transaction);

    drupal_set_message(t('%op of transaction (%transaction_id) failed, error: %error_message (%error_code).',
      array(
        '%transaction_id' => $transaction->transaction_id,
        '%error_message' => $e->getMessage(),
        '%error_code' => $e->getCode(),
        '%op' => drupal_ucfirst($op),
      )), 'error');
  }

  drupal_goto(sprintf('admin/commerce/orders/%s/payment/', $order->order_id));
}
