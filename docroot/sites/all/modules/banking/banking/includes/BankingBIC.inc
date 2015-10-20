<?php

/**
 * Contains BankingBIC.
 */

/**
 * A BIC.
 */
class BankingBIC {

  /**
   * The BIC according to ISO 9362.
   *
   * @var string
   */
  public $bic = NULL;

  /**
   * Constructor.
   *
   * @param string $bic
   */
  function __construct($bic) {
    $this->bic = $bic;
  }

  /**
   * Gets the BIC's country code.
   *
   * @return string
   */
  public function getCountryCode() {
    return substr($this->bic, 4, 2);
  }

  /**
   * Gets the BIC's institution (bank) code.
   *
   * @return string
   */
  function getInstitutionCode() {
    return substr($this->bic, 0, 4);
  }

  /**
   * Gets the BIC's location code.
   *
   * @return string
   */
  function getLocationCode() {
    return substr($this->bic, 6, 2);
  }

  /**
   * Gets the BIC's location code.
   *
   * @return string
   */
  function getBranchCode() {
    $branch_code = substr($this->bic, 8, 3);

    return strlen($branch_code) ? $branch_code : 'XXX';
  }

  /**
   * Validates the BIC.
   *
   * @param string $message
   *
   * @return bool
   */
  function validate(&$message = '') {
    if (!preg_match('/^[a-z]{4}[a-z]{2}[a-z\d]{2}([a-z\d]{3})?$/i', $this->bic)) {
      $message = t('BIC %bic must consist of 6 letters, and 2 letters or digits, optionally followed by 3 more letters or digits.', array(
        '%bic' => $this->bic,
      ));
      return FALSE;
    }

    return TRUE;
  }
}
