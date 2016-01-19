<?php

class CommerceUKMailZone {

  // The default zone to use if one isn't found.
  public $defaultZone = 'A';

  public function __construct($country, $postcode) {
    $this->country = $country;
    $this->postcode = $this->sanitizePostcode($postcode);

    // First, attempt to find the zone from the country.
    $this->findZoneByCountry();

    if (!$this->zone) {
      // Attempt to find the zone from postcode. If a specific zone is not found,
      // the value of $this->defaultZone shall be returned.
      for ($number_of_characters = 4; $number_of_characters > 0; $number_of_characters--) {
        $this->findZoneByPostcode($number_of_characters);

        // If a matching zone has been found, break from the for loop.
        if ($this->zone) {
          break;
        }
      }
    }
  }

  public function __toString() {
    // Return the zone.
    return $this->zone ? $this->zone : $this->defaultZone;
  }

  /**
   * Ensure that the postcodes are consistantly formatted.
   *
   * @param string $postcode
   *   The original postcode, as entered by the user during checkout.
   *
   * @return string
   *   The sanitized postcode.
   */
  public function sanitizePostcode($postcode) {
    return drupal_strtoupper($postcode);
  }

  public function findZoneByCountry() {
    if ($this->country != 'GB') {
      switch ($this->country) {
        case 'IE': // Ireland
          $this->zone = 'E';
          break;

        case 'GG': // Guernsey
        case 'IM': // Isle of Man
        case 'JE': // Jersey
          $this->zone = 'D';
          break;
      }
    }
  }

  /**
   * Find the zone from the postcode.
   *
   * @param integer $number_of_characters
   *   The number of characters in the postcode to attempt to match against.
   */
  public function findZoneByPostcode($number_of_characters) {
    // Limit the postcode to the desired number of characters.
    $postcode = drupal_substr($this->postcode, 0, $number_of_characters);

    // Run the appropriate method for the number of characters.
    $method_name = 'findZoneByPostcode' . $number_of_characters;
    $this->$method_name($postcode);
  }

  /**
   * Attempt to find the zone based on the first character of the postcode.
   *
   * @param string $postcode
   *   The shortened postcode.
   */
  public function findZoneByPostcode1($postcode) {
    switch ($postcode) {
      case 'G':
        $this->zone = 'B';
        break;
    }
  }

  /**
   * Attempt to find the zone based on the first two characters of the
   * postcode.
   *
   * @param string $postcode
   *   The shortened postcode.
   */
  public function findZoneByPostcode2($postcode) {
    switch ($postcode) {
      case 'DD':
      case 'DG':
      case 'EH':
      case 'FK':
      case 'KY':
      case 'ML':
      case 'TD':
        $this->zone = 'B';
        break;

      case 'KA':
        // Don't include KA27 and KA28 as these are within Zone D.
        $four_characters = drupal_substr($this->postcode, 0, 4);
        if (!in_array($four_characters, array('KA27', 'KA28'))) {
          $this->zone = 'B';
        }
        break;

      case 'HS':
      case 'ZE':
        $this->zone = 'D';
        break;

      case 'BT': // Northern Ireland (http://en.wikipedia.org/wiki/BT_postcode_area)
        $this->zone = 'E';
        break;
    }
  }

  /**
   * Attempt to find the zone based on the first three characters of the
   * postcode.
   *
   * @param string $postcode
   *   The shortened postcode.
   */
  public function findZoneByPostcode3($postcode) {
    switch ($postcode) {
      case 'PA1':
      case 'PA2':
      case 'PA3':
      case 'PA4':
      case 'PA5':
      case 'PA6':
      case 'PA7':
      case 'PA8':
      case 'PA9':
      case 'PH1':
      case 'PH2':
      case 'PH3':
        $this->zone = 'B';
        break;

      case 'IV1':
      case 'IV2':
      case 'IV3':
      case 'IV4':
      case 'IV5':
      case 'IV6':
      case 'IV7':
      case 'IV8':
      case 'IV9':
      case 'KW1':
      case 'KW2':
      case 'KW3':
      case 'KW4':
      case 'KW5':
      case 'KW6':
      case 'KW7':
      case 'KW8':
      case 'KW9':
      case 'PH4':
      case 'PH5':
      case 'PH6':
      case 'PH7':
      case 'PH8':
      case 'PH9':
        $this->zone = 'C';
    }
  }

