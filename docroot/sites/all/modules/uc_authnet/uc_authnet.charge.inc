<?php

/**
 * @file
 * Ubercart Authorize.net Payment Gateway - Charge
 */


/**
 * Helper to uc_authnet_charge().
 * (Callback for payment gateway settings)
 */
function _uc_authnet_charge($order_id, $amount, $data) {
  global $user;

  /**
   * Summary of steps:
   *   1) Gather information.
   *   2) Deal with Ubercart's special reference transaction types.
   *   3) Create a new customer profile and/or payment profile, if necessary.
   *   4) Send the transaction to Authorize.net, if necessary.
   */

  /**
   * Step 1: Gather information.
   *
   * There are two basic ways to get to this function:
   *   1) Order admin terminal (/admin/store/orders/%/credit)
   *   2) Checkout
   *
   * The uc_authnet module modifies both so that it can pass extra information to this function. It adds two basic pieces of information,
   * depending on what the user does via the interface:
   *   -If the user is charging an existing payment profile, the payment profile id is passed in.
   *   -If the user is charging a new card, they can optionally choose to save that card to their payment profiles, in which case
   *    a 'save_card' variable is passed in.
   *
   * The storage of those pieces of information varies based on where it's coming from.
   *   -Order admin terminal ($data as returned by uc_authnet_charge_data_get())
   *     -Information about an existing payment profile that was selected will be available in:
   *       $data['payment_profile_id'] (the payment profile id)
   *     -The boolean for saving a new card will be in:
   *       $data['save_card'] (whether or not a card is to be saved)
   *   -Checkout
   *     -Existing payment profile to be charged will be available in:
   *       $order->payment_details['payment_profile_id']
   *     -Boolean for saving a new card will be in:
   *       $order->payment_details['save_card']
   */

  // Container variables:
  $success = FALSE;  // Keep track of our success. Default pessimism.
  $message = '';  // Default message.
  $payment_profile_id = FALSE;  // Authorize.net CIM payment profile id.
  $transaction_id = FALSE;  // AIM or CIM transaction id.
  $save_card = FALSE;  // Whether or not to save the card as a CIM profile on the user.
  $save_card_on_order = FALSE;  // Whether or not to save the card as a CIM profile on the order.

  // Load the order.
  $order = uc_order_load($order_id);

  // Add statically cached data, if any.
  $data += uc_authnet_charge_data_get();

  // If $data contains the 'save_card' variable, or this wasn't initiated by the admin terminal and $order->payment_details contains the 'checkout_save_card' variable, remember that we're saving a new card.
  if (!empty($data['save_card']) || (empty($data['admin_terminal']) && !empty($order->payment_details['checkout_save_card']))) {
    $save_card = TRUE;
  }

  // If a payment profile id was passed in via the terminal or via checkout, remember it.
  if (!empty($data['payment_profile_id'])) {
    $payment_profile_id = $data['payment_profile_id'];
  }
  elseif (!empty($order->payment_details['payment_profile_id'])) {
    $payment_profile_id = $order->payment_details['payment_profile_id'];
  }

  // If an authorization id is set, set it as the transaction id.
  if (!empty($data['auth_id'])) {
    $transaction_id = $data['auth_id'];
  }

  // If this is a 'prior authorization capture' transaction...
  if ($data['txn_type'] == UC_CREDIT_PRIOR_AUTH_CAPTURE && !empty($data['auth_id'])) {

    // Check to see if the payment profile id was included with the authorization (indicating that it's a CIM authorization).
    if (!empty($order->data['cc_txns']['authorizations'][$data['auth_id']]['payment_profile_id'])) {
      $payment_profile_id = $order->data['cc_txns']['authorizations'][$data['auth_id']]['payment_profile_id'];
    }
  }

  // If a payment profile id is set, load the customer profile id.
  if ($payment_profile_id) {
    $record = authnet_cim_entity_payment_profile_load($payment_profile_id);
    if ($record) {
      $customer_profile_id = $record['customer_profile_id'];
    }
  }

  /**
   * Step 2: Deal with Ubercart's special reference transaction types.
   *
   * Our payment gateway only currently supports UC_CREDIT_REFERENCE_SET (create a reference)
   * and UC_CREDIT_REFERENCE_TXN (perform a transaction on a reference).
   * In the future, we may add support for UC_CREDIT_REFERENCE_REMOVE (remove a reference)
   * and UC_CREDIT_REFERENCE_CREDIT (credit a reference).
   *
   * A type of UC_CREDIT_REFERENCE_SET will happen under two conditions:
   *   1) The Authorize.net payment gateway's 'Default credit transaction type' (variable: 'uc_pg_authnet_cc_txn_type')
   *      is set to 'Set a reference only' (value: 'reference_set'), which means credit cards should not be charged
   *      during checkout, but a reference should be created so that it can be charged later.
   *   2) An admin pressed the 'Set a reference only' button in the order admin credit card terminal.
   *
   * In both cases, we want to create a CIM profile. Where we save the profile depends on $save_card:
   * If the user decided to save the card (by checking the checkbox in the checkout or the admin terminal), then we want to
   * associate the CIM profile with the user.
   * Otherwise, we want to associate it with the order, and add a reference to the order that can be charged later.
   *
   * If the type is UC_CREDIT_REFERENCE_TXN, our job is pretty simple: we just need to create a transaction on the CIM profile
   * that was saved to the order. The customer profile id will be passed in via $data['ref_id'].
   */

  // If the transaction type is one of Ubercart's special reference types...
  $reference_types = array(
    UC_CREDIT_REFERENCE_SET,
    UC_CREDIT_REFERENCE_TXN,
    UC_CREDIT_REFERENCE_REMOVE,
    UC_CREDIT_REFERENCE_CREDIT,
  );
  if (in_array($data['txn_type'], $reference_types)) {
    switch ($data['txn_type']) {

      // Set a reference.
      case UC_CREDIT_REFERENCE_SET:

        // If a payment profile id is selected, it means we came in via checkout (and 'Set a reference only' is the default method), so we just want to set $success to TRUE.
        if (!empty($payment_profile_id)) {
          $success = TRUE;
        }

        // Otherwise, we assume that an admin clicked the 'Set a reference only' button in the admin terminal, which means we DO want to save a new card, either to the customer or the order.
        // So, if $save_card is FALSE, then save the card to the order.
        elseif (!$save_card) {
          $save_card_on_order = TRUE;
        }
        break;

      // Perform a transaction on a given reference.
      case UC_CREDIT_REFERENCE_TXN:

        // Grab the customer profile id from $data['ref_id'].
        $customer_profile_id = $data['ref_id'];
        break;

      // Remove a reference.
      case UC_CREDIT_REFERENCE_REMOVE:
        // Not implemented.
        break;

      // Credit a reference.
      case UC_CREDIT_REFERENCE_CREDIT:
        // Not implemented.
        break;
    }
  }

  /**
   * Step 3: Create a new customer profile and/or payment profile, if necessary.
   *
   * This step involves two scenarios and outcomes:
   *   1) If the user wants to save the card to their profile, it does that.
   *   2) If the user doesn't exist, or if $save_card_on_order was set above, we'll save the card to the order.
   *
   * In the first scenario, we will check to see if the user assigned to the order has a customer profile. If not, we'll create one.
   *
   * Then, we'll add the new credit card as a payment profile to that user's customer profile.
   *
   * If a visitor goes through checkout anonymously, the order won't have a user assigned to it at this point.
   * There are two possible solutions to this:
   *   1) It's possible that the user already has an account (and maybe a customer profile), but they aren't logged in.
   *      If that's the case, Ubercart will assign this order to that user, based on their email address. So we'll use the same
   *      logic here to look up the user id based on the email address.
   *   2) If the user is brand new (their email address doesn't exist), then we'll create a new customer profile, but we'll assign
   *      it to the order rather than the user. Then, in hook_uc_checkout_complete(), we'll move it to the user.
   *
   * The second scenario will be triggered if a) the user doesn't want to save the card, or b) a user wasn't found to save the card to in the
   * first scenario. In this case, we'll save the card to a new customer profile on the order.
   */

  // If the customer wants to save their new card...
  if ($save_card) {

    // Get the order's user id.
    $uid = $order->uid;

    // If the order doesn't have a user assigned to it...
    if (!$uid) {

      // Try to find the user based on the primary email address used on the order.
      // This same logic (almost) can be found in Ubercart's uc_cart_complete_sale_account() function.
      $uid = db_query("SELECT uid FROM {users} WHERE LOWER(mail) = :mail", array(':mail' => drupal_strtolower($order->primary_email)))->fetchField();
    }

    // If a user id is available now, attempt to load their customer profile id.
    if ($uid) {

      // Attempt to load their customer profile id.
      $customer_profile_id = authnet_cim_entity_profile_id_load('user', $uid);

      // If a customer profile id was not found, create a new Authorize.net customer profile.
      if (empty($customer_profile_id)) {

        // Create a new customer profile on the user.
        $customer_profile = _uc_authnet_cim_profile_create('user', $uid);

        // If the customer profile didn't save, bail!
        if (!$customer_profile) {

          // Build a message.
          $message = t('Authorize.Net: Creating CIM profile failed.<br />@text', array('@text' => authnet_get_error_message()));

          // Add an admin order comment.
          uc_order_comment_save($order->order_id, $user->uid, $message, 'admin');

          // Return.
          return _uc_authnet_transaction_result(FALSE, $message);
        }

        // So far so good...
        $success = TRUE;

        // Add an admin order comment.
        uc_order_comment_save($order->order_id, $user->uid, t('Authorize.Net: CIM profile created - @id', array('@id' => $customer_profile->customerProfileId)), 'admin');
      }
    }

    // Otherwise, if we couldn't find a user id, set $save_card_on_order to TRUE so we can save the card to the order instead.
    else {
      $save_card_on_order = TRUE;
    }
  }

  // If we are saving the card on the order...
  if ($save_card_on_order) {

    // Create a new customer profile on the order.
    $customer_profile = _uc_authnet_cim_profile_create('order', $order->order_id);

    // If the customer profile didn't save, bail!
    if (!$customer_profile) {

      // Build a message.
      $message = t('Authorize.Net: Creating CIM profile failed.<br />@text', array('@text' => authnet_get_error_message()));

      // Add an admin order comment.
      uc_order_comment_save($order->order_id, $user->uid, $message, 'admin');

      // Return.
      return _uc_authnet_transaction_result(FALSE, $message);
    }

    // So far so good...
    $success = TRUE;

    // Add an admin order comment.
    uc_order_comment_save($order->order_id, $user->uid, t('Authorize.Net: CIM profile created - @id', array('@id' => $customer_profile->customerProfileId)), 'admin');

    /**
     * If the profile was successfully created, and the user didn't select 'save card', add the customer profile as a reference on the order.
     * @see uc_authnet_uc_checkout_complete()
     *
     * There has been some discussion as to whether we should ALWAYS save a reference to the order, no matter what kind of transaction this is.
     * Currently, however, the reference id used in order references in the customer_profile_id, not the payment_profile_id. So adding a reference
     * would not tell you exactly what payment profile was charged.
     *   ... to be continued!
     */
    if (!$save_card) {
      uc_credit_log_reference($order->order_id, $customer_profile->customerProfileId, $order->payment_details['cc_number']);
    }
  }

  // If a customer profile was created, remember the new customer profile id.
  if (!empty($customer_profile)) {
    $customer_profile_id = $customer_profile->customerProfileId;
  }

  // If the card is being saved (to the user or to the order), and we have a customer profile id, save the new payment profile.
  if (($save_card || $save_card_on_order) && $customer_profile_id) {

    // Create the new payment profile.
    $payment_profile = _uc_authnet_cim_payment_profile_create($customer_profile_id, $order);

    // If the payment profile didn't save, bail!
    if (!$payment_profile) {

      // Build a message.
      $message = t('Authorize.Net: Creating CIM payment profile failed.<br />@text', array('@text' => authnet_get_error_message()));

      // Add an admin order comment.
      uc_order_comment_save($order->order_id, $user->uid, $message, 'admin');

      // Return.
      return _uc_authnet_transaction_result(FALSE, $message);
    }

    // So far so good...
    $success = TRUE;

    // Add an admin order comment.
    uc_order_comment_save($order->order_id, $user->uid, t('Authorize.Net: CIM payment profile created - @id', array('@id' => $payment_profile->customerPaymentProfileId)), 'admin');

    // Remember the payment profile id.
    $payment_profile_id = $payment_profile->customerPaymentProfileId;
  }

  /**
   * Step 4: Send the transaction to Authorize.net, if necessary.
   *
   * We only do this if the transaction type is one of the following:
   *   Authorize and Capture (UC_CREDIT_AUTH_CAPTURE)
   *   Prior authorization capture (UC_CREDIT_PRIOR_AUTH_CAPTURE)
   *   Authorize only (UC_CREDIT_AUTH_ONLY)
   *   Process a transaction on a reference (UC_CREDIT_REFERENCE_TXN)
   *
   * If the transaction type is UC_CREDIT_REFERENCE_TXN, we just want to do one thing: load the customer profile and charge the first
   * payment profile we can find. (more on that in a moment...)
   *
   * If a customer profile id and payment profile id are set, send the transaction to Authorize.net via CIM.
   *
   * Or, if only a customer profile id is set (as would be the case if the transaction type is UC_CREDIT_REFERENCE_TXN),
   * attempt to load the first payment profile id, and process the transaction via CIM.
   *
   * Otherwise, send the transaction to Authorize.net via AIM.
   */

  // Only perform a transaction if the transaction type is one of the following:
  $transaction_types = array(
    UC_CREDIT_AUTH_CAPTURE,
    UC_CREDIT_PRIOR_AUTH_CAPTURE,
    UC_CREDIT_AUTH_ONLY,
    UC_CREDIT_REFERENCE_TXN,
  );
  if (in_array($data['txn_type'], $transaction_types)) {

    // Switch through the transaction types.
    switch ($data['txn_type']) {

      // If the transaction type is UC_CREDIT_REFERENCE_TXN...
      case UC_CREDIT_REFERENCE_TXN:

        // If a customer profile id is set, try to get a payment profile id from it, and perform the transaction via CIM.
        if (!empty($customer_profile_id)) {

          // Load the customer profile from Authorize.net.
          $customer_profile = authnet_cim_profile_load($customer_profile_id);

          // If a customer profile wasn't loaded, or it doesn't have any payment profiles, bail.
          if (!$customer_profile || empty($customer_profile->paymentProfiles)) {

            // Build a message.
            $message = t('Authorize.Net: Charging CIM profile failed.<br />The referenced customer profile id doesn\'t exist, or it doesn\'t have any payment profiles.');

            // Add an admin order comment.
            uc_order_comment_save($order->order_id, $user->uid, $message, 'admin');

            // Return.
            return _uc_authnet_transaction_result(FALSE, $message);
          }

          // Get the first payment profile's id.
          $payment_profile_id = $customer_profile->paymentProfiles[0]->customerPaymentProfileId;
        }
        // We don't break here so that the payment_profile_id that was found can be charged in the next case...

      // If the transaction type is 'Authorize and Capture', 'Authorize Only' or 'Prior authorization capture',
      // check to see if we have a customer and payment profile. If so, send the transaction via CIM.
      // Otherwise, send the transaction via AIM.
      case UC_CREDIT_AUTH_CAPTURE:
      case UC_CREDIT_AUTH_ONLY:
      case UC_CREDIT_PRIOR_AUTH_CAPTURE:

        // If a customer profile id and a payment profile id are set, perform the transaction via CIM.
        if (!empty($customer_profile_id) && !empty($payment_profile_id)) {

          // Perform the transaciton.
          return _uc_authnet_cim_transaction($order, $customer_profile_id, $payment_profile_id, $amount, $data['txn_type'], $transaction_id);
        }

        // Otherwise, send via AIM.
        else {
          return _uc_authnet_aim_transaction($order, $amount, $data);
        }
    }
  }

  // Return the result. This will only run if nothing else above returns first.
  return _uc_authnet_transaction_result($success, $message, uc_authnet_translate_txn_type($data['txn_type']));
}

