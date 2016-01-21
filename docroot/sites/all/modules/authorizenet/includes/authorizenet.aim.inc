<?php

class AuthorizenetAim extends Authorizenet {

  /**
   * Return the Request URL for API activities.
   */
  public function requestUrl() {
    $url = $this->login->developer ? 'test' : 'secure';
    $url = 'https://' . $url . '.authorize.net/gateway/transact.dll';
    return $url;
  }

  /**
   * Return the HTTP request headers for API activities.
   * Unlike all other Authorize.Net APIs, AIM does not use XML.  Return bupkis.
   */
  public function requestHeaders() {
    return array();
  }

  /**
   * Return the request data for an Authorize.Net AIM transaction in a format
   * that is recognized by that API.
   *
   * The following fields are not implemented:
   *    x_tax_exempt
   *    x_allow_partial_Auth
   *    x_authentication_
   *    x_cardholder_authentication_value
   *    x_duplicate_window
   */
  public function transactionRequestData($transaction) {

    // S:Sale C:Credit A:Auth D:Delayed V:Void F:Voice I:Inquiry N:Duplicate
    $trxtypes = array(
      'complete'  => 'AUTH_CAPTURE',
      'authorize' => 'AUTH_ONLY',
      'capture'   => 'PRIOR_AUTH_CAPTURE',
      'void'      => 'VOID',
      'credit'    => 'CREDIT',
    );

    if (!$txntype = $trxtypes[$transaction->transactionType]) {
      drupal_set_message("Payment activity '$transaction->transactionType' is unsupported. Not processing transaction", 'error');
      return FALSE;
    }

    $data = array(
      'x_version'         => '3.1',
      'x_delim_data'      => 'TRUE',
      'x_relay_response'  => 'FALSE',
      'x_login'           => $this->login->name,
      'x_tran_key'        => $this->login->transaction_key,
      'x_type'            => $txntype,
      'x_amount'          => $transaction->amount,
      'x_trans_id'        => $transaction->transId,
      'x_auth_code'       => $transaction->approvalCode,
      'x_split_tender_id' => $transaction->splitTenderId,
      'x_email'           => $this->email,
      'x_cust_id'         => $this->account->uid,
      'x_customer_ip'     => ip_address(),
      'x_header_email_receipt' => '',
      'x_footer_email_receipt' => '',
      'x_email_customer'  => $this->emailCustomer,
      'x_email_merchant'  => $this->emailMerchant,

      // This value is not affiliated with Automated Recurring Billing.
      // If this value is set to TRUE, it automatically disables AVS.
      'x_recurring_billing' => strtoupper($transaction->recurringBilling()),
    );

    // Transaction details.
    if ($this->testMode) {
      $data['x_test_request']  = 'TRUE';
    }

    // Extended amounts.
    foreach (array('duty', 'shipping', 'tax') as $amt) {
      if (!empty($transaction->$amt->amount)) {
        $key = ($amt == 'shipping') ? 'x_freight' : 'x_' . $amt;
        $value = $transaction->$amt->name . '<|>';
        $value .= $transaction->$amt->description . '<|>';
        $value .= $transaction->$amt->amount;
        $data[$key] = $value;
      }
    }

    // Order information.
    if (isset($transaction->order)) {
      $data['x_invoice_num'] = $transaction->order->invoiceNumber;
      $data['x_description'] = $transaction->order->description;
      $data['x_po_num']      = $transaction->order->purchaseOrderNumber;
    }

    // Line items.
    $lineitems = array();
    foreach ($transaction->lineItems as $item) {
      $lineitems[] = join('<|>', array(
        urlencode($item->itemId),
        urlencode($item->name),
        urlencode($item->description),
        urlencode($item->quantity),
        urlencode($item->unitPrice),
        urlencode($item->taxable),
      ));
    }

    // Customer's billing address.
    if (isset($transaction->customerBillingAddress)) {
      $data['x_first_name']  = $transaction->customerBillingAddress->firstName;
      $data['x_last_name']   = $transaction->customerBillingAddress->lastName;
      $data['x_company']     = $transaction->customerBillingAddress->company;
      $data['x_address']     = $transaction->customerBillingAddress->address;
      $data['x_city']        = $transaction->customerBillingAddress->city;
      $data['x_state']       = $transaction->customerBillingAddress->state;
      $data['x_zip']         = $transaction->customerBillingAddress->zip;
      $data['x_country']     = $transaction->customerBillingAddress->country;
      $data['x_phone']       = $transaction->customerBillingAddress->phoneNumber;
      $data['x_fax']         = $transaction->customerBillingAddress->faxNumber;
    }

    // Customer's shipping address.
    if (isset($transaction->customerShippingAddress)) {
      $data['x_ship_to_first_name']  = $transaction->customerShippingAddress->firstName;
      $data['x_ship_to_last_name']   = $transaction->customerShippingAddress->lastName;
      $data['x_ship_to_company']     = $transaction->customerShippingAddress->company;
      $data['x_ship_to_address']     = $transaction->customerShippingAddress->address;
      $data['x_ship_to_city']        = $transaction->customerShippingAddress->city;
      $data['x_ship_to_state']       = $transaction->customerShippingAddress->state;
      $data['x_ship_to_zip']         = $transaction->customerShippingAddress->zip;
      $data['x_ship_to_country']     = $transaction->customerShippingAddress->country;
      $data['x_ship_to_phone']       = $transaction->customerShippingAddress->phoneNumber;
      $data['x_ship_to_fax']         = $transaction->customerShippingAddress->faxNumber;
    }

    // Credit card details.
    if (isset($transaction->creditCard)) {
      $data['x_method']      = 'CC';
      $data['x_card_num']    = $transaction->creditCard->cardNumber;
      $data['x_card_code']   = $transaction->creditCard->cardCode;
      $data['x_exp_date']    = date('m/Y', $transaction->creditCard->expirationTimestamp);
    }

    $req = '';
    foreach ($data as $key => $val) {
      if ($val = trim($val)) {
        $req .= check_plain($key) . '=' . urlencode($val) . '&';
      }
    }

    // Add line items separately, as they share the same field name.
    foreach ($lineitems as $item) {
      $req .= 'x_line_item=' . $item . '&';
    }

    return $req;
  }

  /**
   * Parse a response returned by the Authorize.Net API.
   */
  public function transactionResponse(&$transaction, $response) {
    $transaction->setResponse($response);
  }

  /**
   * Create a transaction using this payment profile.
   */
  function setTransaction($type, $transaction = NULL) {
    if (is_a($transaction, 'AuthorizeNetTransaction')) {
      $this->transaction = $transaction;
    }
    else {
      $this->transaction = New AuthorizeNetTransaction($type);
    }
    return $this->transaction;
  }
}
