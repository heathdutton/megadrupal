<?php
/**
 * @file
 * Implements iATS Payments credit card customer code services for use in Drupal
 * Commerce.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_iats_credit_card_customer_code_settings_form($settings = NULL) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  $form = commerce_iats_credit_card_settings_form_base($settings);

  // No invoice when creating customer codes.
  unset($form['invoice_prefix']);

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_iats_credit_card_customer_code_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  if (isset($payment_method['settings']['direct_post']['enable_direct_post']) && $payment_method['settings']['direct_post']['enable_direct_post'] == 1) {
    module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.direct_post.forms');
    return commerce_iats_direct_post_credit_card_customer_code_form($payment_method, $order);
  }
  else {
    module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

    // Prepare the fields to include on the credit card form.
    $fields = array(
      'code' => '',
      'type' => commerce_iats_enabled_credit_card_types($payment_method),
    );

    return commerce_payment_credit_card_form($fields);
  }
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_iats_credit_card_customer_code_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  if (!commerce_iats_validate_payment_method($payment_method)) {
    drupal_set_message(t('Unable to process payment. Please contact the site administrator.'), 'error');
    return FALSE;
  }

  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  if (empty($pane_values['credit_card']['code']))
  {
    unset($pane_values['credit_card']['code']);
  }

  return commerce_payment_credit_card_validate($pane_values['credit_card'], $settings);
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_iats_credit_card_customer_code_submit_form_submit($payment_method, $pane_form, $pane_values, $order) {
  $payment_data = $pane_values;

  return commerce_iats_process_payment($payment_method, $payment_data, $order,
    NULL, 'commerce_iats_create_customer_code');
}

/**
 * Creates a customer code via the iATS Payments API.
 *
 * @param array $payment_method
 *   The payment method used during this transaction.
 * @param array $payment_data
 *   Payment data retrieved from the submit form.
 * @param object $order
 *   The order processed during this transaction.
 * @param array $charge
 *   The transaction charge. Unused in this method, but required
 *   as part of callback signature.
 *
 * @return Mixed
 *   API response array or error string.
 */
function commerce_iats_create_customer_code($payment_method, $payment_data, $order, $charge) {
  // Format credit card data.
  $formatted_card_data = commerce_iats_format_credit_card_data($payment_data['credit_card']);

  // Create and populate the API request.
  $request = array(
    'customerIPAddress' => ip_address(),
    'mop' => $formatted_card_data['type'],
    'creditCardNum' => $formatted_card_data['number'],
    'creditCardExpiry' => $formatted_card_data['expiry'],
    'cvv2' => $payment_data['credit_card']['code'],
    'recurring' => 0,
    'beginDate' => commerce_iats_get_formatted_date(time()),
    'endDate' => commerce_iats_get_formatted_date(time()),
    'currency' => 'USD',
    'comment' => COMMERCE_IATS_COMMENT_PREFIX,
  );

  // Add the billing information.
  commerce_iats_address($request, $order, 'commerce_customer_billing');

  // Get the API service.
  $iats = commerce_iats_get_service('CustomerLink', $payment_method);
  if ($iats == NULL) {
    drupal_set_message(t('Unable to load iATS Payments library.'), 'error');
    return NULL;
  }

  // Log the request.
  if ($payment_method['settings']['log']['request'] === 'request') {
    commerce_iats_log_request($payment_method, 'commerce_iats_create_customer_code', $request);
  }

  $response = $iats->createCreditCardCustomerCode($request);

  return $response;
}
