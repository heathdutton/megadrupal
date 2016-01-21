<?php

namespace Drupal\cointools_fiat;

use Drupal\cointools_fiat\FiatCurrencies;
use Guzzle\Http\Client as GuzzleClient;

/**
 * @file
 * Contains CoinTools.
 */

class Fiat {

  public static function currencyOptions() {
    $currencies = [];

    foreach (FiatCurrencies::currencies() as $code => $info) {
      $currencies[$code] = $info['label'];
    }

    // Sort currencies by label.
    asort($currencies);

    return ['' => t("None")] + $currencies;
  }

  /**
   * Gets the exchange rate.
   */
  public static function updateRates() {
    $rates = [];

    try {
      $client = new \GuzzleHttp\Client();
      $response = $client->get('https://api.bitcoinaverage.com/ticker/global/all');
      $data = $response->json();

      foreach ($data as $code => $info) {
        // Check that there is data.
        if (isset($info['last']) && $info['last'] != 0) {
          $rates[$code] = (float) $info['last'];
        }
      }
      if (!count($rates)) {
        throw new \Exception();
      }
      $timestamp = strtotime($data['timestamp']);
      $source = 'bitcoinaverage.com';
    }
    catch (\Exception $e) {
      try {
        $client = new \GuzzleHttp\Client();
        $response = $client->get('https://bitpay.com/api/rates');
        $data = $response->json();

        foreach ($data as $info) {
          // Check that there is data.
          if (isset($info['rate']) && $info['rate'] != 0) {
            $rates[$info['code']] = (float) $info['rate'];
          }
        }
        if (!count($rates)) {
          throw new \Exception();
        }
        $timestamp = time();
        $source = 'bitpay.com';
      }
      catch (\Exception $e) {
        // Don't update rates.
        return;
      }
    }

    variable_set('cointools_fiat.rates', $rates);
    variable_set('cointools_fiat.timestamp', $timestamp);
    variable_set('cointools_fiat.source', $source);
  }

  /**
   * Gets the exchange rate.
   */
  public static function getRates($max_age = 900) {
    if ($max_age && (time() - variable_get('cointools_fiat.timestamp') > $max_age)) {
      Fiat::updateRates();
    }
    return variable_get('cointools_fiat.rates');
  }

  public static function currentSource() {
    switch (variable_get('cointools_fiat.source')) {
      case 'bitcoinaverage.com':
        $source = '<a href="https://bitcoinaverage.com/" target="_blank">BitcoinAverage</a>';
        break;

      case 'bitpay.com':
        $source = '<a href="https://bitpay.com/" target="_blank">BitPay</a>';
        break;
    }

    return '<p>' . t("Source: ") . $source . '</p>';
  }

}
