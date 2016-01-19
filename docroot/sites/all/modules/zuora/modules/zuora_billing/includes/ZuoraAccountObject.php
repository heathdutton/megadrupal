<?php

/**
 * Class ZuoraAccountObject
 */
class ZuoraAccountObject extends ZuoraObject {
  public $batch;
  public $accountNumber;
  public $status;
  public $name;
  public $currency;
  public $notes;
  public $billCycleDay;
  public $auto_pay;
  public $crm_id;
  public $invoiceTemplateId;
  public $communicationProfileId;
  public $paymentGateway;
  public $paymentTerm;
  public $defaultPaymentMethod = array();
  public $custom_fields = array();
  /**
   * @var ZuoraContactObject
   */
  public $bill_to_contact;
  /**
   * @var ZuoraContactObject
   */
  public $sold_to_contact;
  /**
   * @var array
   */
  public $creation_payment_info;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $data = array()) {
    // If this is being constructed from an API result, merge in basicInfo.
    if (isset($data['basicInfo'])) {
      parent::__construct(array_merge($data, $data['basicInfo']));
    }
    else {
      parent::__construct($data);
    }

    if (isset($data['billToContact'])) {
      $this->bill_to_contact = new ZuoraContactObject($data['billToContact']);
    }

    if (isset($data['soldToContact'])) {
      $this->sold_to_contact = new ZuoraContactObject($data['soldToContact']);
    }

    foreach ($data as $key => $value) {
      // Set billing info
      if ($key == 'billingAndPayment') {
        foreach ($value as $billing_key => $billing_value) {
          $this->{$billing_key} = $billing_value;
        }
      }

      // Set custom variables.
      if (strpos($key, '__c') !== FALSE) {
        $this->custom_fields[str_replace('__c', '', $key)] = $value;
      }
    }
  }


  /**
   * {@inheritdoc}
   */
  public function toArray() {
    $data = array(
      'batch' => $this->batch,
      'accountNumber' => $this->accountNumber,
      'name' => $this->name,
      'currency' => $this->currency,
      'notes' => $this->notes,
      'billCycleDay' => $this->billCycleDay,
      'autoPay' => $this->auto_pay,
      'crmId' => $this->crm_id,
      'invoiceTemplateId' => $this->invoiceTemplateId,
      'communicationProfileId' => $this->communicationProfileId,
      'paymentGateway' => $this->paymentGateway,
      'paymentTerm' => $this->paymentTerm,
      'defaultPaymentMethod' => $this->defaultPaymentMethod,
    );

    if (!empty($this->bill_to_contact)) {
      $data['billToContact'] = $this->bill_to_contact->toArray();
    }
    if (!empty($this->sold_to_contact)) {
      $data['soldToContact'] = $this->sold_to_contact->toArray();
    }

    if (isset($this->creation_payment_info['hpmCreditCardPaymentMethodId'])) {
      $data['hpmCreditCardPaymentMethodId'] = $this->creation_payment_info['hpmCreditCardPaymentMethodId'];
    }
    elseif (isset($this->creation_payment_info['creditCard'])) {
      $data['creditCard'] = $this->creation_payment_info['creditCard'];
    }

    foreach ($this->custom_fields as $key => $value) {
      $data[$key . '__c'] = $value;
    }

    return array_filter($data);
  }

  /**
   * @return mixed
   */
  public function getBatch() {
    return $this->batch;
  }

  /**
   * @param mixed $batch
   *
   * @return ZuoraAccountObject
   */
  public function setBatch($batch) {
    $this->batch = $batch;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getAccountNumber() {
    return $this->accountNumber;
  }

  /**
   * @param mixed $accountNumber
   *
   * @return ZuoraAccountObject
   */
  public function setAccountNumber($accountNumber) {
    $this->accountNumber = $accountNumber;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getName() {
    return $this->name;
  }

  /**
   * @param mixed $name
   *
   * @return ZuoraAccountObject
   */
  public function setName($name) {
    $this->name = $name;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getCurrency() {
    return $this->currency;
  }

  /**
   * @param mixed $currency
   *
   * @return ZuoraAccountObject
   */
  public function setCurrency($currency) {
    $this->currency = $currency;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getNotes() {
    return $this->notes;
  }

  /**
   * @param mixed $notes
   *
   * @return ZuoraAccountObject
   */
  public function setNotes($notes) {
    $this->notes = $notes;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getBillCycleDay() {
    return $this->billCycleDay;
  }

  /**
   * @param mixed $billCycleDay
   *
   * @return ZuoraAccountObject
   */
  public function setBillCycleDay($billCycleDay) {
    $this->billCycleDay = $billCycleDay;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getAutoPay() {
    return $this->auto_pay;
  }

  /**
   * @param mixed $auto_pay
   *
   * @return ZuoraAccountObject
   */
  public function setAutoPay($auto_pay) {
    $this->auto_pay = $auto_pay;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getCrmId() {
    return $this->crm_id;
  }

  /**
   * @param mixed $crm_id
   *
   * @return ZuoraAccountObject
   */
  public function setCrmId($crm_id) {
    $this->crm_id = $crm_id;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getInvoiceTemplateId() {
    return $this->invoiceTemplateId;
  }

  /**
   * @param mixed $invoiceTemplateId
   *
   * @return ZuoraAccountObject
   */
  public function setInvoiceTemplateId($invoiceTemplateId) {
    $this->invoiceTemplateId = $invoiceTemplateId;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getCommunicationProfileId() {
    return $this->communicationProfileId;
  }

  /**
   * @param mixed $communicationProfileId
   *
   * @return ZuoraAccountObject
   */
  public function setCommunicationProfileId($communicationProfileId) {
    $this->communicationProfileId = $communicationProfileId;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getPaymentGateway() {
    return $this->paymentGateway;
  }

  /**
   * @param mixed $paymentGateway
   *
   * @return ZuoraAccountObject
   */
  public function setPaymentGateway($paymentGateway) {
    $this->paymentGateway = $paymentGateway;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getPaymentTerm() {
    return $this->paymentTerm;
  }

  /**
   * @param mixed $paymentTerm
   *
   * @return ZuoraAccountObject
   */
  public function setPaymentTerm($paymentTerm) {
    $this->paymentTerm = $paymentTerm;
    return $this;
  }

  /**
   * @return ZuoraContactObject
   */
  public function getBillToContact() {
    return $this->bill_to_contact;
  }

  /**
   * @param ZuoraContactObject $bill_to_contact
   *
   * @return ZuoraAccountObject
   */
  public function setBillToContact(ZuoraContactObject $bill_to_contact) {
    $this->bill_to_contact = $bill_to_contact;
    return $this;
  }

  /**
   * @return ZuoraContactObject
   */
  public function getSoldToContact() {
    return $this->sold_to_contact;
  }

  /**
   * @param ZuoraContactObject $sold_to_contact
   *
   * @return ZuoraAccountObject
   */
  public function setSoldToContact(ZuoraContactObject $sold_to_contact) {
    $this->sold_to_contact = $sold_to_contact;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * @param mixed $status
   */
  public function setStatus($status) {
    $this->status = $status;
  }

  /**
   * @param mixed $creation_payment_info
   *
   * @return ZuoraAccountObject
   */
  public function setCreationPaymentInfo($creation_payment_info) {
    // If this is not an array, we have a HPM reference ID.
    if (!is_array($creation_payment_info)) {
      $this->creation_payment_info = array(
        'hpmCreditCardPaymentMethodId' => $creation_payment_info,
      );
    }
    // Manually entering in credit card information.
    else {
      $this->creation_payment_info = array(
        'creditCard' => $creation_payment_info
      );
    }
    return $this;
  }

  /**
   * Add a custom field to the account.
   *
   * @note Do not append __c, it will automatically be added.
   *
   * @param $key
   * @param $value
   */
  public function addCustomField($key, $value) {
    $this->custom_fields[$key] = $value;
  }

  /**
   * Returns a custom field.
   *
   * @note Do not append __c, it will automatically be added.
   *
   * @param $key
   *
   * @return mixed
   */
  public function getCustomField($key) {
    if (isset($this->custom_fields[$key])) {
      return $this->custom_fields[$key];
    }
    return null;
  }

  /**
   * Returns the default payment method.
   *
   * @note this is only returned from summary.
   *
   * @return array
   */
  public function getDefaultPaymentMethod() {
    return $this->defaultPaymentMethod;
  }

}
