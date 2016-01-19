<?php
/**
 * @file
 * Card on File module integration.
 */

/**
 * Implements hook_commerce_securepayau_commerce_payment_method_info_alter() on
 * behalf of commerce_cardonfile.module.
 */
function commerce_securepayau_commerce_securepayau_commerce_payment_method_info_alter(&$payment_methods) {
  $payment_methods['securepayau_xmlapi']['cardonfile'] = isset($payment_methods['securepayau_xmlapi']['cardonfile'])
    ? $payment_methods['securepayau_xmlapi']['cardonfile']
    : array();
  $payment_methods['securepayau_xmlapi']['cardonfile']['update callback'] = 'commerce_securepayau_xml_cardonfile_update';
  $payment_methods['securepayau_xmlapi']['cardonfile']['delete callback'] = 'commerce_securepayau_xml_cardonfile_delete';
}

/**
 * Implements hook_form_commerce_cardonfile_update_form_alter().
 */
function commerce_securepayau_form_commerce_cardonfile_update_form_alter(&$form, $form_id) {
  if ($form['card_data']['#value']['payment_method'] == 'securepayau_xmlapi') {
    // Clear Credit Card number default value as SecurePay.com.au XML API does
    // not allow you to update a payment method, instead it needs to delete and
    // recreate the payment method, which requires the Credit Card number to be
    // re-entered.
    $form['credit_card']['number']['#default_value'] = '';
    unset($form['credit_card']['number']['#description']);

    // As we are re-creating the trigger for via the SecurePay.com.au XML API,
    // the CVV code needs to be re-entered.
    $temporary_form = commerce_payment_credit_card_form(array('code' => ''), array());
    $form['credit_card']['code'] = $temporary_form['credit_card']['code'];
  }
}

/**
 * Card on file callback: updates the associated customer payment profile.
 */
function commerce_securepayau_xml_cardonfile_update($form, &$form_state, $payment_method, $card_data) {
  // Delete existing triggered payment.
  commerce_securepayau_xml_cardonfile_delete($form, $form_state, $payment_method, $card_data);

  // Build dummy order object.
  $order = new stdClass();
  $order->uid = $card_data['uid'];

  // Build dummy charge array. We attempt to pull the amount and currency code
  // from $form_state['values'] to give other modules the ability to define
  // these values.
  $charge = array(
    'amount' => isset($form_state['values']['amount']) ? $form_state['values']['amount'] : '1',
    'currency_code' => isset($form_state['values']['currency_code']) ? $form_state['values']['currency_code'] : $payment_method['settings']['settings']['currency'],
  );

  $request = _commerce_securepayau_xmlapi_request_triggered_add($form_state['values'], $order, $charge);
  $xml = _commerce_securepayau_xmlapi_wrapper($payment_method, 'Periodic', _commerce_securepayau_array_to_xml($request));
  $response = commerce_securepayau_xmlapi($payment_method, 'Periodic', '', $xml);

  // Parse the response.
  $response = _commerce_securepayau_parse_response($response);

  return $response['successful'] == 'yes';
}

/**
 * Card on file callback: deletes the associated customer payment profile.
 */
function commerce_securepayau_xml_cardonfile_delete($form, &$form_state, $payment_method, $card_data) {
  $request = _commerce_securepayau_xmlapi_request_triggered_delete($card_data['remote_id']);
  $xml = _commerce_securepayau_xmlapi_wrapper($payment_method, 'Periodic', _commerce_securepayau_array_to_xml($request));
  $response = commerce_securepayau_xmlapi($payment_method, 'Periodic', '', $xml);

  // Parse the response.
  $response = _commerce_securepayau_parse_response($response);

  return $response['successful'] == 'yes';
}

/**
 * Implements hook_commerce_securepayau_xmlapi_settings_form_alter() on behalf
 * of commerce_cardonfile.module.
 *
 * Adds Card on File checkbox to Payment method settings form.
 */
function commerce_cardonfile_commerce_securepayau_xmlapi_settings_form_alter(&$form, $settings) {
  $form['cardonfile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Card on File integration via SecurePay.com.au XML API - Periodic and Triggered add in'),
    '#default_value' => isset($settings['cardonfile']) ? $settings['cardonfile'] : FALSE,
  );
}

