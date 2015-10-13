<?php

/**
 * TODO's - not yet finished with transaction classes.
 */

class Authorizenet {
  // The Authorize.Net login entry for this instance.
  var $login;

  var $emailCustomer;
  var $emailMerchant;
  var $testMode;

  var $email;
  var $account;

  /**
   * Associate Drupal properties with this instance.
   */
  function __construct($login, $account = NULL) {
    $this->setAccount($account);
    $this->login = $login;
  }

  /**
   * Associate a Drupal user account with this instance.
   */
  public function setAccount($account = NULL) {
    if (!isset($account) && !isset($this->account)) {
      global $user;
      $account = $user;
    }
    if ($account) {
      $this->account = $account;
      $this->email = $account->mail;
    }
  }

  public function profile() {
    return New AuthorizenetProfile($this, $this->account);
  }

  /**
   * Return the Request URL for API activities.
   */
  public function requestUrl() {
    $url = $this->login->developer ? 'apitest' : 'api';
    $url = 'https://' . $url . '.authorize.net/xml/v1/request.api';
    return $url;
  }

  /**
   * Return the HTTP request headers for API activities.
   */
  public function requestHeaders() {
    return array('Content-Type' => 'text/xml');
  }

  /**
   * Return the contents of a request for API activities.
   */
  public function requestData($name, $data = NULL) {
    // Basic headers, including request name.
    $xml = '<?xml version="1.0" encoding="utf-8"?>';
    $xml .= '<' . $name . ' xmlns="AnetApi/xml/v1/schema/AnetApiSchema.xsd">';

    // Authentication header.
    $xml .= '<merchantAuthentication>';
    $xml .= '<name>' . $this->login->name . '</name>';
    $xml .= '<transactionKey>' . $this->login->transaction_key . '</transactionKey>';
    $xml .= '</merchantAuthentication>';

    // Request data, provided by calling function.
    $xml .= $data;

    // Close.
    $xml .= '</' . $name . '>';

    return $xml;
  }

  /**
   * Return request data specific to effecting a transaction.
   */
  public function transactionRequestData($transaction) { }

  /**
   * Parse a response returned by the Authorize.Net API.
   */
  public function transactionResponse(&$transaction, $response) {
    $response = preg_replace('/xmlns="(.+?)"/', '', $response);
    $response = new SimpleXMLElement($response);
    $transaction->resultCode = (string) $response->messages->message->code;
    $transaction->result = ($transaction->resultCode[0] == 'I');
    $transaction->resultString = (string) $response->messages->message->text;

    // Dig the CSV values out of an XML response and send them to the parser.
    if ($response = (string) $response->directResponse) {
      $transaction->setResponse($response);
    }
  }

  /**
   * Make an XML request to the Authorize.Net server.
   */
  public function request($name, $data) {
    $host = $this->requestUrl();
    $headers = $this->requestHeaders();
    $xml = $this->requestData($name, $data);
    $ret = drupal_http_request($host, array('headers' => $headers, 'method' => 'POST', 'data' => $xml));

    // See http://community.developer.authorize.net/t5/Integration-and-Testing/ARB-with-SimpleXML-PHP-Issue/td-p/1773
    $ret->data = preg_replace('/xmlns="(.+?)"/', '', $ret->data);

    $result  = new SimpleXMLElement($ret->data);
    $status  = (string) $result->messages->message->code;
    $message = (string) $result->messages->message->text;

    // Result codes begin with I (information) or E (error).j
    $success = $status[0] == 'I';

    $return =  array(
      'status' => $success,
      'data' => $result,
    );
    return (object) $return;
  }

