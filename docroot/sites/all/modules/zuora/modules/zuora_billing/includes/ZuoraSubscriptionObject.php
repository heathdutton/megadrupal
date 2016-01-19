<?php

/**
 * Class ZuoraSubscriptionObject
 */
class ZuoraSubscriptionObject extends ZuoraObject {
  const TERM_TERMED = 'TERMED';
  const TERM_EVERGREEN = 'EVERGREEN';

  public $accountKey;
  public $termType;
  public $contractEffectiveDate;
  public $serviceActivationDate;
  public $customerAcceptanceDate;
  public $termStartDate;
  public $initialTerm;
  public $autoRenew;
  public $renewalTerm;
  public $notes;
  public $invoiceCollect;
  public $invoiceTargetDate;
  public $RatePlans = array();
  public $custom_fields = array();

  public function toArray() {
    $data = array(
      'accountKey' => $this->accountKey,
      'termType' => $this->termType,
      'contractEffectiveDate' => $this->contractEffectiveDate,
      'serviceActivationDate' => $this->serviceActivationDate,
      'customerAcceptanceDate' => $this->customerAcceptanceDate,
      'termStartDate' => $this->termStartDate,
      'initialTerm' => $this->initialTerm,
      'autoRenew' => $this->autoRenew,
      'renewalTerm' => $this->renewalTerm,
      'notes' => $this->notes,
      'invoiceCollect' => $this->invoiceCollect,
      'invoiceTargetDate' => $this->invoiceTargetDate,
      'RatePlans' => $this->RatePlans,
    );

    foreach ($this->custom_fields as $key => $value) {
      $data[$key . '__c'] = $value;
    }

    return array_filter($data);
  }

  /**
   * @return mixed
   */
  public function getAccountKey() {
    return $this->accountKey;
  }

  /**
   * @param mixed $accountKey
   */
  public function setAccountKey($accountKey) {
    $this->accountKey = $accountKey;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getTermType() {
    return $this->termType;
  }

  /**
   * @param mixed $termType
   */
  public function setTermType($termType) {
    $this->termType = $termType;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getContractEffectiveDate() {
    return $this->contractEffectiveDate;
  }

  /**
   * @param DateTime $contractEffectiveDate
   */
  public function setContractEffectiveDate(DateTime $contractEffectiveDate) {
    $this->contractEffectiveDate = $contractEffectiveDate->format('Y-m-d');
    return $this;
  }

  /**
   * @return mixed
   */
  public function getServiceActivationDate() {
    return $this->serviceActivationDate;
  }

  /**
   * @param DateTime $serviceActivationDate
   */
  public function setServiceActivationDate(DateTime $serviceActivationDate) {
    $this->serviceActivationDate = $serviceActivationDate->format('Y-m-d');
    return $this;
  }

  /**
   * @return mixed
   */
  public function getCustomerAcceptanceDate() {
    return $this->customerAcceptanceDate;
  }

  /**
   * @param DateTime $customerAcceptanceDate
   */
  public function setCustomerAcceptanceDate(DateTime $customerAcceptanceDate) {
    $this->customerAcceptanceDate = $customerAcceptanceDate->format('Y-m-d');
    return $this;
  }

  /**
   * @return mixed
   */
  public function getTermStartDate() {
    return $this->termStartDate;
  }

  /**
   * @param DateTime $termStartDate
   */
  public function setTermStartDate(DateTime $termStartDate) {
    $this->termStartDate = $termStartDate->format('Y-m-d');
    return $this;
  }

  /**
   * @return mixed
   */
  public function getInitialTerm() {
    return $this->initialTerm;
  }

  /**
   * @param mixed $initialTerm
   */
  public function setInitialTerm($initialTerm) {
    $this->initialTerm = $initialTerm;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getAutoRenew() {
    return $this->autoRenew;
  }

  /**
   * @param mixed $autoRenew
   */
  public function setAutoRenew($autoRenew) {
    $this->autoRenew = $autoRenew;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getRenewalTerm() {
    return $this->renewalTerm;
  }

  /**
   * @param mixed $renewalTerm
   */
  public function setRenewalTerm($renewalTerm) {
    $this->renewalTerm = $renewalTerm;
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
   */
  public function setNotes($notes) {
    $this->notes = $notes;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getInvoiceCollect() {
    return $this->invoiceCollect;
  }

  /**
   * @param mixed $invoiceCollect
   */
  public function setInvoiceCollect($invoiceCollect) {
    $this->invoiceCollect = $invoiceCollect;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getInvoiceTargetDate() {
    return $this->invoiceTargetDate;
  }

  /**
   * @param mixed $invoiceTargetDate
   */
  public function setInvoiceTargetDate($invoiceTargetDate) {
    $this->invoiceTargetDate = $invoiceTargetDate;
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
    return $this;
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

}
