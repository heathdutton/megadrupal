<?php

/**
 * @file
 * Authorize.net API type wrapper functions
 */

/**
 * Returns a new initialized AuthorizeNet request object.
 *
 * @param $type
 *   The type of AuthorizeNet request object to create (AIM, CIM, etc).
 * @param $options
 *   Optional variable overrides. Possible values:
 *     login_id (Authorize.net login id)
 *     transaction_key (Authorize.net transaction key)
 *     sandbox (Sandbox mode boolean)
 *
 * @return
 *   Returns an AuthorizeNet request object.
 */
function authnet_request_new($type, $options = array()) {

  // Merge in defaults.
  $options += array(
    'login_id' => variable_get('authnet_login_id', ''),
    'transaction_key' => variable_get('authnet_transaction_key', ''),
    'sandbox' => variable_get('authnet_sandbox', FALSE),
  );

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Build the class name.
  $class = 'AuthorizeNet' . $type;

  // Create an Authorize.net request object.
  $request = new $class($options['login_id'], $options['transaction_key']);

  // Enable sandbox mode, if the setting is set.
  if ($options['sandbox']) {
    $request->setSandbox(TRUE);
  }
  else {
    $request->setSandbox(FALSE);
  }

  // Return the request object.
  return $request;
}

/**
 * Returns a new initialized AuthorizeNetCustomer object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetCustomer object.
 */
function authnet_customer_profile_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetCustomer object.
  $customer_profile = new AuthorizeNetCustomer;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['description'])) {
    $customer_profile->description = $values['description'];
  }
  if (isset($values['merchantCustomerId'])) {
    $customer_profile->merchantCustomerId = $values['merchantCustomerId'];
  }
  if (isset($values['email'])) {
    $customer_profile->email = $values['email'];
  }
  if (isset($values['customerProfileId'])) {
    $customer_profile->customerProfileId = $values['customerProfileId'];
  }
  if (isset($values['paymentProfiles']) && !empty($values['paymentProfiles'])) {
    foreach ($values['paymentProfiles'] as $payment_profile) {
      $customer_profile->paymentProfiles[] = authnet_payment_profile_new($payment_profile);
    }
  }
  if (isset($values['shipToList']) && !empty($values['shipToList'])) {
    foreach ($values['shipToList'] as $ship_to) {
      $customer_profile->shipToList[] = authnet_address_new($ship_to);
    }
  }

  // Return the profile object.
  return $customer_profile;
}

/**
 * Returns a new initialized AuthorizeNetAddress object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetAddress object.
 */
function authnet_address_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetAddress object.
  $address = new AuthorizeNetAddress;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['firstName'])) {
    $address->firstName = $values['firstName'];
  }
  if (isset($values['lastName'])) {
    $address->lastName = $values['lastName'];
  }
  if (isset($values['company'])) {
    $address->company = $values['company'];
  }
  if (isset($values['address'])) {
    $address->address = $values['address'];
  }
  if (isset($values['city'])) {
    $address->city = $values['city'];
  }
  if (isset($values['state'])) {
    $address->state = $values['state'];
  }
  if (isset($values['zip'])) {
    $address->zip = $values['zip'];
  }
  if (isset($values['country'])) {
    $address->country = $values['country'];
  }
  if (isset($values['phoneNumber'])) {
    $address->phoneNumber = $values['phoneNumber'];
  }
  if (isset($values['faxNumber'])) {
    $address->faxNumber = $values['faxNumber'];
  }
  if (isset($values['customerAddressId'])) {
    $address->customerAddressId = $values['customerAddressId'];
  }

  // Return the address object.
  return $address;
}

/**
 * Returns a new initialized AuthorizeNetPaymentProfile object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetPaymentProfile object.
 */
function authnet_payment_profile_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetPaymentProfile object.
  $payment_profile = new AuthorizeNetPaymentProfile;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['customerPaymentProfileId'])) {
    $payment_profile->customerPaymentProfileId = $values['customerPaymentProfileId'];
  }
  if (isset($values['customerType'])) {
    $payment_profile->customerType = $values['customerType'];
  }
  if (isset($values['billTo'])) {
    $payment_profile->billTo = authnet_address_new($values['billTo']);
  }
  if (isset($values['payment'])) {
    $payment_profile->payment = authnet_payment_new($values['payment']);
  }

  // Return the profile object.
  return $payment_profile;
}

/**
 * Returns a new initialized AuthorizeNetPayment object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetPayment object.
 */
function authnet_payment_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetPayment object.
  $payment = new AuthorizeNetPayment;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['creditCard'])) {
    $payment->creditCard = authnet_credit_card_new($values['creditCard']);
  }
  if (isset($values['bankAccount'])) {
    $payment->bankAccount = authnet_bank_account_new($values['bankAccount']);
  }

  // Return the payment object.
  return $payment;
}

/**
 * Returns a new initialized AuthorizeNetTransaction object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetTransaction object.
 */