  /**
   * Attempt to find the zone based on the first four characters of the
   * postcode.
   *
   * @param string $postcode
   *   The shortened postcode.
   */
  public function findZoneByPostcode4($postcode) {
    switch ($postcode) {
      case 'AB10':
      case 'AB11':
      case 'AB12':
      case 'AB13':
      case 'AB14':
      case 'AB15':
      case 'AB16':
      case 'AB21':
      case 'AB22':
      case 'AB23':
      case 'AB24':
      case 'AB25':
      case 'AB30':
      case 'AB39':
      case 'PA10':
      case 'PA11':
      case 'PA12':
      case 'PA13':
      case 'PA14':
      case 'PA15':
      case 'PA16':
      case 'PA17':
      case 'PA18':
      case 'PA19':
        $this->zone = 'B';
        break;

      case 'AB31':
      case 'AB32':
      case 'AB33':
      case 'AB34':
      case 'AB35':
      case 'AB36':
      case 'AB37':
      case 'AB38':
      case 'AB40':
      case 'AB41':
      case 'AB42':
      case 'AB43':
      case 'AB44':
      case 'AB45':
      case 'AB46':
      case 'AB47':
      case 'AB48':
      case 'AB49':
      case 'AB50':
      case 'AB51':
      case 'AB52':
      case 'AB53':
      case 'AB54':
      case 'AB55':
      case 'AB56':
      case 'IV10':
      case 'IV11':
      case 'IV12':
      case 'IV13':
      case 'IV14':
      case 'IV15':
      case 'IV16':
      case 'IV17':
      case 'IV18':
      case 'IV19':
      case 'IV20':
      case 'IV21':
      case 'IV22':
      case 'IV23':
      case 'IV24':
      case 'IV25':
      case 'IV26':
      case 'IV27':
      case 'IV28':
      case 'IV30':
      case 'IV31':
      case 'IV32':
      case 'IV36':
      case 'IV37':
      case 'IV38':
      case 'IV39':
      case 'IV40':
      case 'IV52':
      case 'IV53':
      case 'IV54':
      case 'IV63':
      case 'KW10':
      case 'KW11':
      case 'KW12':
      case 'KW13':
      case 'KW14':
      case 'PA21':
      case 'PA22':
      case 'PA23':
      case 'PA24':
      case 'PA25':
      case 'PA26':
      case 'PA27':
      case 'PA28':
      case 'PA29':
      case 'PA30':
      case 'PA31':
      case 'PA32':
      case 'PA33':
      case 'PA34':
      case 'PA35':
      case 'PA36':
      case 'PA37':
      case 'PA38':
      case 'PH10':
      case 'PH11':
      case 'PH12':
      case 'PH13':
      case 'PH14':
      case 'PH15':
      case 'PH16':
      case 'PH17':
      case 'PH18':
      case 'PH19':
      case 'PH20':
      case 'PH21':
      case 'PH22':
      case 'PH23':
      case 'PH24':
      case 'PH25':
      case 'PH26':
      case 'PH27':
      case 'PH28':
      case 'PH29':
      case 'PH30':
      case 'PH31':
      case 'PH32':
      case 'PH33':
      case 'PH34':
      case 'PH35':
      case 'PH36':
      case 'PH37':
      case 'PH38':
      case 'PH39':
      case 'PH40':
      case 'PH41':
      case 'PH49':
      case 'PH50':
        $this->zone = 'C';
        break;

      case 'IV41':
      case 'IV42':
      case 'IV43':
      case 'IV44':
      case 'IV45':
      case 'IV46':
      case 'IV47':
      case 'IV48':
      case 'IV49':
      case 'IV51':
      case 'IV55':
      case 'IV56':
      case 'KA27':
      case 'KA28':
      case 'KW15':
      case 'KW16':
      case 'KW17':
      case 'PA20':
      case 'PA41':
      case 'PA42':
      case 'PA43':
      case 'PA44':
      case 'PA45':
      case 'PA46':
      case 'PA47':
      case 'PA48':
      case 'PA49':
      case 'PH42':
      case 'PH43':
      case 'PH44':
        $this->zone = 'D';
        break;
    }
  }
}