/* ******************************************************************************
 * Helpers
 * *****************************************************************************/

/**
 * Perform a transaction via CIM.
 *
 * @param $order
 *   The order object.
 * @param $customer_profile_id
 *   The Authorize.net customer profile id.
 * @param $payment_profile_id
 *   The Authorize.net payment profile id.
 * @param $amount
 *   The amount of the transaction.
 * @param $method
 *   The Authorize.net method to use.
 * @param $transaction_id
 *   The transaction id, if this is a 'prior authorization capture' transaction. (optional)
 *
 * @return
 *   Returns a result array for the charge function.
 */
function _uc_authnet_cim_transaction($order, $customer_profile_id, $payment_profile_id, $amount, $method, $transaction_id=NULL) {
  global $user;

  // Assemble the new transaction.
  $values = array(
    'amount' => $amount,
    'customerProfileId' => $customer_profile_id,
    'customerPaymentProfileId' => $payment_profile_id,
    'order' => array(
      'invoiceNumber' => $order->order_id,
    ),
  );
  $transaction = authnet_transaction_new($values);

  // Translate the method to it's Authorize.net equivalent.
  $authnet_method = uc_authnet_translate_txn_type($method);

  // If the transaction method is 'Prior authorization capture'...
  if ($authnet_method == AUTHNET_METHOD_PRIOR_AUTH_CAPTURE) {

    // Set the transaction id.
    $transaction->transId = $transaction_id;

    // Unset the order information (Authorize.net doesn't want it).
    unset($transaction->order);
  }

  // Send the transaction and get the result (translate the method to it's Authorize.net equivalent).
  $success = authnet_cim_transaction($transaction, $authnet_method);

  // Get the CIM response object.
  $response = authnet_get_response();

  // If the transaction failed...
  if (!$success) {

    // Set the comment to the error message..
    $message = t('Authorize.Net: Charging CIM profile failed.<br />@text', array('@text' => authnet_get_error_message()));
  }

  // If the transaction succeeded...
  else {

    // Build a comment that contains information about the transaction.
    $types = authnet_transaction_types();
    $message = t('<b>@type:</b> @amount', array(
      '@type' => $types[$authnet_method],
      '@amount' => uc_currency_format($amount),
    ));

    // If this was an authorization only transaction...
    if ($authnet_method == AUTHNET_METHOD_AUTH_ONLY) {

      // Get the transaction response.
      $transaction_response = $response->getTransactionResponse();
      $transaction_id = $transaction_response->transaction_id;

      // Log the authorization to the order.
      uc_authnet_log_cim_authorization($order->order_id, $payment_profile_id, $transaction_response->transaction_id, $amount);
    }

    // Or, if this was a prior authorization capture...
    elseif ($method == AUTHNET_METHOD_PRIOR_AUTH_CAPTURE) {

      // Log the prior authorization capture on the order.
      uc_credit_log_prior_auth_capture($order->order_id, $transaction_id);
    }
  }

  // If a transaction id is not present, use the payment profile id.
  if (empty($transaction_id)) {
    $transaction_id = $payment_profile_id;
  }

  // Add an admin order comment.
  uc_order_comment_save($order->order_id, $user->uid, $message, 'admin');

  // Return the result.
  return _uc_authnet_transaction_result($success, $message, $authnet_method, $transaction_id);
}

