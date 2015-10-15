<?php

/**
 * @file
 * Contains class CurrencyYahooFinanceCurrencyExchanger.
 */

/**
 * Retrieves currency exchange rates from Yahoo! Finance.
 */
class CurrencyYahooFinanceCurrencyExchanger implements CurrencyExchangerInterface {

  /**
   * Implements CurrencyExchangerInterface::load().
   */
  static function load($currency_code_from, $currency_code_to) {
    // We deliberately run this through loadMultiple() rather than request(),
    // because loadMultiple() caches the rates.
    $rates = self::loadMultiple(array(
      $currency_code_from => array($currency_code_to),
    ));

    return $rates[$currency_code_from][$currency_code_to];
  }

  /**
   * Implements CurrencyExchangerInterface::loadMultiple().
   */
  static function loadMultiple(array $currency_codes) {
    $set_cache = FALSE;
    $cached_rates = self::getCache();
    $rates = array();
    foreach ($currency_codes as $currency_code_from => $currency_codes_to) {
      foreach ($currency_codes_to as $currency_code_to) {
        if (empty($cached_rates[$currency_code_from]) || empty($cached_rates[$currency_code_from][$currency_code_to])) {
          $cached_rates[$currency_code_from][$currency_code_to] = self::request($currency_code_from, $currency_code_to);
          $set_cache = TRUE;
        }
        $rates[$currency_code_from][$currency_code_to] = $cached_rates[$currency_code_from][$currency_code_to];
      }
    }
    if ($set_cache) {
      self::setCache($cached_rates);
    }

    return $rates;
  }

  /**
   * Implements CurrencyExchangerInterface::operationsLinks().
   */
  static function operationsLinks() {
    return array(array(
      'title' => t('configure'),
      'href' => 'admin/config/regional/currency-exchange/currency-yahoo-finance',
    ));
  }

  /**
   * Returns the codes of all currencies that Yahoo! Finance offers rates for.
   *
   * @return array
   *   ISO 4217 currency codes.
   */
  static function currencyCodes() {
    static $currency_codes = array('AUD', 'ALL', 'DZD', 'XAL', 'ARS', 'AWG', 'GBP', 'BHD', 'BBD', 'BZD', 'BTN', 'BWP', 'BND', 'BIF', 'BSD', 'BDT', 'BYR', 'BMB', 'BOB', 'BRL', 'BGN', 'CAD', 'KHR', 'KYD', 'XAF', 'COP', 'XCP', 'HRK', 'CZK', 'CNY', 'CVE', 'XOF', 'CLP', 'MKF', 'CRC', 'CUP', 'EUR', 'DJF', 'XCD', 'EGP', 'ERN', 'ETB', 'FJD', 'DKK', 'DOP', 'ECS', 'SVC', 'EEK', 'FKP', 'HKD', 'INR', 'GHC', 'XAU', 'GNF', 'HTG', 'HUF', 'IRR', 'ILS', 'IDR', 'GMD', 'GIP', 'GTQ', 'GYD', 'HNL', 'ISK', 'IQD', 'JPY', 'JOD', 'KES', 'LAK', 'LBP', 'LRD', 'LTL', 'JMD', 'KZT', 'KWD', 'LVL', 'LSL', 'LYD', 'MOP', 'MWK', 'MVR', 'MRO', 'MXN', 'MNT', 'MMK', 'MKD', 'MYR', 'MTL', 'MUR', 'MDL', 'MAD', 'NAD', 'ANG', 'NIO', 'KPW', 'OMR', 'NPR', 'NZD', 'NGN', 'NOK', 'XPF', 'XPD', 'PGK', 'PEN', 'XPT', 'QAR', 'RUB', 'PKR', 'PAB', 'PYG', 'PHP', 'PLN', 'RON', 'RWF', 'CHF', 'WST', 'SAR', 'SLL', 'SGD', 'SIT', 'SOS', 'LKR', 'SDG', 'SEK', 'KRW', 'STD', 'SCR', 'XAG', 'SKK', 'SBD', 'ZAR', 'SHP', 'SZL', 'SYP', 'USD', 'TRY', 'TZS', 'TTD', 'AED', 'UAH', 'THB', 'TWD', 'TOP', 'TND', 'UGX', 'UYU', 'VUV', 'VND', 'ZMK', 'VEF', 'YER', 'ZWD');

    return $currency_codes;
  }

  /**
   * Requests a rate from the Yahoo! Finance API.
   *
   * @param string $currency_code_from
   * @param string $currency_code_to
   *
   * @return float|false
   *   A float if the rate could be found, FALSE if it couldn't.
   */
  protected static function request($currency_code_from, $currency_code_to) {
    if (in_array($currency_code_from, self::currencyCodes()) && in_array($currency_code_to, self::currencyCodes())) {
      $response = drupal_http_request('http://download.finance.yahoo.com/d/quotes.csv?e=.csv&f=sl1&s=' . $currency_code_from . $currency_code_to . '=X');
      if ($response->code == 200) {
        return self::parseResponse($response->data);
      }
    }
    return FALSE;
  }

  /**
   * Parses a response from the Yahoo! Finance server.
   *
   * @param string $response
   *
   * @return float|false
   *   A float if the rate could be found, FALSE if it couldn't.
   */
  static function parseResponse($response) {
    // The response is a CSV string, of which the first column is the
    // original query, and the second the exchange rate.
    $response_fragments = explode(',', $response);
    if (isset($response_fragments[1])) {
      // The message may contain newlines.
      $amount = trim($response_fragments[1]);
      // A rate of 0 means the rate could not be found.
      if (is_numeric($amount) && $amount > 0) {
        return (float) $amount;
      }
    }
    return FALSE;
  }

  /**
   * Gets rates from the cache.
   *
   * @return array
   *   An array with a structure similar to the return value of loadMultiple().
   */
  static function getCache() {
    $cache_lifetime = variable_get('currency_yahoo_finance_cache_lifetime', CURRENCY_YAHOO_FINANCE_CACHE_LIFETIME);
    $cache = cache_get('currency_yahoo_finance');
    if (is_object($cache) && $cache->created + $cache_lifetime > REQUEST_TIME) {
      return $cache->data;
    }
    return array();
  }

  /**
   * Saves rates to the cache.
   *
   * @param array $rates
   *   An array with a structure similar to the return value of loadMultiple().
   */
  static function setCache(array $rates) {
    return cache_set('currency_yahoo_finance', $rates, 'cache', time() + self::cacheLifetime());
  }

  /**
   * Returns the cache lifetime.
   *
   * @return int
   */
  static function cacheLifetime() {
    return (int) variable_get('currency_yahoo_finance_cache_lifetime', CURRENCY_YAHOO_FINANCE_CACHE_LIFETIME);
  }
}