/**
 * Implements hook_commerce_securepayau_xmlapi_submit_form_validate_alter()
 * on behalf of commerce_cardonfile.module.
 */
function commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_validate_alter(&$function, $payment_method, $pane_values) {
  if ($payment_method['settings']['cardonfile'] && $pane_values['cardonfile'] !== 'new') {
    $function = '_commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_validate';
  }
}

function _commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents) {
  return;
}

/**
 * Implements hook_commerce_securepayau_xmlapi_submit_form_submit_alter()
 * on behalf of commerce_cardonfile.module.
 */
function commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_submit_alter(&$function, $payment_method, $pane_values) {
  if ($payment_method['settings']['cardonfile'] == TRUE && !empty($pane_values['cardonfile']) && $pane_values['cardonfile'] !== 'new') {
    $function = '_commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_submit';
  }
}

function _commerce_cardonfile_commerce_securepayau_xmlapi_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // First attempt to load the card on file.
  $card_data = commerce_cardonfile_data_load($pane_values['cardonfile']);

  // Fail now if it is no longer available or the card is inactive.
  if (empty($card_data) || $card_data['status'] == 0) {
    drupal_set_message(t('The requested card on file is no longer valid.'), 'error');
    return FALSE;
  }

  $request = _commerce_securepayau_xmlapi_request_triggered_trigger($card_data, $charge);
  $xml = _commerce_securepayau_xmlapi_wrapper($payment_method, 'Periodic', _commerce_securepayau_array_to_xml($request));
  $response = commerce_securepayau_xmlapi($payment_method, 'Periodic', '', $xml);

  // Parse the response.
  $response = _commerce_securepayau_parse_response($response);

  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('securepayau_xml', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->remote_id = isset($response['txnID']) ? $response['txnID'] : '';
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->payload[REQUEST_TIME] = $response;
  $transaction->status = $response['successful'] == 'yes' ? COMMERCE_PAYMENT_STATUS_SUCCESS : COMMERCE_PAYMENT_STATUS_FAILURE;

  // Save the transaction information.
  commerce_payment_transaction_save($transaction);

  // If the payment failed, display an error and rebuild the form.
  if ($response['successful'] != 'yes') {
    // @TODO - Error message.
    // drupal_set_message(t('We received the following error processing your card. Please enter you information again or try a different card.'), 'error');
    // drupal_set_message(check_plain($response[3]), 'error');
    return FALSE;
  }

  return TRUE;
}

/**
 * Implements hook_commerce_securepayau_payment_successful() on behalf of
 * commerce_cardonfile.module.
 */
function commerce_cardonfile_commerce_securepayau_payment_successful($payment_method, $pane_values, $order, $charge) {
  if ($payment_method['settings']['cardonfile'] && $pane_values['credit_card']['cardonfile_store']) {
    // First look to see if we already have cards on file for the user.
    $stored_cards = commerce_cardonfile_data_load_multiple($order->uid, $payment_method['instance_id']);
    $add_to_profile = NULL;

    if (empty($stored_cards)) {
      $request = _commerce_securepayau_xmlapi_request_triggered_add($pane_values, $order, $charge);
      $xml = _commerce_securepayau_xmlapi_wrapper($payment_method, 'Periodic', _commerce_securepayau_array_to_xml($request));
      $response = commerce_securepayau_xmlapi($payment_method, 'Periodic', '', $xml);

      // Parse the response.
      $response = _commerce_securepayau_parse_response($response);

      if ($response['statusCode'] === '0') {
        $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
        $billing_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();

        $card_data = array(
          'uid' => $order->uid,
          'payment_method' => $payment_method['method_id'],
          'instance_id' => $payment_method['instance_id'],
          'remote_id' => $response['clientID'],
          'card_type' => 'card',
          'card_name' => $billing_address['name_line'],
          'card_number' => substr($pane_values['credit_card']['number'], -4),
          'card_exp_month' => $pane_values['credit_card']['exp_month'],
          'card_exp_year' => $pane_values['credit_card']['exp_year'],
          'status' => 1,
        );

        // Save the creation of the new card on file.
        return commerce_cardonfile_data_save($card_data);
      }
    }
  }
  return FALSE;
}