  /**
   * Helper function to output an XML element for a named attribute of an object
   */
  public function XMLElement($object, $name) {
    if (method_exists($object, $name)) {
      $value = $object->$name();
    }
    elseif (isset($object->$name)) {
      $value = $object->$name;
    }

    if ($value) {
      return '<' . $name . '>' . $value . '</' . $name . '>';
    }
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetProfile object.
   */
  public function profileXML($object, $tag = 'profile') {
    if ($profile = $object->$tag()) {
      $xml = $this->XMLElement($profile, 'id');
      $xml .= $this->XMLElement($profile, 'email');
      $xml .= $this->XMLElement($profile, 'phoneNumber');
      $xml .= $this->XMLElement($profile, 'faxNumber');
      if ($xml) {
        $xml = '<' . $tag . '>' . $xml . '</' . $tag . '>';
      }
    }

    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetAddress object.
   */
  public function addressXML($object, $tag = 'billTo') {
    if ($tag) {
      $address = $object->$tag();
    }
    else {
      $address = $object;
    }
    if ($address) {
      $xml = $this->XMLElement($address, 'firstName');
      $xml .= $this->XMLElement($address, 'lastName');
      $xml .= $this->XMLElement($address, 'company');
      $xml .= $this->XMLElement($address, 'address');
      $xml .= $this->XMLElement($address, 'city');
      $xml .= $this->XMLElement($address, 'state');
      $xml .= $this->XMLElement($address, 'zip');
      $xml .= $this->XMLElement($address, 'country');
      $xml .= $this->XMLElement($address, 'phoneNumber');
      $xml .= $this->XMLElement($address, 'faxNumber');
      if ($tag && $xml) {
        $xml = '<' . $tag . '>' . $xml . '</' . $tag . '>';
      }
    }

    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetCreditCard object.
   */
  public function paymentXML($object) {
    if ($object->creditCard) {
      if ($payment_xml = $this->creditCardXML($object->creditCard)) {
        $xml .= '<payment>' . $payment_xml . '</payment>';
      }
    }
    elseif ($object->bankAccount) {
      if ($payment_xml = $this->bankAccountXML($object->bankAccount)) {
        $xml .= '<payment>' . $payment_xml . '</payment>';
      }
    }
    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetCreditCard object.
   */
  public function creditCardXML($creditCard) {
    $xml = '<creditCard>';
    $xml .= $this->XMLElement($creditCard, 'cardNumber');
    $xml .= $this->XMLElement($creditCard, 'expirationDate');
    $xml .= $this->XMLElement($creditCard, 'cardCode');
    $xml .= '</creditCard>';
    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetBankAccount object.
   */
  public function bankAccountXML($bankAccount) {
    $xml = '<bankAccount>';
    $xml .= $this->XMLElement($bankAccount, 'accountType');
    $xml .= $this->XMLElement($bankAccount, 'routingNumber');
    $xml .= $this->XMLElement($bankAccount, 'accountNumber');
    $xml .= $this->XMLElement($bankAccount, 'nameOnAccount');
    $xml .= $this->XMLElement($bankAccount, 'echeckType');
    $xml .= $this->XMLElement($bankAccount, 'bankName');
    $xml .= '</bankAccount>';
    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetExtendedAmount object.
   */
  public function extendedAmountXML($object, $key) {
    if ($object->$key && $object->$key->amount) {
      $xml = '<' . $key . '>';
      $xml .= $this->XMLElement($object->$key, 'amount');
      $xml .= $this->XMLElement($object->$key, 'name');
      $xml .= $this->XMLElement($object->$key, 'description');
      $xml .= '</' . $key . '>';
    }
    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetExtendedAmount object.
   */
  public function lineItemXML($object) {
    if ($object->lineItems()) {
      foreach ($object->lineItems() as $lineItem) {
        $xml .= '<lineItems>';
        $xml .= $this->XMLElement($object, 'itemId');
        $xml .= $this->XMLElement($object, 'name');
        $xml .= $this->XMLElement($object, 'description');
        $xml .= $this->XMLElement($object, 'quantity');
        $xml .= $this->XMLElement($object, 'unitPrice');
        $xml .= $this->XMLElement($object, 'taxable');
        $xml .= '</lineItems>';
      }
    }
    return $xml;
  }

  /**
   * Helper function: generate XML appropriate for a request that deals with
   * an AuthorizenetOrder object.
   */
  public function orderXML($order) {
    $xml = '';
    $xml .= $this->XMLElement($object, 'invoiceNumber');
    $xml .= $this->XMLElement($object, 'description');
    $xml .= $this->XMLElement($object, 'purchaseOrderNumber');

    if ($xml) {
      $xml = '<order>' . $xml . '</order>';
    }
    return $xml;
  }
}

class AuthorizenetProfile {
  // Drupal-specific record-keeping.
  var $parent;
  var $account;

  var $id;
  var $authorizenet_login_id;
  var $uid;
  var $mail;
  var $description;
  var $authorizenet_id;

  function __construct($parent, $account) {
    $this->parent = $parent;
    $this->account = $account;

    if ($this->account->uid) {
      // Retrieve information from the database, if available.
      $res = db_query("SELECT * FROM {authorizenet_profile}
        WHERE authorizenet_login_id = :authorizenet_login_id
        AND (uid != :(uid AND uid = :uid", array(':authorizenet_login_id' => $this->parent->login->id, ':(uid' => 0, ':uid' => $this->account->uid));
      if ($row = db_fetch_array($res)) {
        foreach ($row as $key => $val) {
          $this->$key = $val;
        }
      }
    }
  }

  /**
   * Authorize.Net API property accessor:
   * Payment gateway assigned ID associated with the customer profile.
   */
  function customerProfileId() {
    return $this->authorizenet_id;
  }

  /**
   * Authorize.Net API property accessor:
   * Merchant assigned ID for the customer.  This is used for some API's, while
   * other API'suse merchantCustomerId().
   */
  function id() {
    return $this->account->uid;
  }

  /**
   * Authorize.Net API property accessor:
   * Merchant assigned ID for the customer.  This is used for some API's, while
   * other API'suse id().
   */
  function merchantCustomerId() {
    //@todo UIDs from different sites collide, rely on email for now and
    // configure tokens for this later.
    // return $this->id();
  }

  /**
   * Authorize.Net API property accessor:
   * Description of the customer or customer profile.
   */
  function description() {
    return $this->description;
  }

  /**
   * Authorize.Net API property accessor:
   * Email address associated with the customer profile
   */
  function email() {
    return $this->account->mail;
  }

  /**
   * Authorize.Net API property accessor:
   * Email address associated with the customer profile
   * TODO unimplemented.
   */
  function shipToList() {
    return array();
  }

  /**
   * Authorize.Net API property accessor:
   * The phone number associated with the customer profile.
   * TODO unimplemented.
   */
  function phoneNumber() { }

  /**
   * Authorize.Net API property accessor:
   * The fax number associated with the customer profile.
   * TODO unimplemented.
   */
  function faxNumber() { }

  /**
   * Save the record to the local database for caching and display purposes.
   */
  function save() {
    $this->authorizenet_login_id = $this->parent->login->id;
    $this->uid = $this->account->uid;
    $this->mail = $this->account->mail;

    $update = $this->id ? array('id') : NULL;
    drupal_write_record('authorizenet_profile', $this, $update);
  }
}

class AuthorizenetAddress {
  public $firstName;
  public $lastName;
  public $company;
  public $address;
  public $city;
  public $state;
  public $country;
  public $phoneNumber;
  public $faxNumber;

  /**
   * Set the contact's first name for this address.
   */
  function setFirstName($value) {
    $this->firstName = drupal_substr(filter_xss($value, array()), 0, 50);
  }

  /**
   * Set the contact's last name for this address.
   */
  function setLastName($value) {
    $this->lastName = drupal_substr(filter_xss($value, array()), 0, 50);
  }

  /**
   * Set the company name for this address.
   */
  function setCompany($value) {
    $this->company = drupal_substr(filter_xss($value, array()), 0, 50);
  }

  /**
   * Set the address string for this address.
   */
  function setAddress($value) {
    $this->address = drupal_substr(filter_xss($value, array()), 0, 60);
  }

  /**
   * Set the city for this address.
   */
  function setCity($value) {
    $this->city = drupal_substr(filter_xss($value, array()), 0, 40);
  }

  /**
   * Set the city for this address.
   */
  function setState($value) {
    $this->state = drupal_substr(filter_xss($value, array()), 0, 40);
  }

  /**
   * Set the zip for this address.
   */
  function setZip($value) {
    $this->zip = drupal_substr(filter_xss($value, array()), 0, 20);
  }

  /**
   * Set the country for this address.
   */
  function setCountry($value) {
    $this->country = drupal_substr(filter_xss($value, array()), 0, 60);
  }

  /**
   * Set the phone number for this address.
   */
  function setPhoneNumber($value) {
    $this->phoneNumber = preg_replace('/[^\d\.]/', '', $value);
  }

  /**
   * Set the fax number for this address.
   */
  function setFaxNumber($value) {
    $this->faxNumber = preg_replace('/[^\d\.]/', '', $value);
  }
}

class AuthorizenetCreditCard {
  public $cardNumber;
  public $cardCode;

  function expirationDate() {
    if ($this->expirationTimestamp) {
      return date('Y-m', $this->expirationTimestamp);
    }
  }

  function setCardNumber($value) {
    $value = preg_replace('/[^\d\.]/', '', $value);
    if (strlen($value) >= 13 && strlen($value) <= 16) {
      $this->cardNumber = $value;
    }
  }

  function setExpirationDate($value) {
    // Value is a timestamp.  Just set it.
    if (is_numeric($value) && $value) {
      $this->expirationTimestamp = $value;
    }

    // Value is an array of ('month' => x, 'year' => y). Derive its timestamp.
    elseif (is_array($value) && $value) {
      if (strlen($value['year']) == 2) {
        $value['year'] = '20' . $value['year'];
      }
      $value = mktime(0, 0, 0, $value['month'] + 1, 1, $value['year']);

      $this->expirationTimestamp = $value - 1;
    }
  }

  function setCardCode($value) {
    $value = preg_replace('/[^\d\.]/', '', $value);
    if (strlen($value) == 3 || strlen($value) == 4) {
      $this->cardCode = $value;
    }
  }

  function cardMask() {
    $mask = isset($this->account_mask) ? $this->account_mask : substr($this->cardNumber, -4);
    return str_pad($mask, 16, '*', STR_PAD_LEFT);
  }

  function valid() {
    $valid = TRUE;
    if (!$this->cardNumber) {
      $valid = FALSE;
    }
    if ((int) $this->expirationTimestamp < REQUEST_TIME) {
      $valid = FALSE;
    }
    return $valid;
  }
}

class AuthorizenetBankAccount {
  public $accountType;
  public $routingNumber;
  public $accountNumber;
  public $nameOnAccount;
  public $echeckType;
  public $bankName;

  function setAccountType($value) {
    if (in_array($value, array('checking', 'savings', 'businessChecking'))) {
      $this->accountType = $value;
    }
  }

  function setRoutingNumber($value) {
    $value = preg_replace('/[^\d\.]/', '', $value);
    if (strlen($value) == 9) {
      $this->routingNumber = $value;
    }
  }

  function setAccountNumber($value) {
    $value = preg_replace('/[^\d\.]/', '', $value);
    if (strlen($value) >= 5 && strlen($value) <= 17) {
      $this->accountNumber = $value;
    }
  }

  function setNameOnAccount($value) {
    $value = filter_xss($value, array());
    $this->nameOnAccount = drupal_substr($value, 0, 22);
  }

  function setEcheckType($value) {
    if (in_array($value, array('CCD', 'PPD', 'TEL', 'WEB', 'ARC', 'BOC'))) {
      $this->echeckType = $value;
    }
  }

  function setBankName($value) {
    $value = filter_xss($value, array());
    $this->bankName = drupal_substr($value, 0, 50);
  }

  function valid() {
    $valid = TRUE;
    if (!$this->routingNumber) {
      $valid = FALSE;
    }
    if (!$this->accountNumber) {
      $valid = FALSE;
    }
    if (!$this->nameOnAccount) {
      $valid = FALSE;
    }

    return $valid;
  }
}

class AuthorizenetExtendedAmount {
  public $amount;
  public $name;
  public $description;

  /**
   * As a simple class, allow the amount, name and description to be included
   * in the constructor.
   */
  function __construct($amount = NULL, $name = NULL, $description = NULL) {
    if (!is_null($amount)) {
      $this->setAmount($amount);
    }
    if (!is_null($name)) {
      $this->setName($name);
    }
    if (!is_null($description)) {
      $this->setDescription($description);
    }
  }

  /**
   * Set the amount for this extended amount item. Up to 4 digits after the
   * decimal point (no dollar symbol) Ex. 12.99 or 12.9999
   */
  function setAmount($value) {
    $this->amount = round((float) preg_replace('/[^\d\.]/', '', $value), 4);
  }

  /**
   * Set the amount for this amount item. Up to 31 characters.
   */
  function setName($value) {
    $value = filter_xss($value, array());
    $this->name = drupal_substr($value, 0, 31);
  }

  /**
   * Set the description this amount item. Up to 255 characters.
   */
  function setDescription($value) {
    $value = filter_xss($value, array());
    $this->name = drupal_substr($value, 0, 255);
  }
}

class AuthorizenetLineItem {
  public $itemId;
  public $name;
  public $description;
  public $quantity;
  public $unitPrice;
  public $taxable;

  /**
   * As a simple class, allow the line item details to be included in a call to
   * the constructor.
   */
  function __construct($itemId = NULL, $name = NULL, $description = NULL, $quantity = NULL, $unitPrice = NULL, $taxable = NULL) {
    if (!is_null($itemId)) {
      $this->setItemId($itemId);
    }
    if (!is_null($name)) {
      $this->setName($name);
    }
    if (!is_null($description)) {
      $this->setDescription($description);
    }
    if (!is_null($quantity)) {
      $this->setQuantity($quantity);
    }
    if (!is_null($unitPrice)) {
      $this->setUnitPrice($unitPrice);
    }
    if (!is_null($taxable)) {
      $this->setTaxable($taxable);
    }
  }

  /**
   * Set the itemId for this line item. Up to 31 characters.
   */
  function setItemId($value) {
    $value = filter_xss($value, array());
    $this->itemId = drupal_substr($value, 0, 31);
  }

  /**
   * Set the name for this line item. Up to 31 characters.
   */
  function setName($value) {
    $value = filter_xss($value, array());
    $this->name = drupal_substr($value, 0, 31);
  }

  /**
   * Set the description for this line item. Up to 255 characters.
   */
  function setDescription($value) {
    $value = filter_xss($value, array());
    $this->description = drupal_substr($value, 0, 255);
  }

  /**
   * Set the quantity of an item. Up to 4 digits (up to two decimal places)
   */
  function setQuantity($value) {
    $this->quantity = round((float) preg_replace('/[^\d\.]/', '', $value), 2);
  }

  /**
   * Set the cost of an item per unit excluding tax, freight, and duty.
   * Up to 4 digits with a decimal point (no dollar symbol)
   */
  function setUnitPrice($value) {
    $this->unitPrice = round((float) preg_replace('/[^\d\.]/', '', $value), 2);
  }

  /**
   * Set an indication of whether the item is subject to tax.
   * 'true' or 'false'.
   */
  function setTaxable($value) {
    $this->taxable = (bool) $value ? 'true' : 'false';
  }
}

class AuthorizenetOrder {
  public $invoiceNumber;
  public $description;
  public $purchaseOrderNumber;

  /**
   * As a simple class, allow the order details to be included in a call to
   * the constructor.
   */
  function __construct($invoiceNumber = NULL, $description = NULL, $purchaseOrderNumber = NULL) {
    if (!is_null($invoiceNumber)) {
      $this->setInvoiceNumber($invoiceNumber);
    }
    if (!is_null($description)) {
      $this->setDescription($description);
    }
    if (!is_null($purchaseOrderNumber)) {
      $this->setPurchaseOrderNumber($purchaseOrderNumber);
    }
  }

  /**
   * Set the merchant assigned invoice number for the transaction.
   * Up to 20 characters (no symbols).
   */
  function setInvoiceNumber($value) {
    $value = filter_xss($value, array());
    $this->invoiceNumber = drupal_substr($value, 0, 20);
  }

  /**
   * Set the transaction description.
   * Up to 255 characters (no symbols).
   */
  function setDescription($value) {
    $value = filter_xss($value, array());
    $this->description = drupal_substr($value, 0, 255);
  }

  /**
   * Set the merchant-assigned purchase order number
   * Up to 25 characters (no symbols).
   */
  function setPurchaseOrderNumber($value) {
    $value = filter_xss($value, array());
    $this->purchaseOrderNumber = drupal_substr($value, 0, 25);
  }
}

class AuthorizenetTransaction {
  public $amount;
  public $tax;
  public $shipping;
  public $duty;
  public $lineItems = array();
  public $order;
  public $taxExempt;
  public $recurringBilling;
  public $cardCode;
  public $splitTenderId;
  public $transId; // for Prior Authorization and CaptureTransactions
  public $approvalCode; // capture only
  public $creditCardNumberMasked; // Refund
  public $bankRoutingNumberMasked; // Refund
  public $bankAccountNumberMasked; // Refund
  public $extraOptions;
  public $customerShippingAddress;
  public $customerBillingAddress;

  public $creditCard;

  // Our own elements.
  public $transactionType;
  public $result;
  public $resultCode;
  public $resultString;
  public $resultData;

  function __construct($type) {
    $this->setTransactionType($type);
  }

  /**
   * If the subclass has something to do to complete this transaction, execute
   * that functionality.
   */
  function execute() { }

  /**
   * Parse a response from Authorize.Net's AIM API, or any other API that
   * returns a compatible format.
   *
   * TODO, there are 55 result values, but we won't acknowledge them all here.
   * See http://www.authorize.net/support/AIM_guide.pdf if something is missing.
   */
  function setResponse($response) {
    if ($r = explode(',', ($response))) {

      // Push the array keys out by 1 to correspond with documented fields.
      array_unshift($r, '');

      $this->setTransId($r[7]);
      $this->setApprovalCode($r[5]);

      /* Possible values:
       1 = Approved
       2 = Declined
       3 = Error
       4 = Held for Review
       */
      $this->result = $r[1];
      $this->resultString = $r[4];
      $this->resultData = $r;
    }
  }

  /**
   * Return the tax exempt status.  It's stored internally as a boolean, but
   * the API will expect a string reading 'TRUE' or 'FALSE'.
   */
  function TaxExempt() {
    if (isset($this->taxExempt)) {
      return $this->taxExempt ? 'true' : 'false';
    }
  }

  /**
   * Return the recurring billing status.  It's stored internally as a
   *  boolean, but the API will expect a string reading 'TRUE' or 'FALSE'.
   */
  function recurringBilling() {
    if (isset($this->recurringBilling)) {
      return $this->recurringBilling ? 'true' : 'false';
    }
  }

  function setTransactionType($value) {
    $types = array(
      'complete',  // Authorization and Capture
      'authorize', // Authorization Only
      'capture',   // Capture Only
      'void',      // Void
      'credit',    // Refund
    );
    // TODO - not acknowledging CIM's "Prior Authorization and Capture" trxtype

    if (in_array($value, $types)) {
      $this->transactionType = $value;
    }
  }

  /**
   * Set the total amount of the transaction.
   * Up to 4 digits after the decimal point (no dollar symbol). This amount
   * should include all other amounts such as tax amount, shipping amount, etc.
   */
  function setAmount($value) {
    $this->amount = round((float) preg_replace('/[^\d\.]/', '', $value), 4);
  }

  /**
   * Set the tax information for the transaction.
   * This amount must be included in the total amount for the transaction.
   */
  function setTax($value, $name = NULL, $description = NULL) {
    $this->tax = New AuthorizenetExtendedAmount($value, $name, $description);
  }

  /**
   * Set the shipping information for the transaction.
   * This amount must be included in the total amount for the transaction.
   */
  function setShipping($value, $name = NULL, $description = NULL) {
    $this->shipping = New AuthorizenetExtendedAmount($value, $name, $description);
  }

  /**
   * Set the duty information for the transaction.
   * This amount must be included in the total amount for the transaction.
   */
  function setDuty($value, $name = NULL, $description = NULL) {
    $this->duty = New AuthorizenetExtendedAmount($value, $name, $description);
  }

  /**
   * Set line item details about the order.
   * Up to 30 distinct instances of this element may be included per transaction
   * to describe items included in the order.
   */
  function addLineItem($id = NULL, $name = NULL, $description = NULL, $quantity = NULL, $price = NULL, $taxable = NULL) {
    if (!count($this->lineItems) >= 30) {
      $this->lineItems[] = New AuthorizenetLineItem($id, $name, $description, $quantity, $price, $taxable);
    }
  }

  /**
   * Set the information about the order
   */
  function setOrder($number = NULL, $description = NULL, $po = NULL) {
    $this->order = New AuthorizenetOrder($number, $description, $po);
  }

  /**
   * Set the tax exempt status.
   */
  function setTaxExempt($value) {
    $this->taxExempt = (bool) $value;
  }

  /**
   * Set the recurringBilling status.
   *
   * According to http://www.authorize.net/support/AIM_guide.pdf , this is an
   * Indicating marker used by merchant account providers to identify
   * transactions which originate from merchant hosted recurring billing
   * applications. This value is not affiliated with Automated Recurring Billing
   */
  function setRecurringBilling($value) {
    $this->recurringBilling = (bool) $value;
  }

  /**
   * Set the customer shipping address for this transaction.
   */
  function setCustomerShippingAddress($address) {
    if (is_a($address, 'AuthorizeNetAddress')) {
      return $this->customerShippingAddress = $address;
    }
    else {
      return $this->customerShippingAddress = New AuthorizeNetAddress();
    }
  }

  /**
   * Set the customer billing address for this transaction.
   */
  function setCustomerBillingAddress($address) {
    if (is_a($address, 'AuthorizeNetAddress')) {
      return $this->customerBillingAddress = $address;
    }
    else {
      return $this->customerBillingAddress = New AuthorizeNetAddress();
    }
  }

  /**
   * Set the payment gateway assigned transaction ID of an original transaction.
   */
  function setTransId($value) {
    $this->transId = (int) $value;
  }

  /**
   * Set the authorization code of an original transaction required for a
   * Capture Only. 6 characters.
   */
  function setApprovalCode($value) {
    $value = filter_xss($value, array());
    $this->approvalCode = drupal_substr($value, 0, 6);
  }

  function lineItems() {
    return $this->lineItems;
  }

  /**
   * Set the credit card number for this transaction.  If no credit card object
   * exists for this instance, create one.  setCardNumber must precede calls
   * to setExpirationDate, etc.
   */
  function setCardNumber($value) {
    if (!isset($this->creditCard)) {
      $this->creditCard = New AuthorizenetCreditCard();
    }
    $this->creditCard->setCardNumber($value);
  }

  /**
   * Set the credit card expiration date for this transaction.
   */
  function setExpirationDate($value) {
    if (isset($this->creditCard)) {
      $this->creditCard->setExpirationDate($value);
    }
  }

  /**
   * Set the credit card validation code.
   */
  function setCardCode($value) {
    if (isset($this->creditCard)) {
      $this->creditCard->setCardCode($value);
    }
  }
}
