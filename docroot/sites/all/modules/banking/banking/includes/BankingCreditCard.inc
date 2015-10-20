<?php

/**
 * Contains BankingCreditCard.
 */

/**
 * A BIC.
 */
class BankingCreditCard {

  /**
   * American Express.
   */
  const TYPE_AMERICAN_EXPRESS = 0;

  /**
   * Carte Blanche.
   */
  const TYPE_CARTE_BLANCHE = 1;

  /**
   * Diners Club.
   */
  const TYPE_DINERS_CLUB = 2;

  /**
   * Diners Club International.
   */
  const TYPE_DINERS_CLUB_INTERNATIONAL = 3;

  /**
   * Discover
   */
  const TYPE_DISCOVER = 4;

  /**
   * JCB.
   */
  const TYPE_JCB = 5;

  /**
   * Laser.
   */
  const TYPE_LASER = 6;

  /**
   * Maestro.
   */
  const TYPE_MAESTRO = 7;

  /**
   * Mastercard.
   */
  const TYPE_MASTERCARD = 8;

  /**
   * Solo.
   */
  const TYPE_SOLO = 9;

  /**
   * Switch.
   */
  const TYPE_SWITCH = 10;

  /**
   * Visa.
   */
  const TYPE_VISA = 11;

  /**
   * Visa Electron.
   */
  const TYPE_VISA_ELECTRON = 12;

  /**
   * Non-numeric characters that are allowed in the number.
   *
   * @var array
   */
  protected $allowedCharacters = array(' ', '-', '.');

  /**
   * The credit card number.
   *
   * @var string
   */
  protected $number = NULL;

  /**
   * The credit card security code.
   *
   * @var string
   */
  protected $securityCode = NULL;

  /**
   * Constructor.
   *
   * @param array $properties
   *   Keys are property names, and values are property values to set.
   */
  function __construct(array $properties = array()) {
    foreach ($properties as $property => $value) {
      $this->$property = $value;
    }
    unset($properties);
  }

  /**
   * Gets the credit card number.
   *
   * @return string
   */
  public function getNumber() {
    return str_replace($this->allowedCharacters, '', $this->number);
  }

  /**
   * Gets the credit card number.
   *
   * @return string
   */
  public function getSecurityCode() {
    return $this->securityCode;
  }

  /**
   * Validates the credit card number.
   *
   * @param string $message
   *
   * @return bool
   */
  public function validate(&$message = '') {
    if (!$this->validateNumber($message)) {
      return FALSE;
    }
    if (!$this->validateSecurityCode($message)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Validates the credit card security code.
   *
   * @param string $message
   *
   * @return bool
   */
  public function validateSecurityCode(&$message = '') {
    if (!preg_match('/^\d{3,4}$/', $this->getSecurityCode())) {
      $message = t('The security code must consist of 3 or 4 digits.');
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Validates the credit card number.
   *
   * @param string $message
   *
   * @return bool
   */
  function validateNumber(&$message = '') {
    $number = $this->getNumber();
    if (!ctype_digit($number)) {
      return FALSE;
    }

    // Validate the number using the Luhn algorithm.
    $total = 0;
    for ($i = 0; $i < strlen($number); $i++) {
      $digit = substr($number, $i, 1);
      if ((strlen($number) - $i - 1) % 2) {
        $digit *= 2;
        if ($digit > 9) {
          $digit -= 9;
        }
      }
      $total += $digit;
    }

    if ($total % 10 != 0) {
      $message = t('The number %number is not a valid credit card number.', array(
        '%number' => $this->number,
      ));
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Returns the different credit card brands.
   *
   * @return array
   *   Keys are IDs, values are human-readable brand names.
   */
  public static function brandOptions() {
    return array(
      self::TYPE_AMERICAN_EXPRESS => t('American Express'),
      self::TYPE_CARTE_BLANCHE => t('Carte Blanche'),
      self::TYPE_DINERS_CLUB => t('Diners Club'),
      self::TYPE_DINERS_CLUB_INTERNATIONAL => t('Diners Club International'),
      self::TYPE_DISCOVER => t('Discover Card'),
      self::TYPE_JCB => t('JCB'),
      self::TYPE_LASER => t('Laser'),
      self::TYPE_MAESTRO => t('Maestro'),
      self::TYPE_MASTERCARD => t('MasterCard'),
      self::TYPE_SOLO => t('Solo'),
      self::TYPE_SWITCH => t('Switch'),
      self::TYPE_VISA => t('Visa'),
      self::TYPE_VISA_ELECTRON => t('Visa Electron'),
    );
  }
}