/**
 * Perform a transaction via AIM.
 *
 * This is a chunk copied from Ubercart's core _uc_authorizenet_charge() function,
 * modified to use the new API wrapper functions provided by authnet.module,
 * and cleaned up a bit.
 *
 * @param $order
 *   The order object.
 * @param $amount
 *   The amount of the transaction.
 * @param $data
 *   The data array from the charge function.
 *
 * @return
 *   Returns a result array for the charge function.
 */
function _uc_authnet_aim_transaction($order, $amount, $data) {
  global $user;

  // Build a description of the order for logging in Auth.Net.
  $description = array();
  foreach ((array) $order->products as $product) {
    $description[] = $product->qty . 'x ' . $product->model . ' $' . round($product->price * $product->qty, 2);
  }

  $billing_country = uc_get_country_data(array('country_id' => $order->billing_country));
  $delivery_country = uc_get_country_data(array('country_id' => $order->delivery_country));

  // Format the charge amount.
  $uc_price = uc_currency_format($amount, FALSE, FALSE);

  // Translate the transaction method that was specified to the Authorize.net equivalent.
  $method = uc_authnet_translate_txn_type($data['txn_type']);

  // Build the sale data for the transaction.
  $sale = array(

    // Merchant Information
    'login' => variable_get('authnet_login_id', ''),
    'tran_key' => variable_get('authnet_transaction_key', ''),

    // Transaction Information
    'version' => '3.1',
    'type' => $method,
    // 'method' => $order->payment_method == 'credit' ? 'CC' : 'ECHECK',
    'method' => 'CC',
    // 'recurring_billing' => 'FALSE',
    'amount' => $uc_price,
    'card_num' => $order->payment_details['cc_number'],
    'exp_date' => $order->payment_details['cc_exp_month'] . '/' . $order->payment_details['cc_exp_year'],
    'card_code' => $order->payment_details['cc_cvv'],
    // 'trans_id' => '',
    // 'auth_code' => '',
    'test_request' => variable_get('authnet_sandbox', 0) ? 'TRUE' : 'FALSE',
    'duplicate_window' => variable_get('uc_authnet_aim_duplicate_window', 120),

    // Order Information
    'invoice_num' => $order->order_id,
    'description' => drupal_substr(implode(', ', $description), 0, 255),

    // Customer Information
    'first_name' => drupal_substr($order->billing_first_name, 0, 50),
    'last_name' => drupal_substr($order->billing_last_name, 0, 50),
    'company' => drupal_substr($order->billing_company, 0, 50),
    'address' => drupal_substr($order->billing_street1, 0, 60),
    'city' => drupal_substr($order->billing_city, 0, 40),
    'state' => drupal_substr(uc_get_zone_code($order->billing_zone), 0, 40),
    'zip' => drupal_substr($order->billing_postal_code, 0, 20),
    'country' => !$billing_country ? '' : $billing_country[0]['country_iso_code_2'],
    'phone' => drupal_substr($order->billing_phone, 0, 25),
    // 'fax' => drupal_substr('', 0, 25),
    'email' => drupal_substr($order->primary_email, 0, 255),
    'cust_id' => drupal_substr($order->uid, 0, 20),
    'customer_ip' => drupal_substr(ip_address(), 0, 15),

    // Shipping Information
    'ship_to_first_name' => drupal_substr($order->delivery_first_name, 0, 50),
    'ship_to_last_name' => drupal_substr($order->delivery_last_name, 0, 50),
    'ship_to_company' => drupal_substr($order->delivery_company, 0, 50),
    'ship_to_address' => drupal_substr($order->delivery_street1, 0, 60),
    'ship_to_city' => drupal_substr($order->delivery_city, 0, 40),
    'ship_to_state' => drupal_substr(uc_get_zone_code($order->delivery_zone), 0, 40),
    'ship_to_zip' => drupal_substr($order->delivery_postal_code, 0, 20),
    'ship_to_country' => !$delivery_country ? '' : $delivery_country[0]['country_iso_code_2'],

    // Extra Information
    'delim_data' => 'TRUE',
    'delim_char' => '|',
    'encap_char' => '"',
    'relay_response' => 'FALSE',
    'email_customer' => variable_get('uc_authnet_aim_email_receipt', 0) ? 'TRUE' : 'FALSE',
  );

  // If the transaction type is a "prior authorization capture", add the authorization id.
  if ($method == AUTHNET_METHOD_PRIOR_AUTH_CAPTURE) {
    $sale['trans_id'] = $data['auth_id'];
  }

  // Send the transaction to Authorize.net.
  $success = authnet_aim_transaction($sale, $method);

  // Get the response from Authorize.net.
  $response = authnet_get_response();

  // Get the Authorize.net transaction types.
  $types = authnet_transaction_types();

  // If the transaction failed...
  if (!$success) {

    // Set the comment to the error message..
    $message = t('Credit card payment declined: @text', array('@text' => authnet_get_error_message()));
  }

  // If the transaction succeeded...
  else {

    // Set the comment.
    $message = t('Type: @type<br />ID: @id', array('@type' => $types[$method], '@id' => $response->transaction_id));

    // If this was an authorization only transaction...
    if ($method == AUTHNET_METHOD_AUTH_ONLY) {

      // Log the authorization to the order.
      uc_credit_log_authorization($order->order_id, $response->transaction_id, $amount);
    }

    // Or, if this was a prior authorization capture...
    elseif ($method == AUTHNET_METHOD_PRIOR_AUTH_CAPTURE) {

      // Log the prior authorization capture on the order.
      uc_credit_log_prior_auth_capture($order->order_id, $data['auth_id']);
    }

    /**
     * @todo
     * Not yet implemented...
     * (copied directly from _uc_authorizenet_charge())
     */
    // // Create a transaction reference if specified in the payment gateway
    // // settings and this is an appropriate transaction type.
    // if (variable_get('uc_authnet_cim_profile', FALSE) && in_array($data['txn_type'], array(UC_CREDIT_AUTH_ONLY, UC_CREDIT_AUTH_CAPTURE))) {
    //   // Ignore the returned message for now; that will appear in the comments.
    //   _uc_authorizenet_cim_profile_create($order);
    // }
  }

  // Build an admin order comment.

  $admin_comment = t('<b>@type</b><br /><b>@status:</b> @message<br />Amount: @amount',
    array(
      '@type' => $types[$method],
      '@status' => $success ? t('ACCEPTED') : t('REJECTED'),
      '@message' => $response->response_reason_text,
      '@amount' => uc_currency_format($response->amount),
    )
  );

  // If there was an error, add more information to the admin comment.
  if (!$success) {

    // Add the AVS response, if available.
    if (!empty($response->avs_response)) {
      $admin_comment .= '<br />' . t('AVS response: @avs', array('@avs' => authnet_avs($response->avs_response)));
    }

    // Add the CVV response, if available and enabled.
    if (!empty($response->card_code_response) && variable_get('uc_credit_cvv_enabled', TRUE)) {
      $admin_comment .= '<br />' . t('CVV match: @cvv', array('@cvv' => authnet_cvv($response->card_code_response)));
    }
  }

  // Add an admin order comment.
  uc_order_comment_save($order->order_id, $user->uid, $admin_comment, 'admin');

  // Return the result.
  return _uc_authnet_transaction_result($success, $message, $method, $response->transaction_id, $response->authorization_code);
}

