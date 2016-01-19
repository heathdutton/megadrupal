<?php

/**
 * This hook lets you add/modify/remove currencies to the currencies list.
 * @param  array $currencies A key/value array with the information of the
 * currencies.
 */
function hook_era_currencies_alter(&$currencies) {
  // For my new country I'll have to add a new currency.
  $currencies['currencies']['TAD'] = "Trully Awesome Dollar";
}

/**
 * This hook lets you add/modify/remove rates to the rates list.
 * @param  array $rates A key/value array with the information of the
 * rates.
 * @param  string $date The date of this rates, useful for historical data. This
 * value will be NULL for current rates.
 */
function hook_era_rates_alter(&$rates, $date) {
  // The base rate is USD, and 1 Trully Awesome Dollar costs USD $2.00
  $rates['rates']['TAD'] = 1/2;
}

/**
 * @file
 * The Exchange Rate API hooks documentation.
 */

/**
 * This hook lets you modify the rate returned by the system just before to be
 * returned.
 * @param  float  $rate The current rate.
 * @param  string $from The currency from which the conversion is calculated.
 * @param  string $to   The currency to which the conversion is calculated.
 * @param  string $date The date of this rates, useful for historical data. This
 * value will be NULL for current rates.
 */
function hook_era_rate_alter(&$rate, $from, $to, $date) {
  // If we are converting from Euro to Dollar, just leave the price as it is.
  if ($from == 'EUR' && $to == 'USD') {
    $rate = 1;
  }
}
