<?php

/**
 * Creates a new payment transaction on an order.
 */
function commerce_drupalgap_stripe_create($data, $flatten_fields) {
  // Ensure the create request specifies a valid payment method type.
  if (empty($data['payment_method'])) {
    return services_error(t('You must specify a valid payment method type'), 400);
  }

  // Create the new payment transaction
  $transaction = commerce_payment_transaction_new($data['payment_method'], $data['order_id']);
  $order = commerce_order_load($data['order_id']);
  // Remove the method_id and order_id from the data array since they've been set.
  unset($data['payment_method'], $data['order_id']);
  // The card is new.  Either charge and forget, or charge and save.
  if (!commerce_stripe_load_library()) {
    services_error(t('Error making the payment. Please contact shop admin to proceed.'), 400);
    return FALSE;
  }


  $settings = _commerce_stripe_load_settings();
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $total = $order_wrapper->commerce_order_total->value();
  // Begin assembling charge parameters.
  //$transaction->instance_id = '$payment_method['instance_id']';
  $transaction->amount = $total['amount'];
  $transaction->currency_code = $settings['stripe_currency'];
  Stripe::setApiKey($settings['secret_key']);
  $c = array(
    'amount' => $total['amount'],
    'currency' => $settings['stripe_currency'],
    'card' => $data['stripe_token'],
    'description' => t('Order Number: @order_number', array('@order_number' => $order->order_number)),
  );

  try {
    $response = Stripe_Charge::create($c);
    $transaction->remote_id = $response->id;
    $transaction->payload[REQUEST_TIME] = $response->__toJSON();
    $transaction->message = t('Payment completed successfully.');
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    commerce_payment_transaction_save($transaction);
    return TRUE;
  } catch (Exception $e) {
    watchdog('commerce_stripe', 'Following error received when processing card @stripe_error.', array('@stripe_error' => $e->getMessage()), WATCHDOG_NOTICE);
    $transaction->remote_id = $e->getHttpStatus();
    $transaction->payload[REQUEST_TIME] = $e->json_body;
    $transaction->message = t('Card processing error: @stripe_error', array('@stripe_error' => $e->getMessage()));
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    commerce_payment_transaction_save($transaction);
    return FALSE;
  }
  return FALSE;
}

/**
 * Determines an API user's access to create new pamyent transactions.
 */
function commerce_drupalgap_stripe_create_access($data) {
  // Load the order from the data array to determine access to create payments
  // on it.
  $order = NULL;

  if (!empty($data['order_id'])) {
    $order = commerce_order_load($data['order_id']);
  }

  if (empty($order)) {
    return services_error(t('You must specify a valid order ID'), 400);
  }

  // If the user has access to update the order...
  if (commerce_order_access('update', $order)) {
    return TRUE;
  }
  else {
    return services_error(t('Access to this operation not granted'), 401);
  }
}