/**
 * Assemble a transaction result.
 *
 * @param $success
 *   Boolean representing the transaction's success.
 * @param $message
 *   A message to include in the result.
 * @param $method
 *   The transaction method used.
 * @param $transaction_id
 *   The transaction id returned by Authorize.net.
 * @param $authcode
 *   The authorization code returned by Authorize.net.
 *
 * @param
 *   Returns a result array, as expected by uc_payment_process().
 *
 * @see _uc_authnet_cim_transaction()
 * @see _uc_authnet_aim_transaction()
 * @see uc_payment_process()
 */
function _uc_authnet_transaction_result($success = NULL, $message = NULL, $method = NULL, $transaction_id = NULL, $authcode = NULL) {
  global $user;

  // Start with some defaults.
  $result = array(
    'success' => FALSE,
    'message' => t('An error occurred during processing. Please consult the order logs, or contact the site administrator.'),
    'uid' => $user->uid,
  );

  // Set the success flag.
  if ($success) {
    $result['success'] = $success;
  }

  // Set the comment and the message.
  if ($message) {
    $result['comment'] = $message;
    $result['message'] = $message;
  }

  // Assemble the data array.
  $result['data'] = array(
    'module' => 'uc_authnet',
  );

  // If the method is set...
  if ($method) {

    // Add it to the data.
    $result['data']['txn_type'] = $method;

    // Don't log this as a payment if money wasn't actually captured.
    $non_payments = array(
      AUTHNET_METHOD_AUTH_ONLY,
    );
    if (in_array($method, $non_payments)) {
      $result['log_payment'] = FALSE;
    }
  }

  // If a transaction id is set, add it to the data.
  if ($transaction_id) {
    $result['data']['txn_id'] = $transaction_id;
  }

  // If an authorization code is set, add it to the data.
  if ($authcode) {
    $result['data']['txn_authcode'] = $authcode;
  }

  // Return the result.
  return $result;
}

