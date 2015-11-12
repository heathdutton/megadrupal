<?php

/**
 * @file
 * Drupal Module: XML Charts data objects.
 *
 * XMLCharts is a module that consumes,
 * caches and provides blocks to display
 * industrial and precious metal prices
 * from XMLCharts Free XML Price Feeds.
 *
 * author: Andrew Wasson <https://drupal.org/user/127091>
 */

/**
 * XMLCharts Data Object.
 */
class XMLChartsData {
  public $options;
  public $urlPreciousMetals;
  public $urlIndustrialMetals;
  public $tokenPreciousMetals;
  public $tokenIndustrialMetals;

  /**
   * XMLChartsData() Constructor.
   */
  public function __construct() {
    $this->urlPreciousMetals     = 'http://www.xmlcharts.com/cache/precious-metals.php?format=json';
    $this->urlIndustrialMetals   = 'http://www.xmlcharts.com/cache/industrial-metals.php?format=json';
    $this->tokenPreciousMetals   = 'xml_charts_precious_metals';
    $this->tokenIndustrialMetals = 'xml_charts_industrial_metals';
    $this->options = array(
      'method' => 'GET',
      'timeout' => 30,
      'headers' => array('Content-Type' => 'text/plain'),
    );
  }

  /**
   * Private xmlMetalsObject($url, $token).
   *
   * @param string $url
   *   Contains a URL to xmlcharts.com feed.
   * @param string $token
   *   Represents the name to assign/retrieve cached data.
   *
   * @return json Object
   *   Contains commodities and prices with associated data.
   */
  private function xmlMetalsObject($url, $token) {
    if ($cache = cache_get($token)) {
      $xml_object = $cache->data;
    }
    else {
      $xml_object = drupal_http_request($url, $this->options);
      cache_set($token, $xml_object, 'cache');
      watchdog('xml_charts', "NOTICE: Building @token XMLChartsData", array('@token' => $token));
    }
    return $xml_object;
  }

  /**
   * Private getMetalsData($url, $token).
   *
   * @param string $url
   *   Contains a URL to xmlcharts.com feed.
   * @param string $token
   *   Represents the name to assign/retrieve cached data.
   *
   * @return array
   *   Contains commodities and prices only.
   */
  private function getMetalsData($url, $token) {
    $xml_metal = $this->xmlMetalsObject($url, $token);
    return drupal_json_decode($xml_metal->data);
  }

  /**
   * Private getMetalsTypes($url, $token).
   *
   * @param string $url
   *   Contains a URL to xmlcharts.com feed.
   * @param string $token
   *   Represents the name to assign/retrieve cached data.
   *
   * @return array
   *   Contains first single array of commodities and prices.
   */
  private function getMetalsTypes($url, $token) {
    $xml_metal_data = $this->getMetalsData($url, $token);
    return $xml_metal_data[key($xml_metal_data)];
  }

  /**
   * Private getXmlchartsCurrencies($url, $token).
   *
   * @param string $url
   *   Contains a URL to xmlcharts.com feed.
   * @param string $token
   *   Represents the name to assign/retrieve cached data.
   *
   * @return array
   *   Contains the currencies that are available.
   */
  private function getXmlchartsCurrencies($url, $token) {
    $arr_metals_currencies = array();
    foreach ($this->getMetalsData($url, $token) as $key => $value) {
      $arr_metals_currencies[] = $key;
    }
    return $arr_metals_currencies;
  }

  /**
   * Public getPreciousMetalsTypes().
   *
   * @return array
   *   Contains single array of precious metal types.
   */
  public function getPreciousMetalsTypes() {
    return $this->getMetalsTypes($this->urlPreciousMetals, $this->tokenPreciousMetals);
  }

  /**
   * Public getIndustrialMetalsTypes().
   *
   * @return array
   *   Contains single array of industrial metal types.
   */
  public function getIndustrialMetalsTypes() {
    return $this->getMetalsTypes($this->urlIndustrialMetals, $this->tokenIndustrialMetals);
  }

  /**
   * Public getPreciousMetalsCurrencies().
   *
   * @return array
   *   Contains the currencies that are available for precious metals.
   */
  public function getPreciousMetalsCurrencies() {
    return $this->getXmlchartsCurrencies($this->urlPreciousMetals, $this->tokenPreciousMetals);
  }

  /**
   * Public getIndustrialMetalsCurrencies().
   *
   * @return array
   *   Contains the currencies that are available for industrial metals.
   */
  public function getIndustrialMetalsCurrencies() {
    return $this->getXmlchartsCurrencies($this->urlIndustrialMetals, $this->tokenIndustrialMetals);
  }

  /**
   * Public getPreciousMetalsData().
   *
   * @return array
   *   Contains commodities and prices for precious metals.
   */
  public function getPreciousMetalsData() {
    return $this->getMetalsData($this->urlPreciousMetals, $this->tokenPreciousMetals);
  }

  /**
   * Public getIndustrialMetalsData().
   *
   * @return array
   *   Contains commodities and prices for industrial metals.
   */
  public function getIndustrialMetalsData() {
    return $this->getMetalsData($this->urlIndustrialMetals, $this->tokenIndustrialMetals);
  }

