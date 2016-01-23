<?php

/**
 * Functionality that extends the basic Authorize.Net code with support for
 * the Customer Information Manager API.
 */
class AuthorizenetCim extends Authorizenet {

  /**
   * Return request data specific to effecting a transaction.
   */
  public function transactionRequestData($transaction) {
    if ($transaction) {
      $request = 'createCustomerProfileTransactionRequest';
      $data = $this->cimTransactionXML($transaction);
      return $this->requestData($request, $data);
    }
  }

  /**
   * Override the profile() method to instantiate an AuthorizeNetCimProfile
   * object.
   */
  public function profile() {
    return New AuthorizenetCimProfile($this, $this->account);
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetCimProfile object.
   */
  public function cimProfileXML($profile) {
    $xml = '<profile>';
    $xml .= $this->XMLElement($profile, 'merchantCustomerId');
    $xml .= $this->XMLElement($profile, 'description');
    $xml .= $this->XMLElement($profile, 'email');

    if ($profile->paymentProfiles()) {
      foreach ($profile->paymentProfiles() as $payment_profile) {
        if ($profile_xml = $this->cimPaymentProfileXML($payment_profile)) {
          $xml .= '<paymentProfiles>' . $profile_xml . '</paymentProfiles>';
        }
      }
    }
    $xml .= '</profile>';

    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetCimPaymentProfile object.
   */
  public function cimPaymentProfileXML($payment) {
    $xml = '';
    $xml .= $this->XMLElement($payment, 'customerType');
    $xml .= $this->addressXML($payment, 'billTo');
    $xml .= $this->paymentXML($payment);
    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetCimTransaction object.
   */
  public function cimTransactionXML($transaction) {
    $xml = '';

    // Amount and line items are pertinent to every type but 'void'.
    if ($transaction->transactionType != 'void') {
      $xml .= $this->XMLElement($transaction, 'amount');
      foreach (array('tax', 'shipping', 'duty') as $key) {
        $xml .= $this->extendedAmountXML($transaction, $key);
      }
      $xml .= $this->lineItemXML($transaction);
    }

    // These elements are pertinent to every transaction type.
    $xml .= $this->XMLElement($transaction, 'customerProfileId');
    $xml .= $this->XMLElement($transaction, 'customerPaymentProfileId');
    $xml .= $this->XMLElement($transaction, 'customerShippingAddressId');

    // These elements are pertinent only to authorize / authorize & capture.
    if (in_array($transaction->transactionType, array('authorize', 'complete'))) {
      $xml .= $this->orderXML($transaction);
      $xml .= $this->XMLElement($transaction, 'taxExempt');
      $xml .= $this->XMLElement($transaction, 'recurringBilling');
      $xml .= $this->XMLElement($transaction, 'cardCode');
      $xml .= $this->XMLElement($transaction, 'splitTenderId');
    }

    // 'capture' transactions use a different sequence and require approvalCode.
    elseif ($transaction->transactionType == 'capture') {
      $xml .= $this->orderXML($transaction);
      $xml .= $this->XMLElement($transaction, 'taxExempt');
      $xml .= $this->XMLElement($transaction, 'recurringBilling');
      $xml .= $this->XMLElement($transaction, 'cardCode');
      $xml .= $this->XMLElement($transaction, 'splitTenderId');
      $xml .= $this->XMLElement($transaction, 'approvalCode');
    }

    // 'credit' transactions use a different sequence and require transId.
    elseif ($transaction->transactionType == 'credit') {
      $xml .= $this->XMLElement($transaction, 'creditCardNumberMasked');
      $xml .= $this->XMLElement($transaction, 'bankRoutingNumberMasked');
      $xml .= $this->XMLElement($transaction, 'bankAccountNumberMasked');
      $xml .= $this->orderXML($transaction);
      $xml .= $this->XMLElement($transaction, 'transId');
    }

    // 'void' transactions only contain identifiers and 'transId'
    elseif ($transaction->transactionType == 'void') {
      $xml .= $this->XMLElement($transaction, 'transId');
    }

    if ($xml) {

      // This XML will be wrapped in an element named for its transaction type.
      $type = FALSE;
      switch ($transaction->transactionType) {
        case 'complete':
          $type = 'profileTransAuthCapture';
          break;
        case 'authorize':
          $type = 'profileTransAuthOnly';
          break;
        case 'capture':
          $type = 'profileTransCaptureOnly';
          break;
        case 'void':
          $type = 'profileTransVoid';
          break;
        case 'credit':
          $type = 'profileTransRefund';
          break;
      }

      // If we have a wrapper element, also wrap it with 'transaction'.
      if ($type) {
        $xml = "<transaction><$type>" . $xml . "</$type></transaction>";
      }

      // Otherwise we just don't have anything valid.
      else {
        unset($xml);
      }
    }
    return $xml;
  }

  /**
   * Authorize.Net CIM API: Create a new customer profile along with any
   * customer payment profiles and customer shipping addresses for the
   * customer profile.
   */
  public function createCustomerProfileRequest($profile) {
    $data = $this->cimProfileXML($profile);
    $result = $this->request('createCustomerProfileRequest', $data);

    if ($result->status) {
      // Set the customerProfileId so we can update this record later.
      $profile->setCustomerProfileId( (int) $result->data->customerProfileId);

      // If there were paymentProfile entries, create them locally.
      if ($profile->paymentProfiles()) {

        // We got a list of customerPaymentProfileId's for each submitted one.
        $ids = array();
        foreach ($result->data->customerPaymentProfileIdList->children() as $item) {
          $ids[] = (int) $item;
        }

        foreach ($profile->paymentProfiles() as $i => $payment) {
          $payment->setCustomerPaymentProfileId($ids[$i]);
          $payment->save();
        }
      }

      // TODO: customerShippingAddressIdList , customerPaymentProfileIdList,
      // validationDirectResponseList
    }

    return $result->status;
  }

  /**
   * Authorize.Net CIM API: Create a new customer payment profile for an
   * existing customer profile. You can create up to 10 payment profiles for
   * each customer profile.
   */
  public function createCustomerPaymentProfileRequest($payment) {
    if ($profile_data = $this->cimPaymentProfileXML($payment)) {
      $data = $this->XMLElement($payment, 'customerProfileId');
      $data .= '<paymentProfile>' . $profile_data . '</paymentProfile>';
    }
    $result = $this->request('createCustomerPaymentProfileRequest', $data);
    if ($result->status) {
      $payment->setCustomerPaymentProfileId((int) $result->data->customerPaymentProfileId);
      // TODO: validationDirectResponseList
    }
    else {
      dpr(debug_backtrace());
      print_r($result);
      dpr($data);
      die;
    }

    return $result->status;
  }

  /**
   * Authorize.Net CIM API: Create a new customer shipping address for an
   * existing customer profile. You can create up to 100 customer shipping
   * addresses for each customer profile.
   */
  public function createCustomerShippingAddressRequest($address) {
    if ($data = $this->addressXML($address)) {
      $data = '<address>' . $data . '</address>';
      $data .= $this->XMLElement($address, 'customerProfileId');
    }
    return $this->request('createCustomerShippingAddressRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Create a new payment transaction from an existing
   * customer profile.
   */
  public function createCustomerProfileTransactionRequest($transaction) {
    $data = $this->cimTransactionXML($transaction);
    $result = $this->request('createCustomerProfileTransactionRequest', $data);

    // The CIM API returns a CSV string, in the same format as AIM.
    $response = (string) $result->directResponse;
    return $result->status;
  }

  /**
   * Authorize.Net CIM API: Delete an existing customer profile along with all
   * associated customer payment profiles and customer shipping addresses.
   */
  public function deleteCustomerProfileRequest($profile) {
    if ($data = $this->XMLElement($profile, 'customerProfileId')) {
      return $this->request('deleteCustomerProfileRequest', $data);
    }
  }

  /**
   * Authorize.Net CIM API: Delete a customer payment profile from an existing
   * customer profile.
   */
  public function deleteCustomerPaymentProfileRequest($payment) {
    if ($data = $this->XMLElement($payment, 'customerProfileId')) {
      if ($value = $this->XMLElement($payment, 'customerPaymentProfileId')) {
        $data .= $value;
        return $this->request('deleteCustomerPaymentProfileRequest', $data);
      }
    }
  }

  /**
   * Authorize.Net CIM API: Delete a customer shipping address from an existing
   * customer profile.
   */
  public function deleteCustomerShippingAddressRequest() {
    // TODO
    return $this->request('deleteCustomerShippingAddressRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Retrieve all customer profile IDs you have
   * previously created.
   */
  public function getCustomerProfileIdsRequest() {
    return $this->request('getCustomerProfileIdsRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Retrieve an existing customer profile along with
   * all the associated customer payment profiles and customer shipping
   * addresses.
   */
  public function getCustomerProfileRequest($profile) {
    if ($data = $this->XMLElement($profile, 'customerProfileId')) {
      return $this->request('getCustomerProfileRequest', $data);
    }
  }

  /**
   * Authorize.Net CIM API: Retrieve a customer payment profile for an existing
   * customer profile.
   */
  public function getCustomerPaymentProfileRequest($payment) {
    if ($data = $this->XMLElement($payment, 'customerProfileId')) {
      if ($value = $this->XMLElement($payment, 'customerPaymentProfileId')) {
        $data .= $value;
        return $this->request('getCustomerPaymentProfileRequest', $data);
      }
    }
  }

  /**
   * Authorize.Net CIM API: Retrieve a customer shipping address for an existing
   * customer profile.
   */
  public function getCustomerShippingAddressRequest() {
    //TODO
    return $this->request('getCustomerShippingAddressRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Update an existing customer profile.
   */
  public function updateCustomerProfileRequest() {
    //TODO
    return $this->request('updateCustomerProfileRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Update a customer payment profile for an
   * existing customer profile.
   */
  public function updateCustomerPaymentProfileRequest() {
    //TODO
    return $this->request('updateCustomerPaymentProfileRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Update a shipping address for an existing customer
   * profile.
   */
  public function updateCustomerShippingAddressRequest() {
    //TODO
    return $this->request('updateCustomerShippingAddressRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Update the status of a split tender group (a group
   * of transactions, each of which pays for part of one order).
   */
  public function updateSplitTenderGroupRequest() {
    //TODO
    return $this->request('updateSplitTenderGroupRequest', $data);
  }

  /**
   * Authorize.Net CIM API: Verify an existing customer payment profile by
   * generating a test transaction.
   */
  public function validateCustomerPaymentProfileRequest($payment) {
    // TODO
    return $this->request('validateCustomerPaymentProfileRequest', $data);
  }
}

class AuthorizenetCimProfile extends AuthorizenetProfile {
  var $paymentProfiles;

  function setCustomerProfileId($value) {
    if (!isset($this->authorizenet_id) && is_numeric($value)) {
      $this->authorizenet_id = $value;
    }
  }

  function customerProfileId() {
    return $this->authorizenet_id;
  }

  public function addPaymentProfile($payment = NULL) {
    if (is_a($profile, 'AuthorizenetCimPaymentProfile')) {
      if (!isset($payment->profile)) {
        $payment->profile = $this;
      }
      if ($payment->profile == $this) {
        return $this->paymentProfiles[] = $payment;
      }
    }
    else {
      return $this->paymentProfiles[] = New AuthorizenetCimPaymentProfile($this);
    }
  }

  public function getPaymentProfile($id) {
    $profiles = $this->paymentProfiles(TRUE);
    return $profiles[$id];
  }

  /**
   * Authorize.Net API property accessor:
   * Payment profiles for the customer profile.
   */
  public function paymentProfiles($refresh = FALSE) {
    if ($refresh) {
      if (!isset($this->remotePaymentProfiles)) {
        $this->remotePaymentProfiles = array();
        $remote = $this->parent->getCustomerProfileRequest($this);
        if (is_object($remote)) {
          foreach ($remote->data->profile->paymentProfiles as $item) {
            $id = (int) $item->customerPaymentProfileId;
            $this->remotePaymentProfiles[$id] = (object) $item;
          }
        }
      }
    }

    if (!isset($this->paymentProfiles)) {
      $this->paymentProfiles = array();

      if ($this->id) {
        $res = db_query("SELECT * FROM {authorizenet_payment_profile}
          WHERE authorizenet_profile_id = :authorizenet_profile_id", array(':authorizenet_profile_id' => $this->id));

        foreach ($res as $row) {
          $profile = New AuthorizenetCimPaymentProfile($this);

          if ($refresh) {
            if (isset($this->remotePaymentProfiles[$row->authorizenet_id])) {
              $item = $this->remotePaymentProfiles[$row->authorizenet_id];
            }
            else {
              // Skip this row if it's not recognized by the API.
              // This might happen if it was deleted outside of Drupal.
              continue;
            }

            // For now, we're not storing address information locally, so add it
            // to the paymentProfile for display and use.
            $address = $profile->setBillTo();
            if (isset($item->billTo)) {
              foreach ($item->billTo->children() as $field) {
                $func = 'set' . $field->getName();
                $value = (string) $field;
                if (method_exists($address, $func)) {
                  $address->$func($value);
                }
              }
            }
          }

          $profile->id = $row->id;
          $profile->setCustomerPaymentProfileId($row->authorizenet_id);
          $profile->setAccountType($row->account_type);
          //$profile->description = $row->description;  // TODO

          // It's a credit card.
          if ($row->expiration) {
            $profile->setCardNumber($row->account_mask);
            $profile->creditCard->account_mask = $row->account_mask;
            $profile->setExpirationDate($row->expiration);
          }

          $this->paymentProfiles[$profile->id] = $profile;
        }
      }
    }
    return $this->paymentProfiles;
  }

  /**
   * Extend the save method from the AuthorizenetProfile class to save the
   * profile to the CIM API before storing it in a database.
   */
  function save() {
    // Create or update this profile via the CIM API.
    if ($this->customerProfileId()) {
      $this->parent->updateCustomerProfileRequest($this);
    }
    else {
      $this->parent->createCustomerProfileRequest($this);
    }

    // Defer to the database-saving part of the parent class.
    parent::save();
  }
}

class AuthorizenetCimPaymentProfile {
  // Authorize.Net API properties.
  public $customerType;
  public $billTo;
  public $payment;

  // Drupal-specific record-keeping.
  public $id;
  public $profile;
  public $accountType;

  function __construct($profile) {
    $this->profile = $profile;
  }

  function setCustomerPaymentProfileId($value) {
    if (!isset($this->authorizenet_id) && is_numeric($value)) {
      $this->authorizenet_id = $value;
    }
  }

  function setBillTo($address = NULL) {
    if (is_a($address, 'AuthorizeNetAddress')) {
      return $this->billTo = $address;
    }
    else {
      return $this->billTo = New AuthorizeNetAddress();
    }
  }

  /**
   * Authorize.Net API property accessor:
   * Payment gateway assigned ID associated with the customer profile.
   */
  function customerProfileId() {
    return $this->profile->authorizenet_id;
  }

  /**
   * Authorize.Net API property accessor:
   * Payment gateway assigned ID associated with the customer payment profile.
   */
  function customerPaymentProfileId() {
    return $this->authorizenet_id;
  }

  /**
   * Authorize.Net API property accessor:
   * 'individual' or 'business'
   */
  function customerType() {
    return $this->customerType;
  }

  /**
   * Authorize.Net API property accessor:
   */
  function billTo() {
    if (isset($this->billTo)) {
      return $this->billTo;
    }
  }

  /**
   * Payment details accessor.  Returns an instance of either
   * authorizenetCreditCard or authorizenetBankAccount, depending on which is
   * available.
   */
  function payment() {
    if ($payment = $this->creditCard()) {
      return $payment;
    }
    if ($payment = $this->bankAccount()) {
      return $payment;
    }
  }

  /**
   * Credit card accessor.
   */
  function creditCard() {
    if (isset($this->creditCard)) {
      return $this->creditCard;
    }
  }

  /**
   * Set the credit card number for this profile.  If no credit card object
   * exists for this instance, create one.  setCardNumber must precede calls
   * to setExpirationDate, etc.
   */
  function setCardNumber($value) {
    if (!isset($this->creditCard)) {
      $this->creditCard = New AuthorizenetCreditCard();
    }
    $this->creditCard->setCardNumber($value);

    // Set the account mask, which will be stored to the local database.
    $this->account_mask = substr($value, -4);
  }

  /**
   * Set the credit card expiration date for this payment profile.
   * Make this value available to both the CIM api (for storage) and to the
   * local database so that we can alert users to upcoming expiries.
   */
  function setExpirationDate($value) {
    if (isset($this->creditCard)) {
      $this->creditCard->setExpirationDate($value);
    }

    // Also set expiration to our local database for notifications, etc.
    $this->expiration = $this->creditCard->expirationTimestamp;
  }

  /**
   * Set the credit card validation code.
   */
  function setCardCode($value) {
    if (isset($this->creditCard)) {
      $this->creditCard->setCardCode($value);
    }
  }

  /**
   * Bankaccount accessor.
   */
  function bankAccount() {
    if (isset($this->bankAccount) && $this->bankAccount->valid()) {
      return $this->bankAccount();
    }
  }

  /**
   * Set the Bank Account type (checking, savings, businessChecking)  for this
   * paymentProfile.
   * If no bankAccount object exists for this instance, create one.
   */
  function setBankAccountType($value) {
    if (!isset($this->bankAccount)) {
      $this->bankAccount = New AuthorizenetBankAccount();
    }
    $this->bankAccount->setAccountType($value);
  }

  /**
   * Set the bank routing number for this paymentProfile.
   * If no bankAccount object exists for this instance, create one.
   */
  function setBankRoutingNumber($value) {
    if (!isset($this->bankAccount)) {
      $this->bankAccount = New AuthorizenetBankAccount();
    }
    $this->bankAccount->setRoutingNumber($value);
  }

  /**
   * Set the bank account number for this paymentProfile.
   * If no bankAccount object exists for this instance, create one.
   */
  function setBankAccountNumber($value) {
    if (!isset($this->bankAccount)) {
      $this->bankAccount = New AuthorizenetBankAccount();
    }
    $this->bankAccount->setAccountNumber($value);

    // Set the account mask, which will be stored to the local database.
    $this->account_mask = substr($value, -4);
  }

  /**
   * Set the bank account name for this paymentProfile.
   * If no bankAccount object exists for this instance, create one.
   */
  function setBankNameOnAccount($value) {
    if (!isset($this->bankAccount)) {
      $this->bankAccount = New AuthorizenetBankAccount();
    }
    $this->bankAccount->setNameOnAccount($value);
  }

  /**
   * Set the echeck type (CCD, PPD, TEL, WEB) for this paymentProfile.
   */
  function setBankEcheckType($value) {
    if (isset($this->bankAccount)) {
      $this->bankAccount->setEcheckType($value);
    }
  }

  /**
   * Set the bank name for this paymentProfile.
   */
  function setBankName($value) {
    if (isset($this->bankAccount)) {
      $this->bankAccount->setEcheckType($value);
    }
  }

  /**
   * Create a transaction using this payment profile.
   */
  function addTransaction($type, $transaction = NULL) {
    if (is_a($transaction, 'AuthorizeNetCimTransaction')) {
      $transaction->profile = $this;
      return $transaction;
    }
    else {
      return New AuthorizeNetCimTransaction($type, $this);
    }
  }

  function setAccountType($value) {
    $this->accountType = $this->account_type = check_plain($value);
  }

  function save() {
    // If the customerProfile associated with this paymentProfile hasn't been
    // saved, we need to save that object instead of this one.  Its call to
    // CreateCustomerPaymentProfileRequest() will result in saving both records.
    if (!$this->customerProfileId()) {
      // Saving the parent profile may result in a call to this->save().
      // Set a variable to avoid
      $this->saveParent = TRUE;
      $this->profile->save();
      unset($this->saveParent);
    }

    // This paymentProfile hasn't been saved before.  Create a new one.
    elseif (!$this->customerPaymentProfileId()) {
      $this->profile->parent->createCustomerPaymentProfileRequest($this);
    }

    // Update an existing instance of this paymentProfile.
    elseif (!isset($this->saveParent)) {
      $this->profile->parent->updateCustomerPaymentProfileRequest($this);
    }

    // Save the record to the local database for caching and display purposes..
    $this->authorizenet_profile_id = $this->profile->id;
    $this->account_type = $this->accountType;
    $this->description = $this->description; // TODO

    $update = $this->id ? array('id') : NULL;
    drupal_write_record('authorizenet_payment_profile', $this);
  }
}

/**
 * Extend the AuthorizenetTransaction class in order to reference customer
 * profiles and payment profiles.
 */
class AuthorizenetCimTransaction extends AuthorizenetTransaction {
  public $customerProfileId;
  public $customerPaymentProfileId;
  public $customerShippingAddressId;

  function __construct($type, $payment) {
    $this->payment = $payment;
    parent::__construct($type);
  }

  /**
   * Effect a request to the CIM API to complete this transaction.
   */
  function execute() {
    $this->payment->profile->parent->createCustomerProfileTransactionRequest($this);
  }

  /**
   * The payment gateway assigned ID associated with the customer profile.
   */
  function customerProfileId() {
    return $this->payment->customerProfileId();
  }

  /**
   * The payment gateway assigned ID associated with the customer payment
   * profile.
   */
  function customerPaymentProfileId() {
    return $this->payment->customerPaymentProfileId();
  }

  /**
   * Set the Payment gateway assigned ID associated with the customer shipping
   * address.
   */
  function customerShippingAddressId() {
    // TODO we'll need to save this and get an Id for it.
    if (isset($this->customerShippingAddress)) {
    }
  }
}
