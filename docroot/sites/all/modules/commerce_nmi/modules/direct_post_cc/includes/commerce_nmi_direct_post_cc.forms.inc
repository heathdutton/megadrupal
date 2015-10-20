<?php
/**
 * @file
 * Form callbacks for the Commerce NMI Direct Post - Credit eCheck module.
 */

use AllPlayers\NMI\Client;
use AllPlayers\NMI\PaymentMethod\CreditCard;
use AllPlayers\NMI\PaymentMethod\CustomerVault;
use AllPlayers\NMI\Request\CustomerVault\Add;

/**
 * Payment method callback: settings form.
 */
function commerce_nmi_direct_post_cc_settings_form($settings = NULL) {
  module_load_include('inc', 'commerce_nmi', 'includes/commerce_nmi.direct_post');
  $form = commerce_nmi_direct_post_settings_form($settings);

  // If the card on file module is enabled then offer it with this payment
  // method.
  $settings += array('cardonfile' => FALSE);
  if (module_exists('commerce_cardonfile')) {
    $form['cardonfile'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Card on File functionality with this payment method using NMI Customer Vault.'),
      '#description' => t('This requires an NMI merchant account with Customer Vault enabled.'),
      '#default_value' => $settings['cardonfile'],
    );
  }

  return $form;
}

/**
 * Payment method callback: settings form submit.
 */
function commerce_nmi_direct_post_cc_settings_form_submit(&$form, &$form_state) {
  module_load_include('inc', 'commerce_nmi', 'includes/commerce_nmi.direct_post');
  commerce_nmi_direct_post_settings_form_submit($form, $form_state);
}

/**
 * Payment method callback: checkout form.
 *
 * Uses the credit card form provided by commerce_payment with the security code
 * field.
 */