function authnet_transaction_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetTransaction object.
  $transaction = new AuthorizeNetTransaction;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['amount'])) {
    $transaction->amount = $values['amount'];
  }
  if (isset($values['tax'])) {
    $transaction->tax = (object) $values['tax'];
  }
  if (isset($values['shipping'])) {
    $transaction->shipping = (object) $values['shipping'];
  }
  if (isset($values['duty'])) {
    $transaction->duty = (object) $values['duty'];
  }
  if (isset($values['lineItems']) && !empty($values['lineItems'])) {
    foreach ($values['lineItems'] as $line_item) {
      $transaction->lineItems[] = authnet_line_item_new($line_item);
    }
  }
  if (isset($values['customerProfileId'])) {
    $transaction->customerProfileId = $values['customerProfileId'];
  }
  if (isset($values['customerPaymentProfileId'])) {
    $transaction->customerPaymentProfileId = $values['customerPaymentProfileId'];
  }
  if (isset($values['customerShippingAddressId'])) {
    $transaction->customerShippingAddressId = $values['customerShippingAddressId'];
  }
  if (isset($values['creditCardNumberMasked'])) {
    $transaction->creditCardNumberMasked = $values['creditCardNumberMasked'];
  }
  if (isset($values['bankRoutingNumberMasked'])) {
    $transaction->bankRoutingNumberMasked = $values['bankRoutingNumberMasked'];
  }
  if (isset($values['bankAccountNumberMasked'])) {
    $transaction->bankAccountNumberMasked = $values['bankAccountNumberMasked'];
  }
  if (isset($values['order'])) {
    $transaction->order = (object) $values['order'];
  }
  if (isset($values['taxExempt'])) {
    $transaction->taxExempt = $values['taxExempt'];
  }
  if (isset($values['recurringBilling'])) {
    $transaction->recurringBilling = $values['recurringBilling'];
  }
  if (isset($values['cardCode'])) {
    $transaction->cardCode = $values['cardCode'];
  }
  if (isset($values['splitTenderId'])) {
    $transaction->splitTenderId = $values['splitTenderId'];
  }
  if (isset($values['approvalCode'])) {
    $transaction->approvalCode = $values['approvalCode'];
  }
  if (isset($values['transId'])) {
    $transaction->transId = $values['transId'];
  }

  // Return the transaction object.
  return $transaction;
}

/**
 * Returns a new initialized AuthorizeNetLineItem object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetLineItem object.
 */
function authnet_line_item_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetLineItem object.
  $line_item = new AuthorizeNetLineItem;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['itemId'])) {
    $line_item->itemId = $values['itemId'];
  }
  if (isset($values['name'])) {
    $line_item->name = $values['name'];
  }
  if (isset($values['description'])) {
    $line_item->description = $values['description'];
  }
  if (isset($values['quantity'])) {
    $line_item->quantity = $values['quantity'];
  }
  if (isset($values['unitPrice'])) {
    $line_item->unitPrice = $values['unitPrice'];
  }
  if (isset($values['taxable'])) {
    $line_item->taxable = $values['taxable'];
  }

  // Return the line item object.
  return $line_item;
}

/**
 * Returns a new initialized AuthorizeNetCreditCard object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetCreditCard object.
 */
function authnet_credit_card_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetCreditCard object.
  $card = new AuthorizeNetCreditCard;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['cardNumber'])) {
    $card->cardNumber = $values['cardNumber'];
  }
  if (isset($values['expirationDate'])) {
    $card->expirationDate = $values['expirationDate'];
  }
  if (isset($values['cardCode'])) {
    $card->cardCode = $values['cardCode'];
  }

  // Return the credit card object.
  return $card;
}

/**
 * Returns a new initialized AuthorizeNetBankAccount object, optionally with some initial values.
 *
 * @param $values
 *   An array of values to pre-populate the object with.
 *
 * @return
 *   Returns an initialized AuthorizeNetBankAccount object.
 */
function authnet_bank_account_new($values=array()) {

  // Include the Authorize.net SDK.
  authnet_sdk_include();

  // Create a new AuthorizeNetBankAccount object.
  $bank = new AuthorizeNetBankAccount;

  // Initialize some properties, using the $values argument if it's available.
  if (isset($values['accountType'])) {
    $bank->accountType = $values['accountType'];
  }
  if (isset($values['routingNumber'])) {
    $bank->routingNumber = $values['routingNumber'];
  }
  if (isset($values['accountNumber'])) {
    $bank->accountNumber = $values['accountNumber'];
  }
  if (isset($values['nameOnAccount'])) {
    $bank->nameOnAccount = $values['nameOnAccount'];
  }
  if (isset($values['echeckType'])) {
    $bank->echeckType = $values['echeckType'];
  }
  if (isset($values['bankName'])) {
    $bank->bankName = $values['bankName'];
  }

  // Return the bank account object.
  return $bank;
}
