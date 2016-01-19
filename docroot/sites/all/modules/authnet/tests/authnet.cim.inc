<?php

/**
 * @file
 * Authorize.net - CIM test code
 */

/**
 * The following are some sample PHP snippets for testing the API wrapper functions.
 * Just paste this into the Devel module's /devel/php to test it out.
 * It may not be SimpleTest, but it's better than nothing.
 */

/**
 * Create a new customer profile based on a user
 */
$uid = X;
$user = user_load($uid);

$values = array(
  'merchantCustomerId' => $user->uid,
  'description' => $user->name,
  'email' => $user->mail,
  'paymentProfiles' => array(
    array(
      'customerType' => 'individual',
      'billTo' => array(
        'firstName' => 'John',
        'lastName' => 'Doe',
        'company' => 'Company, Inc.',
        'address' => '123 Fake Street',
        'city' => 'Pleasantville',
        'state' => 'CA',
        'zip' => '123456',
        'country' => 'USA',
        'phoneNumber' => '123 456 7890',
        'faxNumber' => '789 123 4567',
      ),
      'payment' => array(
        'creditCard' => array(
          'cardNumber' => '4007000000027',
          'expirationDate' => '2012-12',
          'cardCode' => '123',
        ),
      ),
    ),
  ),
);
$cim_profile = authnet_customer_profile_new($values);
dpm($cim_profile);
authnet_cim_profile_save($cim_profile);
dpm($cim_profile);

/**
 * Load an existing customer profile based on a customerProfileId.
 */

$customer_profile_id = XXXXXXXX;

$cim_profile = authnet_cim_profile_load($customer_profile_id);

dpm($cim_profile);

/**
 * Update an existing profile.
 */

$customer_profile_id = XXXXXXXX;
$cim_profile = authnet_cim_profile_load($customer_profile_id);

dpm($cim_profile);

$cim_profile->description = 'TEST TEST TEST';

authnet_cim_profile_save($cim_profile);

dpm($cim_profile);

/**
 * Delete a profile.
 */

$customer_profile_id = XXXXXXXX;
authnet_cim_profile_delete($customer_profile_id);

/**
 * Load a payment profile.
 */

$customer_profile_id = XXXXXXXX;
$customer_payment_profile_id = XXXXXXXX;

$payment_profile = authnet_cim_payment_profile_load($customer_profile_id, $customer_payment_profile_id);

dpm($payment_profile);

/**
 * Save a new payment profile.
 */

$customer_profile_id = XXXXXXXX;

$values = array(
  'customerType' => 'individual',
  'billTo' => array(
    'firstName' => 'John',
    'lastName' => 'Doe',
    'company' => 'Company, Inc.',
    'address' => '123 Fake Street',
    'city' => 'Pleasantville',
    'state' => 'CA',
    'zip' => '123456',
    'country' => 'USA',
    'phoneNumber' => '123 456 7890',
    'faxNumber' => '789 123 4567',
  ),
  'payment' => array(
    'bankAccount' => array(
      'accountType' => 'checking',
      'routingNumber' => '123456789',
      'accountNumber' => '987654321',
      'nameOnAccount' => 'John Doe',
      'bankName' => 'Bank, Inc.',
    ),
  ),
);

$payment_profile = authnet_payment_profile_new($values);

dpm($payment_profile);

authnet_cim_payment_profile_save($customer_profile_id, $payment_profile);

dpm($payment_profile);

/**
 * Update an existing payment profile.
 */

$customer_profile_id = XXXXXXXX;
$payment_profile_id = XXXXXXXX;

$payment_profile = authnet_cim_payment_profile_load($customer_profile_id, $payment_profile_id);

dpm($payment_profile);

$payment_profile->billTo->firstName = 'Mike';

authnet_cim_payment_profile_save($customer_profile_id, $payment_profile);

dpm($payment_profile);

/**
 * Delete an existing payment profile.
 */

$customer_profile_id = XXXXXXXX;
$payment_profile_id = XXXXXXXX;

$payment_profile = authnet_cim_payment_profile_delete($customer_profile_id, $payment_profile_id);

/**
 * Load a shipping address.
 */

$customer_profile_id = XXXXXXXX;
$customer_shipping_address_id = XXXXXXXX;

$address = authnet_cim_shipping_address_load($customer_profile_id, $customer_shipping_address_id);

dpm($address);

/**
 * Save a new shipping address.
 */

$customer_profile_id = XXXXXXXX;

$values = array(
  'firstName' => 'John',
  'lastName' => 'Doe',
  'company' => 'Company, Inc.',
  'address' => '123 Fake Street',
  'city' => 'Pleasantville',
  'state' => 'CA',
  'zip' => '123456',
  'country' => 'USA',
  'phoneNumber' => '123 456 7890',
  'faxNumber' => '789 123 4567',
);

$address = authnet_address_new($values);

dpm($address);

authnet_cim_shipping_address_save($customer_profile_id, $address);

dpm($address);

/**
 * Update an existing shipping address.
 */

$customer_profile_id = XXXXXXXX;
$customer_address_id = XXXXXXXX;

$address = authnet_cim_shipping_address_load($customer_profile_id, $customer_address_id);

dpm($address);

$address->firstName = 'Mike';

authnet_cim_shipping_address_save($customer_profile_id, $address);

dpm($address);

/**
 * Delete a shipping address.
 */

$customer_profile_id = XXXXXXXX;
$customer_address_id = XXXXXXXX;

authnet_cim_shipping_address_delete($customer_profile_id, $customer_address_id);

/**
 * Create a new transaction.
 */

$customer_profile_id = XXXXXXXX;
$customer_payment_profile_id = XXXXXXXX;

$values = array(
  'amount' => '100.00',
  'customerProfileId' => $customer_profile_id,
  'customerPaymentProfileId' => $customer_payment_profile_id,
);

$transaction = authnet_transaction_new($values);

dpm($transaction);

$transaction_id = authnet_cim_transaction($transaction, AUTHNET_METHOD_AUTH_ONLY);

dpm($transaction_id);