function commerce_nmi_direct_post_cc_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  return commerce_payment_credit_card_form(array('code' => ''));
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_nmi_direct_post_cc_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  // If the customer specified a card on file then we have nothing to validate.
  if (
    module_exists('commerce_cardonfile')
    && !empty($payment_method['settings']['cardonfile'])
    && !empty($pane_values['cardonfile'])
    && $pane_values['cardonfile'] !== 'new'
  ) {
    return;
  }

  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');

  // Validate the credit card fields.
  $settings = array(
    'form_parents' => array_merge($form_parents, array('credit_card')),
  );

  return commerce_payment_credit_card_validate($pane_values['credit_card'], $settings);
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_nmi_direct_post_cc_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Make sure the library has been installed before we continue.
  if (!commerce_nmi_library_installed()) {
    drupal_set_message(t('We encountered an error processing your transaction. Please contact us to resolve the issue.'), 'error');
    watchdog('commerce_nmi', 'Unable to find the AllPlayers\\NMI client library.', array(), WATCHDOG_ERROR);

    return FALSE;
  }

  // If the amount is zero then we do not want to process the payment.
  if ($charge['amount'] == 0) {
    // Create a transaction to log the skipped transaction and display a
    // helpful message to the customer.
    $transaction = commerce_payment_transaction_new('nmi_dp_cc', $order->order_id);
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = t('Invalid @amount; transaction not attempted.', array('@amount' => commerce_currency_format($charge['amount'], $charge['currency_code'])));
    commerce_payment_transaction_save($transaction);

    drupal_set_message('We encountered an error processing your transaction. Please contact us to resolve the issue.', 'error');
    return FALSE;
  }

  // If the customer specified a card on file then we should use it for payment
  // processing.
  if (
    module_exists('commerce_cardonfile')
    && !empty($payment_method['settings']['cardonfile'])
    && !empty($pane_values['cardonfile'])
    && $pane_values['cardonfile'] !== 'new'
  ) {
    // Attempt to load the stored card data.
    $card_data = commerce_cardonfile_load($pane_values['cardonfile']);

    // If the card is inactive or no longer available then we can not continue.
    if (empty($card_data) || $card_data->status == 0) {
      drupal_set_message(t('The requested card on file is no longer valid.'), 'error');

      return FALSE;
    }

    // Use the customer vault entry to pay for this transaction.
    $payment = new CustomerVault();
    $payment->customerVaultId = $card_data->remote_id;
  }
  else {
    // Build the credit card payment method for the request.
    $payment = new CreditCard();
    $payment->setExpiration(new DateTime($pane_values['credit_card']['exp_year'] . '-' . $pane_values['credit_card']['exp_month']));
    $payment->number = $pane_values['credit_card']['number'];
    $payment->cvv = $pane_values['credit_card']['code'];
  }

  // Build the request and add the payment method to it.
  $request = commerce_nmi_create_sale_request($payment_method, $charge, $order);
  $request->setPaymentMethod($payment);

  // Attempt to process the transaction.
  $client = new Client();
  $response = $client->transaction()->execute($request);

  // Prepare a transaction object to log the API response.
  $transaction = commerce_payment_transaction_new('nmi_dp_cc', $order->order_id);
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
  // display a message to the user. Otherwise, add the avs and cvv responses to
  // the transaction message.
  if ($response['response_code'] != COMMERCE_NMI_RESPONSE_CODE_SUCCESS) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    drupal_set_message(t('We received the following error processing your transaction. Please enter your information again: %error', array('%error' => $response['responsetext'])), 'error');
  }
  else {
    $transaction->message[] = t('AVS Reponse: %response', array('%response' => commerce_nmi_avs_response($response['avsresponse'])));
    $transaction->message[] = t('CVV Reponse: %response', array('%response' => commerce_nmi_cvv_response($response['cvvresponse'])));
  }

  // Format the message and save the transaction.
  $transaction->message = implode('<br />', $transaction->message);
  commerce_payment_transaction_save($transaction);

  // If the request failed then we need to rebuild the form.
  if ($response['response_code'] != COMMERCE_NMI_RESPONSE_CODE_SUCCESS) {
    return FALSE;
  }

  // If this payment method is configured for card on file and the user has
  // requested to store this card then we need to create a new customer vault
  // entry.
  if (
    module_exists('commerce_cardonfile')
    && $payment_method['settings']['cardonfile']
    && !empty($pane_values['credit_card']['cardonfile_store'])
  ) {
    // Build the request to add the customer vault entry using the same data
    // from the request.
    $vault = new Add();
    $vault->currency = $request->currency;
    $vault->username = $request->username;
    $vault->password = $request->password;
    $vault->setBillingDetails($request->getBillingDetails());
    $vault->setPaymentMethod($request->getPaymentMethod());
    $vault->setShippingDetails($request->getShippingDetails());

    // Execute the request.
    $response = $client->transaction()->execute($vault);

    // If the request succeeded then add the card, otherwise log the error.
    if ($response['response_code'] == COMMERCE_NMI_RESPONSE_CODE_SUCCESS) {
      module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
      $card_type = commerce_payment_validate_credit_card_type($pane_values['credit_card']['number'], array_keys(commerce_payment_credit_card_types()));

      $billing = $request->getBillingDetails();
      $card_data = commerce_cardonfile_new();
      $card_data->uid = $order->uid;
      $card_data->payment_method = $payment_method['method_id'];
      $card_data->instance_id = $payment_method['instance_id'];
      $card_data->remote_id = $response['customer_vault_id'];
      $card_data->card_type = !empty($card_type) ? $card_type : 'card';
      $card_data->card_name = !empty($billing) ? $billing->firstName . ' ' . $billing->lastName : '';
      $card_data->card_number = substr($pane_values['credit_card']['number'], -4);
      $card_data->card_exp_month = $pane_values['credit_card']['exp_month'];
      $card_data->card_exp_year = $pane_values['credit_card']['exp_year'];
      $card_data->status = 1;

      // Save the card data.
      commerce_cardonfile_save($card_data);
    }
    else {
      drupal_set_message('A problem was encountered while saving your payment information for later use.');
      watchdog('commerce_nmi', "An error was encountered while attempting to save a user's payment information: %error", array('%error' => $response['responsetext']), WATCHDOG_ERROR);
    }
  }
}
