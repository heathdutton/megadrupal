<?php

/**
 * @file
 * DHL Capability and Quote service.
 */

class DHLQuote {

  // Factory instance.
  protected static $me;

  // The default country.
  protected $domestic = 'FR';

  // Data (zone, delivery time, etc.) for countries.
  protected $countries = array();

  // Not all package sizes are available.
  protected $allowedPackageSizes = array();

  // Rates.
  protected $rates = array();
  protected $ratesDomestic = array();

  protected $countryCode;

  const ERROR_WEIGHT_OVER = -1;
  const ERROR_WEIGHT_UNKNOWN = -2;
  const ERROR_COUNTRY_UNKNOWN = -3;

  /**
   * Constructor.
   */
  public function __construct() {
    $path = dirname(__FILE__) . '/../data';
    $handle = fopen($path . '/code-zone.csv', 'r');
    while ($cols = fgetcsv($handle)) {
      $this->countries[$cols[1]] = array(
        'name' => $cols[0],
        'zone' => $cols[2],
      );
    }

    $handle = fopen($path . '/tarif.csv', 'r');
    $line = 0;
    while ($cols = fgetcsv($handle)) {
      if (!$line++) {
        // Ignore the first line.
        continue;
      }
      // Convert decimal point from "," to ".".
      $cols = str_replace(',', '.', $cols);

      // $cols[0] is now useless, but keep it so that we have the index
      // $cols[$zone].
      $this->rates[$cols[0]] = $cols;
    }

    $handle = fopen($path . '/tarif-domestic.csv', 'r');
    $line = 0;
    while ($cols = fgetcsv($handle)) {
      if (!$line++) {
        // Ignore the first line.
        continue;
      }
      $this->ratesDomestic[] = $cols;
    }
    // Add a gate-keeper to keep the algo simple.
    $this->ratesDomestic[] = array(1000, 10, 10, 10);
  }

  /**
   * Returns an object instance.
   */
  public static function Factory() {
    if (!isset(self::$me)) {
      self::$me = new DhlQuote();
    }

    return self::$me;
  }

  /**
   * Set allowed package sizes.
   */
  public function setAllowedPackageSizes($sizes) {
    $this->allowedPackageSizes = $sizes;
    sort($this->allowedPackageSizes, SORT_NUMERIC);
    return $this;
  }

  /**
   * Calculates a quote.
   */
  public function calculate($weight, $country_code = NULL) {
    if (isset($country_code)) {
      $this->countryCode = $country_code;
    }

    if ($this->allowedPackageSizes) {
      for ($i = 0; $i < count($this->allowedPackageSizes); $i++) {
        if ($this->allowedPackageSizes[$i] >= $weight) {
          $weight = $this->allowedPackageSizes[$i];
          break;
        }
      }
    }

    if ($weight > max(array_keys($this->rates))) {
      return self::ERROR_WEIGHT_OVER;
    }

    // Handle domestic shipping rate.
    if ($this->countryCode == $this->domestic) {
      // 3 types of services: 18h, 12h and 9h.
      $service = 1;
      $weight = ceil($weight);
      $rate = $this->ratesDomestic[0][$service];
      for ($i = 1; $i < count($this->ratesDomestic) - 1; $i++) {
        if ($this->ratesDomestic[$i + 1][0] >= $weight) {
          $rate += ($weight - $this->ratesDomestic[$i][0]) * $this->ratesDomestic[$i][$service];
          break;
        }
        else {
          $rate += ($this->ratesDomestic[$i + 1][0] - $this->ratesDomestic[$i][0]) * $this->ratesDomestic[$i][$service];
        }
      }
      return $this->addExtra($rate);
    }

    // Round up to the next half and convert to string to avoid numeric index
    // issue.
    $weight = (string) (ceil($weight * 2) / 2);
    if (!isset($this->rates[$weight])) {
      return self::ERROR_WEIGHT_UNKNOWN;
    }

    if (!isset($this->countries[$this->countryCode])) {
      return self::ERROR_COUNTRY_UNKNOWN;
    }
    $zone = $this->countries[$this->countryCode]['zone'];
    return $this->addExtra($this->rates[$weight][$zone], in_array($zone, array(1, 2, 3)));
  }

  /**
   * Add extra rate.
   *
   * Add carburant and TVA (if EU).
   *
   * @param float $rate
   *   Rate.
   *
   * @param boolean $EU
   *   Whether shipping to Europe EU zone.
   */
  protected function addExtra($rate, $EU = TRUE) {
    return $rate * 1.2 * ($EU ? 1.2 : 1);
  }
}

