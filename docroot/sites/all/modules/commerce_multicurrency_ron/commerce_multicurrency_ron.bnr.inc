<?php

/**
 * Fetch the currency exchange rates for the requested currency combination.
 * Use CBR.ru as provider.
 *
 * Return an array with the array(target_currency_code => rate) combination.
 *
 * @param string $currency_code
 *   Source currency code.
 * @param array $target_currencies
 *   Array with the target currency codes.
 *
 * @return array
 *   Array with the array(target_currency_code => rate) combination.
 */
function commerce_multicurrency_ron_exchange_rate_sync_provider_bnr($currency_code, $target_currencies) {
  $data = cache_get(__FUNCTION__, 'cache');

  if (!$data) {
    $bnr_rates = array();
    if ($xml = @simplexml_load_file('http://bnr.ro/nbrfxrates.xml')) {
      foreach($xml->Body->Cube->Rate as $line) {
        $rate = (float)$line;
        $currency = (string)$line['currency'];
        $multiplier = (int)$line['multiplier'];
        $bnr_rates[$currency] = empty($multiplier) ? $rate : $multiplier / $rate;
      }
      cache_set(__FUNCTION__, $bnr_rates, 'cache', time() + 3600);
    }
    else {
      watchdog(
        'commerce_multicurrency', 'Rate provider BNR: Unable to fetch / process the currency data of @url',
        array('@url' => 'http://bnr.ro/nbrfxrates.xml'),
        WATCHDOG_ERROR
      );
    }
  }
  else {
    $bnr_rates = $data->data;
  }

  $rates = array();
  foreach ($target_currencies as $target_currency_code) {
    if ($currency_code == 'RON' && isset($bnr_rates[$target_currency_code])) {
      $rates[$target_currency_code] = 1 / $bnr_rates[$target_currency_code];
    }
    elseif (isset($bnr_rates[$currency_code]) && $target_currency_code == 'RON') {
      $rates[$target_currency_code] = $bnr_rates[$currency_code];
    }
    elseif (isset($bnr_rates[$currency_code]) && isset($bnr_rates[$target_currency_code])) {
      $rates[$target_currency_code] = $bnr_rates[$target_currency_code] / $bnr_rates[$currency_code];
    }
  }

  return $rates;
}