  /**
   * Public refresh_xmlcharts_cache($url, $token).
   *
   * @param string $url
   *   Contains a URL to xmlcharts.com feed.
   * @param string $token
   *   Represents the name to assign/retrieve cached data.
   *
   * @return bool TRUE
   *   Refreshes cache for a given $url and $token.
   */
  public function refreshXmlchartsCache($url, $token) {
    $xml_object = drupal_http_request($url, $this->options);
    cache_set($token, $xml_object, 'cache');
    watchdog('xml_charts', "NOTICE: Refreshing @token XMLChartsData", array('@token' => $token));
    return TRUE;
  }

}


/**
 * XMLChartsCurrency Data Object.
 */
class XMLChartsCurrency {
  public $setLocale;
  public $currencySymbol;
  public $currencyAlign;

  /**
   * XMLChartsCurrency() Constructor.
   */
  public function __construct() {
    $this->setLocale      = 'en_US';
    $this->currencySymbol = '$';
    $this->currencyAlign  = 'left';
  }

  /**
   * Public function SetCurrency($currency).
   *
   * @param string $currency
   *   Represents one of the applicable currencies.
   *
   *   This method returns nothing and is used to
   *   set the object's variables for local, symbol,
   *   and the alignment of the symbol: left/right.
   */
  public function setCurrency($currency) {
    switch ($currency) {
      case 'aud':
        $this->setLocale      = 'en_AU';
        $this->currencySymbol = '$';
        $this->currencyAlign  = 'left';
        break;

      case 'brl':
        $this->setLocale      = 'pt_BR';
        $this->currencySymbol = 'R$';
        $this->currencyAlign  = 'left';
        break;

      case 'cad':
        $this->setLocale      = 'en_CA';
        $this->currencySymbol = '$';
        $this->currencyAlign  = 'left';
        break;

      case 'chf':
        $this->setLocale      = 'de_CH';
        $this->currencySymbol = 'CHF';
        $this->currencyAlign  = 'left';
        break;

      case 'cny':
        $this->setLocale      = 'zh_ZH';
        $this->currencySymbol = '¥';
        $this->currencyAlign  = 'left';
        break;

      case 'eur':
        $this->setLocale      = 'de_DE';
        $this->currencySymbol = '€';
        $this->currencyAlign  = 'left';
        break;

      case 'gbp':
        $this->setLocale      = 'en_GB';
        $this->currencySymbol = '£';
        $this->currencyAlign  = 'left';
        break;

      case 'inr':
        $this->setLocale      = 'en_IN';
        $this->currencySymbol = '₹';
        $this->currencyAlign  = 'left';
        break;

      case 'jpy':
        $this->setLocale      = 'ja_JP';
        $this->currencySymbol = '¥';
        $this->currencyAlign  = 'left';
        break;

      case 'mxn':
        $this->setLocale      = 'es_MX';
        $this->currencySymbol = '$';
        $this->currencyAlign  = 'left';
        break;

      case 'rub':
        $this->setLocale      = 'de_DE';
        $this->currencySymbol = '₽';
        $this->currencyAlign  = 'left';
        break;

      case 'usd':
        $this->setLocale      = 'en_US';
        $this->currencySymbol = '$';
        $this->currencyAlign  = 'left';
        break;

      case 'zar':
        $this->setLocale      = 'en_ZA';
        $this->currencySymbol = 'R';
        $this->currencyAlign  = 'left';
        break;

      default:
        $this->setLocale      = 'en_US';
        $this->currencySymbol = '$';
        $this->currencyAlign  = 'left';
        break;
    }
  }

}


/**
 * Custom Function xmlcharts_check_time().
 *
 * @param string $time1
 *   Containing an English date format.
 * @param string $time2
 *   Containing an English date format.
 *
 * @return bool
 *   TRUE if $time1 occurs after $time2, otherwise FALSE.
 */
function xmlcharts_check_time($time1, $time2) {
  $start = strtotime($time1);
  $end = strtotime($time2);
  if ($start - $end > 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}


/**
 * Custom Function xmlcharts_check_helper().
 *
 * @param string $type
 *   Containing 'precious' or 'industrial'.
 * @param string $time
 *   Containing an English date format.
 * @param object $xmlcharts_data
 *   Containing an XMLChartsData() object.
 *
 *   This function returns nothing and is used to
 *   check whether the cached metal pricings exist
 *   and/or have expired. If they have expired it refreshes them.
 */
function xmlcharts_check_helper($type, $time, $xmlcharts_data) {

  switch ($type) {
    case 'precious':
      $url_metals     = $xmlcharts_data->urlPreciousMetals;
      $token_metals   = $xmlcharts_data->tokenPreciousMetals;
      break;

    case 'industrial':
      $url_metals     = $xmlcharts_data->urlIndustrialMetals;
      $token_metals   = $xmlcharts_data->tokenIndustrialMetals;
      break;

  }

  if ($cache = cache_get($token_metals)) {
    $cache_data = $cache->data;
    $timestamp = $cache_data->headers['date'];
    if (xmlcharts_check_time($time, $timestamp)) {
      $xmlcharts_data->refreshXmlchartsCache($url_metals, $token_metals);
    }
  }
  else {
    $xmlcharts_data->refreshXmlchartsCache($url_metals, $token_metals);
  }

}


/**
 * Custom Function xmlcharts_check_refresh().
 *
 * @param string $time
 *   Containing an English date format.
 *
 *   This function returns nothing and is used to hand
 *   off an XMLChartsData() for checking and refreshing.
 */
function xmlcharts_check_refresh($time) {
  $xmlcharts_data = new XMLChartsData();
  xmlcharts_check_helper('precious', $time, $xmlcharts_data);
  xmlcharts_check_helper('industrial', $time, $xmlcharts_data);
}
