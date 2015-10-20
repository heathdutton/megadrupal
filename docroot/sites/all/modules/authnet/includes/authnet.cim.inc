<?php

/**
 * @file
 * Authorize.net API CIM wrapper functions
 */

/* ******************************************************************************
 * CIM customer profile wrapper functions
 * *****************************************************************************/

/**
 * Load an existing customer profile from Authorize.net.
 *
 * @param $customer_profile_id
 *   The customer profile id.
 *
 * @return
 *   Returns an AuthorizeNetCustomer object if available, FALSE otherwise.
 */
function authnet_cim_profile_load($customer_profile_id) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Retrieve the customer profile.
  $response = $request->getCustomerProfile($customer_profile_id);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return the profile.
  if ($response->isOK()) {

    // Define the non-unique indices to look for (paymentProfiles and shipToList)
    $nonunique = array('paymentProfiles', 'shipToList');

    // Convert the profile from a SimpleXMLElement to an AuthorizeNetCustomer object.
    $customer_profile = authnet_customer_profile_new(_authnet_simplexml_to_array($response->xml->profile, $nonunique));

    // Invoke hook_authnet_cim_profile_load and pass in the customer profile object.
    module_invoke_all('authnet_cim_profile_load', $customer_profile);

    // Return the profile.
    return $customer_profile;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Save a customer profile to Authorize.net.
 * If the $customer_profile->customerProfileId propery is set, then the profile will be updated.
 * Otherwise, a new one will be saved.
 *
 * @param AuthorizeNetCustomer &$customer_profile
 *   The customer profile object to be saved, passed by reference.
 *
 * @return
 *   Returns TRUE if the save was successful, FALSE otherwise.
 */
function authnet_cim_profile_save(AuthorizeNetCustomer &$customer_profile) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Determine the validation mode.
  $validation = variable_get('authnet_cim_validation_mode', 'none');
  /**
   * @todo
   * The validation mode is currently passed in, but the module doesn't handle the validation responses yet.
   * Use $response->getValidationResponses() to retrieve validation responses, and then parse/display them.
   */

  // If the $customer_profile->customerProfileId propery is set, then update an existing profile.
  if (isset($customer_profile->customerProfileId) && !empty($customer_profile->customerProfileId)) {

    // Authorize.net does not accept payment profiles or ship to addresses when updating a profile, so unset them here.
    // They will be reloaded later.
    unset($customer_profile->paymentProfiles);
    unset($customer_profile->shipToList);

    // Send the request.
    $response = $request->updateCustomerProfile($customer_profile->customerProfileId, $customer_profile);
    $new = FALSE;
  }

  // Otherwise, save a new customer profile.
  else {

    // Only use validation if there are payment profiles assigned to this new customer profile.
    if (empty($customer_profile->paymentProfiles)) {
      $validation = 'none';
    }

    // Send the request.
    $response = $request->createCustomerProfile($customer_profile, $validation);
    $new = TRUE;
  }

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // If a new profile was saved...
    if ($new) {

      // Get the new profile id from the response.
      $customer_profile_id = $response->getCustomerProfileId();

      // If the new customer profile has payment profiles with credit cards, save their expiration date information so that it's available in the insert hook after reloading the profile.
      $expiration_dates = array();
      if (!empty($customer_profile->paymentProfiles)) {
        foreach ($customer_profile->paymentProfiles as $payment_profile) {
          if (!empty($payment_profile->payment->creditCard->expirationDate)) {
            $expiration_dates[$payment_profile->customerPaymentProfileId] = $payment_profile->payment->creditCard->expirationDate;
          }
        }
      }

      // Load the full customer profile object with another call to Authorize.net.
      $customer_profile = authnet_cim_profile_load($customer_profile_id);

      // Restore the expiration dates.
      if (!empty($customer_profile->paymentProfiles)) {
        foreach ($customer_profile->paymentProfiles as $payment_profile) {
          if (!empty($expiration_dates[$payment_profile->customerPaymentProfileId])) {
            $payment_profile->payment->creditCard->expirationDate = $expiration_dates[$payment_profile->customerPaymentProfileId];
          }
        }
      }

      // Invoke hook_authnet_cim_profile_insert and pass in the customer profile object.
      module_invoke_all('authnet_cim_profile_insert', $customer_profile);
    }

    // If the profile is being updated...
    else {

      // Load the full customer profile object with another call to Authorize.net.
      $customer_profile = authnet_cim_profile_load($customer_profile->customerProfileId);

      // Invoke hook_authnet_cim_profile_update and pass in the customer profile object.
      module_invoke_all('authnet_cim_profile_update', $customer_profile);
    }

    // Return TRUE.
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Delete a customer profile from Authorize.net
 *
 * @param $customer_profile_id
 *   The id of the customer profile to be deleted.
 *
 * @return
 *   Returns TRUE on success, FALSE otherwise.
 */
function authnet_cim_profile_delete($customer_profile_id) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Delete the profile.
  $response = $request->deleteCustomerProfile($customer_profile_id);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // Invoke hook_authnet_cim_profile_delete and pass in the customer profile id.
    module_invoke_all('authnet_cim_profile_delete', $customer_profile_id);

    // Return TRUE.
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/* ******************************************************************************
 * CIM payment profile wrapper functions
 * *****************************************************************************/

/**
 * Load an existing customer payment profile from Authorize.net.
 *
 * @param $customer_profile_id
 *   The customer profile id.
 * @param $payment_profile_id
 *   The customer payment profile id.
 *
 * @return
 *   Returns an AuthorizeNetPaymentProfile object if available, FALSE otherwise.
 */
function authnet_cim_payment_profile_load($customer_profile_id, $payment_profile_id) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Retrieve the customer profile.
  $response = $request->getCustomerPaymentProfile($customer_profile_id, $payment_profile_id);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return the profile.
  if ($response->isOK()) {

    // Convert the payment profile from a SimpleXMLElement to an AuthorizeNetPaymentProfile object.
    $payment_profile = authnet_payment_profile_new(_authnet_simplexml_to_array($response->xml->paymentProfile));

    // Invoke hook_authnet_cim_payment_profile_load and pass in the payment profile object.
    module_invoke_all('authnet_cim_payment_profile_load', $customer_profile_id, $payment_profile);

    // Return the profile.
    return $payment_profile;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Save a payment profile to Authorize.net.
 * If the $payment_profile->customerPaymentProfileId propery is set, then the profile will be updated.
 * Otherwise, a new one will be saved.
 *
 * @param $customer_profile_id
 *   The customer profile id.
 * @param AuthorizeNetPaymentProfile &$payment_profile
 *   The payment profile object to be saved, passed by reference.
 *
 * @return
 *   Returns TRUE if the save was successful, FALSE otherwise.
 */
function authnet_cim_payment_profile_save($customer_profile_id, AuthorizeNetPaymentProfile &$payment_profile) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Determine the validation mode.
  $validation = variable_get('authnet_cim_validation_mode', 'none');
  /**
   * @todo
   * The validation mode is currently passed in, but the module doesn't handle the validation responses yet.
   * Use $response->getValidationResponses() to retrieve validation responses, and then parse/display them.
   */

  // If the $payment_profile->customerPaymentProfileId propery is set, then the profile will be updated.
  if (isset($payment_profile->customerPaymentProfileId) && !empty($payment_profile->customerPaymentProfileId)) {
    $response = $request->updateCustomerPaymentProfile($customer_profile_id, $payment_profile->customerPaymentProfileId, $payment_profile, $validation);
    $new = FALSE;
  }

  // Otherwise, save a new payment profile.
  else {
    $response = $request->createCustomerPaymentProfile($customer_profile_id, $payment_profile, $validation);
    $new = TRUE;
  }

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // If a new profile was created...
    if ($new) {

      // Get the new profile id from the response.
      $payment_profile_id = $response->getPaymentProfileId();

      // If the new payment profile has a credit card expiration date, save it so that it's available in the insert hook after reloading the profile.
      $expiration_date = '';
      if (!empty($payment_profile->payment->creditCard->expirationDate)) {
        $expiration_date = $payment_profile->payment->creditCard->expirationDate;
      }

      // Load the full payment profile object with another call to Authorize.net.
      $payment_profile = authnet_cim_payment_profile_load($customer_profile_id, $payment_profile_id);

      // Restore the expiration date.
      if (!empty($expiration_date)) {
        $payment_profile->payment->creditCard->expirationDate = $expiration_date;
      }

      // Invoke hook_authnet_cim_payment_profile_insert and pass in the payment profile object.
      module_invoke_all('authnet_cim_payment_profile_insert', $customer_profile_id, $payment_profile);
    }

    // If the profile is being updated...
    else {

      // Invoke hook_authnet_cim_payment_profile_update and pass in the payment profile object.
      module_invoke_all('authnet_cim_payment_profile_update', $customer_profile_id, $payment_profile);
    }

    // Return TRUE.
    return TRUE;
  }
  elseif ($response->getMessageCode() == 'E00039') {
    // Duplicate customer payment profile exists. Authorize.net matches
    // createCustomerPaymentProfileRequest using the following fields:
    // customerProfileId, creditCard->cardNumber OR (bankAccount->accountNumber
    // & bankAccount->routingNumber), billToFirstName, billToLastName,
    // billToAddress, billToZip.
    // See: https://support.authorize.net/authkb/index?page=content&id=A116
    // Note: During testing it appears that not all billTo fields have to be the
    // same to get a duplicate match.

    $customer_profile = authnet_cim_profile_load($customer_profile_id);
    if (!empty($payment_profile->payment->creditCard->cardNumber)) {
      // Try to match existing profile using simple card number matching.
      $matches = array();
      $payment_profile_id = FALSE;
      foreach ($customer_profile->paymentProfiles as $payment_profile_key => $payment_profile_loaded) {
        $loaded_last_nums = preg_replace('/[^0-9]/', '', $payment_profile_loaded->payment->creditCard->cardNumber);
        $pos = strrpos($payment_profile->payment->creditCard->cardNumber, $loaded_last_nums);
        $pos_expected = strlen($payment_profile->payment->creditCard->cardNumber) - strlen($loaded_last_nums);
        if ($pos === $pos_expected) {
          $matches[] =  $payment_profile_loaded->customerPaymentProfileId;
        }
      }
      if (count($matches) == 1) {
        // Great only one match so it must be it.
        $payment_profile_id = array_shift($matches);
      }
      else {
        // This is a travesty caused by a flaw in the Authorize.net SDK
        // that was reported June 21st 2012. Really the SDK should return the
        // duplicate payment profile ID in the error response.
        // See: http://community.developer.authorize.net/t5/Integration-and-Testing/CIM-createCustomerPaymentProfileRequest-duplicate-entries/td-p/542
        // Try to do the best we can by comparing billing addresses of matched
        // credit card bill to addresses.
        $best_match = FALSE;
        $best_ratio = 0;
        $fields = array('firstName', 'lastName', 'address', 'zip');
        $fields_total = count($fields);
        foreach ($matches as $payment_profile_key => $payment_profile_loaded) {
          $bill_to_loaded = (array) $customer_profile->paymentProfiles[$payment_profile_key]->billTo;
          $bill_to = (array) $payment_profile->billTo;
          $fields_match = 0;
          foreach($bill_to as $field_key => $field_value) {
            if (in_array($field_key, $fields) && !empty($bill_to_loaded[$field_key]) && $field_value == $bill_to_loaded[$field_key]) {
              $fields_match++;
            }
          }
          $field_ratio = $fields_match/$fields_total;
          if ($field_ratio > $best_ratio) {
            $best_ratio = $field_ratio;
            $best_match = $payment_profile_key;
          }
        }

        // Normally such matching would not be advised but we have to remember
        // that Authorize.net has already indicated that one payment profile is
        // a match so we pick one that has the most matching fields.
        $payment_profile_id = $customer_profile->paymentProfiles[$best_match]->customerPaymentProfileId;
      }

      if ($payment_profile_id) {
        $payment_profile->customerPaymentProfileId = $payment_profile_id;
        $payment_profile->is_default = TRUE;

        // Invoke hook_authnet_cim_payment_profile_insert to save the entity.
        module_invoke_all('authnet_cim_payment_profile_insert', $customer_profile_id, $payment_profile);

        // Return TRUE so that the transaction can continue.
        return TRUE;
      }
    }
    elseif (!empty($payment_profile->payment->bankAccount->accountNumber)) {
      // @todo - Do similar comparison to try match bank account numbers.
    }
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Delete a payment profile from Authorize.net
 *
 * @param $customer_profile_id
 *   The customer profile id.
 * @param $payment_profile_id
 *   The id of the payment profile to be deleted.
 *
 * @return
 *   Returns TRUE if successful, FALSE otherwise.
 */
function authnet_cim_payment_profile_delete($customer_profile_id, $payment_profile_id) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Delete the profile.
  $response = $request->deleteCustomerPaymentProfile($customer_profile_id, $payment_profile_id);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, or if the record doesn't exist in Authorize.net (message code 'E00040')...
  if ($response->isOk() || ($response->xml->messages->message->code == 'E00040')) {

    // Invoke hook_authnet_cim_payment_profile_delete and pass in the payment profile id.
    module_invoke_all('authnet_cim_payment_profile_delete', $customer_profile_id, $payment_profile_id);

    // Return TRUE.
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/* ******************************************************************************
 * CIM shipping address wrapper functions
 * *****************************************************************************/

/**
 * Load an existing customer shipping address from Authorize.net.
 *
 * @param $customer_profile_id
 *   The customer profile id.
 * @param $customer_address_id
 *   The customer address id id.
 *
 * @return
 *   Returns an AuthorizeNetAddress object if available, FALSE otherwise.
 */
function authnet_cim_shipping_address_load($customer_profile_id, $customer_address_id) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Retrieve the customer shipping address.
  $response = $request->getCustomerShippingAddress($customer_profile_id, $customer_address_id);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return the address.
  if ($response->isOK()) {

    // Convert the address from a SimpleXMLElement to an AuthorizeNetAddress object.
    $address = authnet_address_new(_authnet_simplexml_to_array($response->xml->address));

    // Return the address.
    return $address;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Save a shipping address to Authorize.net.
 * If the $address->customerAddressId propery is set, then the address will be updated.
 * Otherwise, a new one will be saved.
 *
 * @param $customer_profile_id
 *   The customer profile id.
 * @param AuthorizeNetAddress &$address
 *   The address object to be saved, passed by reference.
 *
 * @return
 *   Returns an AuthorizeNetAddress object if available, FALSE otherwise.
 */
function authnet_cim_shipping_address_save($customer_profile_id, AuthorizeNetAddress &$address) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // If the $address->customerAddressId propery is set, then the profile will be updated.
  if (isset($address->customerAddressId) && !empty($address->customerAddressId)) {
    $response = $request->updateCustomerShippingAddress($customer_profile_id, $address->customerAddressId, $address);
    $new = FALSE;
  }

  // Otherwise, save a new address.
  else {
    $response = $request->createCustomerShippingAddress($customer_profile_id, $address);
    $new = TRUE;
  }

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful...
  if ($response->isOk()) {

    // If a new address was created...
    if ($new) {

      // Get the new address id from the response.
      $customer_address_id = $response->getCustomerAddressId();

      // Load the full address object with another call to Authorize.net.
      $address = authnet_cim_shipping_address_load($customer_profile_id, $customer_address_id);
    }

    // Return the address.
    return $address;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/**
 * Delete a shipping address from Authorize.net
 *
 * @param $customer_profile_id
 *   The customer profile id.
 * @param $customer_address_id
 *   The customer address id.
 *
 * @return
 *   Returns TRUE if successful, FALSE otherwise.
 */
function authnet_cim_shipping_address_delete($customer_profile_id, $customer_address_id) {

  // Create an Authorize.net request object.
  $request = authnet_request_new('CIM');

  // Delete the address.
  $response = $request->deleteCustomerShippingAddress($customer_profile_id, $customer_address_id);

  // Log the response object.
  authnet_log_response($response);

  // If the request was successful, return TRUE.
  if ($response->isOk()) {
    return TRUE;
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}

/* ******************************************************************************
 * CIM transaction wrapper functions
 * *****************************************************************************/

/**
 * Send a new transaction to Authorize.net.
 *
 * @param $transaction
 *   The AuthorizeNetTransaction object that contains all of the transaction data.
 * @param $method
 *   The method of transaction to perform. Must be one of the defined AUTHNET_METHOD_* constants (see authnet.module).
 *
 * @return
 *   Returns TRUE if successful, FALSE otherwise.
 */
function authnet_cim_transaction(AuthorizeNetTransaction $transaction, $method = AUTHNET_METHOD_AUTH_CAPTURE) {

  // Define the available transaction methods.
  $methods = array(
    AUTHNET_METHOD_AUTH_CAPTURE => 'AuthCapture',
    AUTHNET_METHOD_PRIOR_AUTH_CAPTURE => 'PriorAuthCapture',
    AUTHNET_METHOD_AUTH_ONLY => 'AuthOnly',
    AUTHNET_METHOD_CAPTURE_ONLY => 'CaptureOnly',
    AUTHNET_METHOD_CREDIT => 'Refund',
    AUTHNET_METHOD_VOID => 'Void',
  );

  // If a valid method is set...
  if (isset($methods[$method])) {

    // Create an Authorize.net request object.
    $request = authnet_request_new('CIM');

    // Round the transaction amount to 2 decimal places.
    $transaction->amount = round($transaction->amount, 2);

    // Send the transaction to Authorize.net.
    $response = $request->createCustomerProfileTransaction($methods[$method], $transaction);

    // Log the response object.
    authnet_log_response($response);

    // If the request was successful, return TRUE.
    if ($response->isOk()) {
      return TRUE;
    }
  }

  // Return FALSE if the above didn't return anything else.
  return FALSE;
}