/**
 * Create a new CIM profile and assign it to either customer or the order.
 *
 * @param $type
 *   The type of entity to assign the profile to. Should be either 'user' or 'order'.
 * @param $id
 *   The id of the entity.
 *
 * @return
 *   Returns the customer profile if one was successfully created, FALSE otherwise.
 */
function _uc_authnet_cim_profile_create($type, $id) {
  global $user;

  // Create a customer profile based on the type of entity.
  switch ($type) {

    // If the type is 'user', create a new profile for the user.
    case 'user':
      $customer_profile = authnet_user_customer_profile_new($id, FALSE);
      $success = authnet_cim_profile_save($customer_profile);
      break;

    // If the type is 'order', create a new profile for the order.
    case 'order':
      $customer_profile = authnet_customer_profile_new();
      $customer_profile->merchantCustomerId = 'order:' . $id;
      $customer_profile->description = t('Order @order taking place at @date', array('@order' => $id, '@date' => format_date(REQUEST_TIME)));
      $success = authnet_cim_profile_save($customer_profile);
      break;

    // If the type is anything else, return FALSE.
    default:
      return FALSE;
  }

  // If there was an error saving the profile return FALSE.
  if (!$success) {
    return FALSE;
  }

  // Return the customer profile.
  return $customer_profile;
}

