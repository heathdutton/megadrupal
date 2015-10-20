<?php

/**
 * Contains BankingIBAN.
 */

/**
 * An IBAN.
 */
class BankingIBAN {

  /**
   * The IBAN according to ISO 13616.
   *
   * @var string
   */
  public $iban = NULL;

  /**
   * Constructor.
   *
   * @param string $iban
   */
  function __construct($iban) {
    $this->iban = $iban;
  }

  /**
   * Gets all defined IBAN formats.
   *
   * @return array
   *   An array with BankingIBANFormat objects, keyed by country code.
   */
  public static function getFormats() {
    static $formats = NULL;

    if (is_null($formats)) {
      $formats = array();
      foreach (module_invoke_all('banking_iban_format_info') as $format) {
        $formats[$format->countryCode] = $format;
      }
      drupal_alter('banking_iban_format_info', $formats);
    }

    return $formats;
  }

  /**
   * Gets the IBAN's country code.
   *
   * @return string
   */
  public function getCountryCode() {
    return substr($this->iban, 0, 2);
  }

  /**
   * Gets the IBAN's checksum.
   *
   * @return string
   */
  function getChecksum() {
    return substr($this->iban, 2, 2);
  }

  /**
   * Gets the IBAN's Basic Bank Account Number.
   *
   * @return string
   */
  function getBBAN() {
    return substr($this->iban, 4);
  }

  /**
   * Validates an IBAN.
   *
   * @param string $message
   *
   * @return bool
   */
  function validate(&$message = '') {
    require_once DRUPAL_ROOT . '/includes/locale.inc';

    // Check for a correct ISO 13616 formatting.
    $format = new BankingIBANFormat();
    $pattern = $format->pattern;
    unset($format);
    if (!preg_match($pattern, $this->iban)) {
      $message = t('The IBAN %iban must consist of 2 letters, 2 digits, and 11 to 30 letters or digits.', array(
        '%iban' => $this->iban,
      ));
      return FALSE;
    }

    // Check if the IBAN matches a country-specific format.
    $formats = self::getFormats();
    $country_code = $this->getCountryCode();
    if (isset($formats[$country_code])) {
      $pattern = $formats[$country_code]->pattern;
      if (!preg_match($pattern, $this->iban)) {
        $country_options = country_get_list();
        $message = t('The IBAN %iban for %country is incorrect.', array(
          '%iban' => $this->iban,
          '%country' => isset($country_options[$country_code]) ? $country_options[$country_code] : $country_code,
        ));
        if ($format->example) {
          $message .= ' ' . t('Example: @example', array(
            '@example' => $format->example,
          ));
        }
        return FALSE;
      }
    }

    // Validate the checksum.
    if (!$this->validateChecksum()) {
      $message = t('IBAN %iban contains an invalid checksum %checksum.', array(
        '%iban' => $this->iban,
        '%checksum' => $this->getChecksum(),
      ));
      return FALSE;
    }

    // If all checks passed, the IBAN is valid.
    return TRUE;
  }

  /**
   * Validates the IBAN's checksum.
   *
   * @return bool
   */
  public function validateChecksum() {
    $number_alphanumeric = $this->getBBAN() . $this->getCountryCode() . $this->getChecksum();
    $number = ltrim(str_replace(range('A', 'Z'), array_map('strval', range(10, 35)), strtoupper($number_alphanumeric)), '0');

    return bcmod($number, '97') == 1;
  }
}
