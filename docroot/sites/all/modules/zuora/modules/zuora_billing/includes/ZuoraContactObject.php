<?php

/**
 * Class ZuoraContactObject
 */
class ZuoraContactObject extends ZuoraObject {
  public $address1;
  public $address2;
  public $city;
  public $country;
  public $county;
  public $firstName;
  public $lastName;
  public $fax;
  public $homePhone;
  public $mobilePhone;
  public $nickname;
  public $otherPhone;
  public $otherPhoneType;
  public $personalEmail;
  public $zipCode;
  public $state;
  public $taxRegion;
  public $workEmail;
  public $workPhone;

  /**
   * {@inheritdoc}
   */
  public function toArray() {
    $data = array(
      'address1' => $this->address1,
      'address2' => $this->address2,
      'city' => $this->city,
      'country' => $this->country,
      'county' => $this->county,
      'fax' => $this->fax,
      'firstName' => $this->firstName,
      'homePhone' => $this->homePhone,
      'lastName' => $this->lastName,
      'mobilePhone' => $this->mobilePhone,
      'nickname' => $this->nickname,
      'otherPhone' => $this->otherPhone,
      'otherPhoneType' => $this->otherPhoneType,
      'personalEmail' => $this->personalEmail,
      'zipCode' => $this->zipCode,
      'state' => $this->state,
      'taxRegion' => $this->taxRegion,
      'workEmail' => $this->workEmail,
      'workPhone' => $this->workPhone,
    );

    return array_filter($data);
  }

  /**
   * @return mixed
   */
  public function getAddress1() {
    return $this->address1;
  }

  /**
   * @param mixed $address1
   *
   * @return ZuoraContactObject
   */
  public function setAddress1($address1) {
    $this->address1 = $address1;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getAddress2() {
    return $this->address2;
  }

  /**
   * @param mixed $address2
   *
   * @return ZuoraContactObject
   */
  public function setAddress2($address2) {
    $this->address2 = $address2;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getCity() {
    return $this->city;
  }

  /**
   * @param mixed $city
   *
   * @return ZuoraContactObject
   */
  public function setCity($city) {
    $this->city = $city;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getCountry() {
    return $this->country;
  }

  /**
   * @return mixed
   */
  public function getCounty() {
    return $this->county;
  }

  /**
   * @param mixed $county
   *
   * @return ZuoraContactObject
   */
  public function setCounty($county) {
    $this->county = $county;
    return $this;
  }

  /**
   * @param mixed $country
   *
   * @return ZuoraContactObject
   */
  public function setCountry($country) {
    if (strlen($country) == 2) {
      $country = zuora_payments_country_code($country);
    }
    $this->country = $country;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getFirstName() {
    return $this->firstName;
  }

  /**
   * @param mixed $firstName
   *
   * @return ZuoraContactObject
   */
  public function setFirstName($firstName) {
    $this->firstName = $firstName;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getLastName() {
    return $this->lastName;
  }

  /**
   * @param mixed $lastName
   *
   * @return ZuoraContactObject
   */
  public function setLastName($lastName) {
    $this->lastName = $lastName;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getFax() {
    return $this->fax;
  }

  /**
   * @param mixed $fax
   *
   * @return ZuoraContactObject
   */
  public function setFax($fax) {
    $this->fax = $fax;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getHomePhone() {
    return $this->homePhone;
  }

  /**
   * @param mixed $homePhone
   *
   * @return ZuoraContactObject
   */
  public function setHomePhone($homePhone) {
    $this->homePhone = $homePhone;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getMobilePhone() {
    return $this->mobilePhone;
  }

  /**
   * @param mixed $mobilePhone
   *
   * @return ZuoraContactObject
   */
  public function setMobilePhone($mobilePhone) {
    $this->mobilePhone = $mobilePhone;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getNickname() {
    return $this->nickname;
  }

  /**
   * @param mixed $nickname
   *
   * @return ZuoraContactObject
   */
  public function setNickname($nickname) {
    $this->nickname = $nickname;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getOtherPhone() {
    return $this->otherPhone;
  }

  /**
   * @param mixed $otherPhone
   *
   * @return ZuoraContactObject
   */
  public function setOtherPhone($otherPhone) {
    $this->otherPhone = $otherPhone;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getOtherPhoneType() {
    return $this->otherPhoneType;
  }

  /**
   * @param mixed $otherPhoneType
   *
   * @return ZuoraContactObject
   */
  public function setOtherPhoneType($otherPhoneType) {
    $this->otherPhoneType = $otherPhoneType;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getPersonalEmail() {
    return $this->personalEmail;
  }

  /**
   * @param mixed $personalEmail
   *
   * @return ZuoraContactObject
   */
  public function setPersonalEmail($personalEmail) {
    $this->personalEmail = $personalEmail;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getZipCode() {
    return $this->zipCode;
  }

  /**
   * @param mixed $zipCode
   *
   * @return ZuoraContactObject
   */
  public function setZipCode($zipCode) {
    $this->zipCode = $zipCode;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getState() {
    return $this->state;
  }

  /**
   * @param mixed $state
   *
   * @return ZuoraContactObject
   */
  public function setState($state) {
    $this->state = $state;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getTaxRegion() {
    return $this->taxRegion;
  }

  /**
   * @param mixed $taxRegion
   *
   * @return ZuoraContactObject
   */
  public function setTaxRegion($taxRegion) {
    $this->taxRegion = $taxRegion;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getWorkEmail() {
    return $this->workEmail;
  }

  /**
   * @param mixed $workEmail
   *
   * @return ZuoraContactObject
   */
  public function setWorkEmail($workEmail) {
    $this->workEmail = $workEmail;
    return $this;
  }

  /**
   * @return mixed
   */
  public function getWorkPhone() {
    return $this->workPhone;
  }

  /**
   * @param mixed $workPhone
   *
   * @return ZuoraContactObject
   */
  public function setWorkPhone($workPhone) {
    $this->workPhone = $workPhone;
    return $this;
  }
}
