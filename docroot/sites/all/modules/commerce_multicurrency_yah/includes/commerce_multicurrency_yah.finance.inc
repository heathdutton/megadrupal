<?php

/**
 * @file
 *
 * The inc file for the Yahoo Finance support for the commerce multicurrency.
 */

/**
 * Fetch the currency exchange rates for the requested currency combination.
 *
 * Use the Yahoo Finance api to get the currency rates.
 *
 * Return an array with the array(target_currency_code => rate) combination.
 *
 * @param string $currency_code
 *   Source currency code.
 * @param array $target_currencies
 *   Array with the target currency codes.
 *
 * @return array $rates
 *   Array with the array(target_currency_code => rate) combination.
 */
function commerce_multicurrency_yah_exchange_rate_sync_provider($currency_code, array $target_currencies) {
  $rates = array();

  if (isset($currency_code) && is_array($target_currencies)) {
    foreach ($target_currencies as $target_currency_code) {
      $rate = commerce_multicurrency_yah_getrate($currency_code, $target_currency_code);
      $rates[$target_currency_code] = $rate;
    }
  }

  return $rates;
}
