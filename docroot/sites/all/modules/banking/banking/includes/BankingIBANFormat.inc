<?php

/**
 * Contains BankingIBANFormat.
 */

/**
 * A country-specific IBAN format.
 */
class BankingIBANFormat {

  /**
   * The ISO 3166-1 alpha-2 code of the country the format is for.
   *
   * @var string
   */
  public $countryCode = NULL;

  /**
   * An example of a valid IBAN.
   *
   * @var string
   */
  public $example = NULL;

  /**
   * A PCRE pattern that matches valid IBANs.
   *
   * @var string
   */
  public $pattern = '/^[a-z]{2}\d{2}[a-z\d]{11,30}$/i';

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
}
