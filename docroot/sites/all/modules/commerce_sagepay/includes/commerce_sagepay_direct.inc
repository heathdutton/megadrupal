<?php
/**
 * @file
 * commerce_sagepay_direct.inc
 * Functions for Direct integration.
 */

module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_utils');
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_formatters');
module_load_include('inc', 'commerce_sagepay', 'includes/commerce_sagepay_common');


/**
 * Payment method callback: checkout form.
 */
function commerce_sagepay_direct_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  $card_types = variable_get(SAGEPAY_SETTING_ENABLED_CARDS, array());
  if (empty($card_types)) {
    $card_types = array_keys(_commerce_sagepay_all_card_types());
  }

  $credit_card_settings = array(
    'type'       => $card_types,
    'number'     => '',
    'start_date' => '',
    'issue'      => '',
    'code'       => '',
  );

  return commerce_payment_credit_card_form($credit_card_settings);
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_sagepay_direct_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  // Check if settings are overriding this check.
  $perform_card_validation_check = TRUE;

  if (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE) == SAGEPAY_TXN_MODE_TEST
    && variable_get(SAGEPAY_SETTING_BYPASS_COMMERCE_CARD_VALIDATION) == 1
  ) {
    $perform_card_validation_check = FALSE;
  }

  if (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE) == SAGEPAY_TXN_MODE_SHOWPOST
    && variable_get(SAGEPAY_SETTING_BYPASS_COMMERCE_CARD_VALIDATION) == 1
  ) {
    $perform_card_validation_check = FALSE;
  }

  if ($perform_card_validation_check) {
    if (!commerce_payment_credit_card_validate($pane_values['credit_card'], $settings)) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_sagepay_direct_submit_form_submit($payment_method, $pane_form, $pane_values, &$order, $charge) {
  // Wrap the order for easier access to data.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  $total = commerce_line_items_total($wrapper->commerce_line_items);

  // Add tax if we have sales tax in the order.
  $total['amount'] = $wrapper->commerce_order_total->amount->value();

  // Load customer profile.
  $profile = commerce_customer_profile_load($order->commerce_customer_billing[LANGUAGE_NONE][0]['profile_id']);

  // Get user billing address.
  $billing_address = $profile->commerce_customer_address[LANGUAGE_NONE][0];

  // Get user delivery address.
  $delivery_address = NULL;

  if (isset($order->commerce_customer_shipping)) {
    $delivery_profile = commerce_customer_profile_load($order->commerce_customer_shipping[LANGUAGE_NONE][0]['profile_id']);
    $delivery_address = $delivery_profile->commerce_customer_address[LANGUAGE_NONE][0];
  }

  $settings = array();
  $data = _commerce_sagepay_encrypted_order($settings, $order, $total, $billing_address, $delivery_address, SAGEPAY_DIRECT, $pane_values);

  // If in TEST mode, replace the actual details entered with test data.
  if (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE) == SAGEPAY_TXN_MODE_TEST &&
    variable_get(SAGEPAY_SETTING_USE_TEST_DATA, 0) == 1
  ) {
    _commerce_sagepay_substitute_for_test_data($data);
  }
  if (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE) == SAGEPAY_TXN_MODE_SHOWPOST &&
    variable_get(SAGEPAY_SETTING_USE_TEST_DATA, 0) == 1
  ) {
    _commerce_sagepay_substitute_for_test_data($data);
  }

  // Create a POST to send to SagePay.
  $post = '';
  foreach ($data as $name => $value) {
    $post .= urlencode($name) . '=' . urlencode($value) . '&';
  }

  // Chop off the last &.
  $post = substr($post, 0, -1);

  // Determine the correct url based on the transaction mode.
  switch (variable_get(SAGEPAY_SETTING_TRANSACTION_MODE)) {
    case SAGEPAY_TXN_MODE_LIVE:
      $server_url = SAGEPAY_DIRECT_SERVER_LIVE;
      break;

    case SAGEPAY_TXN_MODE_TEST:
      $server_url = SAGEPAY_DIRECT_SERVER_TEST;
      break;

    case SAGEPAY_TXN_MODE_SHOWPOST:
      $server_url = SAGEPAY_DIRECT_SERVER_SHOWPOST;
      break;

    case SAGEPAY_TXN_MODE_SIMULATION:
      $server_url = SAGEPAY_DIRECT_SERVER_SIMULATION;
      break;

    default:
      $server_url = SAGEPAY_DIRECT_SERVER_SIMULATION;

  }
  drupal_alter('sagepay_url', $server_url, $pane_values);

  // Create a pending transaction that will be completed on return.
  // Determine the correct remote status based on the transaction type.
  switch($data['TxType']) {
    case 'DEFERRED' :
      $remote_status = 'DEFERRED';
      break;
    default :
      $remote_status = 'PAYMENT';
  }

  $tokens = array();
  $transaction = commerce_sagepay_transaction($payment_method, $order,
    $charge, $tokens,
    COMMERCE_PAYMENT_STATUS_PENDING, $remote_status);

  // Send post.
  $response = _commerce_sagepay_request_post($server_url, $post);

  // Collect additional data to store in the transaction.
  $response['Last4Digits'] = substr($pane_values['credit_card']['number'], 12, 16);
  $response['ExpMonth'] = $pane_values['credit_card']['exp_month'];
  $response['ExpYear'] = $pane_values['credit_card']['exp_year'];
  $response['cardonfile_store'] = isset($pane_values['credit_card']['cardonfile_store']) ? $pane_values['credit_card']['cardonfile_store'] : '0';
  $response['cardonfile_instance_default'] = isset($pane_values['cardonfile_instance_default']) ? $pane_values['cardonfile_instance_default'] : '0';
  $response['CardType'] = $pane_values['credit_card']['type'];
  $response['Amount'] = $wrapper->commerce_order_total->amount->value();

  $result = commerce_sagepay_process_response($payment_method, $order,
    $response, $transaction);

  return $result;
}