/**
 * Create a new payment profile for an order.
 *
 * @param $customer_profile_id
 *   An Authorize.net customer profile id.
 * @param $order
 *   An order object.
 */
function _uc_authnet_cim_payment_profile_create($customer_profile_id, $order) {
  global $user;

  // Assemble the new payment profile information.
  $values = _uc_authnet_payment_profile_from_order($order);

  // Create a new payment profile with the values.
  $payment_profile = authnet_payment_profile_new($values);

  // Send it to Authorize.net
  $success = authnet_cim_payment_profile_save($customer_profile_id, $payment_profile);

  // If there was an error saving the profile, return FALSE.
  if (!$success) {
    return FALSE;
  }

  // Load the new payment profile record from the database and resave it to include the credit card type and default status.
  $record = authnet_cim_entity_payment_profile_load($payment_profile->customerPaymentProfileId);

  // Set the credit card type, if available.
  if (isset($order->payment_details['cc_type'])) {
    $record['cc_type'] = check_plain($order->payment_details['cc_type']);
  }

  // If the customer doesn't have a default payment profile, set this as their default.
  if (!authnet_cim_entity_customer_payment_profile_default($customer_profile_id)) {
    $record['is_default'] = 1;
  }

  // Save the record.
  authnet_cim_entity_payment_profile_save($record);

  // Save the new payment profile id to the order's payment details, so that admins know that this is the preferred profile for this order in the future.
  $order->payment_details['payment_profile_id'] = $payment_profile->customerPaymentProfileId;
  uc_order_save($order);

  // Return the payment profile.
  return $payment_profile;
}

