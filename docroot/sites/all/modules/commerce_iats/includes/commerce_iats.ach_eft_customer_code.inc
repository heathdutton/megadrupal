<?php
/**
 * @file
 * Implements iATS Payments ACH/EFT customer code services for use in Drupal
 * Commerce.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_iats_ach_eft_customer_code_settings_form($settings = NULL) {
  $form = commerce_iats_settings_form_base($settings);

  // No invoice when creating customer codes.
  unset($form['invoice_prefix']);

  return $form;
}

/**
 * Payment method callback: checkout form.
 */
function commerce_iats_ach_eft_customer_code_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  if (isset($payment_method['settings']['direct_post']['enable_direct_post']) && $payment_method['settings']['direct_post']['enable_direct_post'] == 1) {
    module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.direct_post.forms');
    return commerce_iats_direct_post_ach_eft_customer_code_form($payment_method, $order);
  }
  else {
    module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.ach_eft.forms');

    $fields = array(
      'number' => '',
    );

    $currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];

    switch ($currency_code) {
      case 'USD':
        return commerce_iats_ach_eft_payment_form_usd($fields);
        break;
      case 'CAD':
        return commerce_iats_ach_eft_payment_form_cad($fields);
        break;
      case 'GBP':
        return commerce_iats_ach_eft_payment_form_gbp($fields);
        break;
      default:
        return commerce_iats_ach_eft_payment_form($fields);
    }
  }
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_iats_ach_eft_customer_code_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  if (!commerce_iats_validate_payment_method($payment_method)) {
    drupal_set_message(t('Unable to process payment. Please contact the site administrator.'), 'error');
    return FALSE;
  }

  module_load_include('inc', 'commerce_iats', 'includes/commerce_iats.ach_eft.forms');

  // Validate the bank account fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('bank_account')),
  );

  if (!commerce_iats_ach_eft_payment_form_validate($pane_values['bank_account'], $settings)) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_iats_ach_eft_customer_code_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $payment_data = $pane_values;

  return commerce_iats_process_payment($payment_method, $payment_data, $order,
    $charge, 'commerce_iats_create_customer_code');
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
  // Create currency-specific account number string.
  $account_num = '';
  switch ($charge['currency_code']) {
    case 'USD':
      $account_num .= $payment_data['bank_account']['routing_number'];
      break;
    case 'CAD':
      $account_num .= $payment_data['bank_account']['bank_number'];
      $account_num .= $payment_data['bank_account']['transit_number'];
      break;
    case 'GBP':
      $account_num .= commerce_iats_format_sort_code($payment_data['bank_account']['sort_code']);
      break;
  }

  $account_num .= $payment_data['bank_account']['number'];

  // Create and populate the API request.
  $request = array(
    'customerIPAddress' => ip_address(),
    'accountNum' => $account_num,
    'accountType' => isset($payment_data['bank_account']['type']) ? $payment_data['bank_account']['type'] : '',
    'recurring' => 0,
    'beginDate' => commerce_iats_get_formatted_date(time()),
    'endDate' => commerce_iats_get_formatted_date(time()),
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

  $response = $iats->createACHEFTCustomerCode($request);

  return $response;
}
