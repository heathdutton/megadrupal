<?php
/**
 * @file
 * Form callbacks for the Commerce NMI Direct Post - eCheck module.
 */

use AllPlayers\NMI\Client;
use AllPlayers\NMI\PaymentMethod\Check;

/**
 * Payment method callback: settings form.
 */
function commerce_nmi_direct_post_echeck_settings_form($settings = NULL) {
  module_load_include('inc', 'commerce_nmi', 'includes/commerce_nmi.direct_post');

  return commerce_nmi_direct_post_settings_form($settings);
}

/**
 * Payment method callback: settings form submit.
 */
function commerce_nmi_direct_post_echeck_settings_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'commerce_nmi', 'includes/commerce_nmi.direct_post');
  commerce_nmi_direct_post_settings_form_submit($form, $form_state);
}

/**
 * Payment method callback: checkout form.
 *
 * Uses the echeck form provided by commerce_payment with the bank name and
 * account type fields.
 */
function commerce_nmi_direct_post_echeck_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.echeck');

  $fields = drupal_map_assoc(array('bank_name'));
  $fields['type'] = drupal_map_assoc(array('checking', 'business_checking', 'savings'));

  return commerce_payment_echeck_form($fields);
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_nmi_direct_post_echeck_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.echeck');

  // Validate the echeck fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('echeck')),
  );

  return commerce_payment_echeck_validate($pane_values['echeck'], $settings);
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_nmi_direct_post_echeck_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Make sure the library has been installed before we continue.
  if (!commerce_nmi_library_installed()) {
    drupal_set_message(t('We encountered an error processing your transaction. Please contact us to resolve the issue.'), 'error');
    watchdog(WATCHDOG_ERROR, 'Unable to find the AllPlayers\\NMI client library.');

    return FALSE;
  }

  // If the amount is zero then we do not want to process the payment.
  if ($charge['amount'] == 0) {
    // Create a transaction to log the skipped transaction and display a
    // helpful message to the customer.
    $transaction = commerce_payment_transaction_new('nmi_dp_echeck', $order->order_id);
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = t('Invalid @amount transaction not attempted.', array('@amount' => commerce_currency_format($charge['amount'], $charge['currency_code'])));
    commerce_payment_transaction_save($transaction);

    drupal_set_message('We encountered an error processing your transaction. Please contact us to resolve the issue.', 'error');
    return FALSE;
  }

  $account_type_map = array(
    'checking' => Check::ACCOUNT_TYPE_CHECKING,
    'business_checking' => Check::ACCOUNT_TYPE_CHECKING,
    'savings' => Check::ACCOUNT_TYPE_SAVINGS,
  );

  // Build the echeck payment method for the request.
  $payment = new Check();
  $payment->accountHolderType = ($pane_values['echeck']['type'] == 'business_checking' ? Check::ACCOUNT_HOLDER_BUSINESS : Check::ACCOUNT_HOLDER_PERSONAL);
  $payment->accountNumber = $pane_values['echeck']['acct_num'];
  $payment->accountType = $account_type_map[$pane_values['echeck']['type']];
  $payment->bankName = $pane_values['echeck']['bank_name'];
  $payment->routingNumber = $pane_values['echeck']['aba_code'];

  // Build the request and add the payment method to it.
  $request = commerce_nmi_create_sale_request($payment_method, $charge, $order);
  $request->setPaymentMethod($payment);

  // Attempt to process the transaction.
  $client = new Client();
  $response = $client->transaction()->execute($request);

  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('nmi_dp_echeck', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->payload[time()] = $response;
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->message = array('<b>' . $response['responsetext'] . '</b>');

  // If a transaction id was returned then set it on the transaction.
  if (isset($response['transactionid'])) {
    $transaction->remote_id = $response['transactionid'];
  }

  // If the request failed then we need to set the status appropriatly and
  // display a message to the user.
  if ($response['response_code'] != COMMERCE_NMI_RESPONSE_CODE_SUCCESS) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    drupal_set_message(t('We received the following error processing your transaction. Please enter your information again: %error', array('%error' => $response['responsetext'])), 'error');
  }

  // Format the message and save the transaction.
  $transaction->message = implode('<br />', $transaction->message);
  commerce_payment_transaction_save($transaction);

  // If the request failed then we need to rebuild the form.
  if ($response['response_code'] != COMMERCE_NMI_RESPONSE_CODE_SUCCESS) {
    return FALSE;
  }
}
