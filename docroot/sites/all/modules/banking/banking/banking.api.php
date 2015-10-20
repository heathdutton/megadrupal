<?php

/**
 * Defines country-specific IBAN formats.
 *
 * @return array
 *   An array of BankingIBANFormat objects.
 */
function hook_banking_iban_format_info() {
  return array(
    new BankingIBANFormat(array(
      'countryCode' => 'XX',
      'example' => 'XX123412341234',
      'pattern' => '/^XX(\d{12, 14})$/',
    )),
  );
}

/**
 * Alters defined country-specific IBAN formats.
 *
 * @param array $formats
 *   An array of BankingIBANFormat objects, keyed by their country codes.
 */
function hook_banking_iban_format_info_alter(array $formats) {
  $formats['XX']->example = $formats['XX']->example . '12';
}