/**
 * Assemble new payment profile info from order
 *
 * Maximum string length reference:
 *   https://sandbox.authorize.net/help/Merchant_Interface_RoboHelp_Project.htm#Tools/Customer_Information_Manager/Customer_Profile_Fields.htm%3E%3Epan=2
 *
 * @param $order
 *   An order object.
 *
 * @return
 *   Returns an assembled array of payment profile values as expected by authnet_payment_profile_new().
 */
function _uc_authnet_payment_profile_from_order($order) {
  return array(
    'customerType' => !empty($order->billing_company) ? 'business' : 'individual',
    'billTo' => array(
      'firstName' => drupal_substr($order->billing_first_name, 0, 50),
      'lastName' => drupal_substr($order->billing_last_name, 0, 50),
      'company' => drupal_substr($order->billing_company, 0, 50),
      'address' => drupal_substr($order->billing_street1, 0, 60),
      'city' => drupal_substr($order->billing_city, 0, 40),
      'state' => drupal_substr(uc_get_zone_code($order->billing_zone), 0, 2),
      'zip' => drupal_substr($order->billing_postal_code, 0, 20),
      'country' => drupal_substr(uc_country_get_by_id($order->billing_country), 0, 60),
      'phoneNumber' => drupal_substr(check_plain($order->billing_phone), 0, 25),
    ),
    'payment' => array(
      'creditCard' => array(
        'cardNumber' => $order->payment_details['cc_number'],
        'expirationDate' => $order->payment_details['cc_exp_year'] . '-' . str_pad($order->payment_details['cc_exp_month'], 2, '0', STR_PAD_LEFT),
        'cardCode' => $order->payment_details['cc_cvv'],
      ),
    ),
  );
}
