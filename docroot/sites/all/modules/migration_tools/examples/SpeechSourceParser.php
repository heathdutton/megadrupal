<?php
/**
 * @file
 * Includes SpeechSourceParser class.
 *
 * This class contains customization to parse speeches.
 *
 * This is an example of extra processing to handle locations.
 */

/**
 * Class SpeechSourceParser.
 *
 * @package migration_tools
 */

abstract class SpeechSourceParser extends MTNodeSourceParser {
  protected $city;
  protected $country;
  protected $date;
  // $location contains city, state, country.
  protected $location;
  protected $state;

  /**
   * Getter $this->city property.
   */
  public function getCity() {
    $city = $this->getProperty('city');

    return $city;
  }

  /**
   * Getter $this->country property.
   */
  public function getCountry() {
    $country = $this->getProperty('country');
    $country = strtoupper($country);
    // @TODO Add additional country filtering as the use case arises.
    $countries = array(
      'USA' => 'US',
      'US' => 'US',
      'UNITED STATES' => 'US',
      'UNITED STATES OF AMERICA' => 'US',
    );

    if (!empty($countries[$country])) {
      return $countries[$country];
    }
  }

  /**
   * Getter $this->date property.
   */
  public function getDate() {
    $date_string = $this->getProperty('date');
    MigrationMessage::makeMessage("Raw Date: @date_string", array('@date_string' => $date_string), WATCHDOG_DEBUG, 2);

    if (empty($date_string)) {
      $date = '';
    }
    else {
      $date = date('n/d/Y', strtotime($date_string));
      if (!empty($date)) {
        // Output success to show progress to aid debugging.
        MigrationMessage::makeMessage("Formatted Date: @date", array('@date' => $date), WATCHDOG_DEBUG, 2);
      }
    }

    return $date;
  }

  /**
   * Getter for location. Uses geocoder to attempt the full address.
   *
   * If this is unable to get all the parts of the address, it will call
   * specific finders as needed.
   */
  public function getLocation() {
    try {
      // Run the location obtainer to try to get the full address.
      $location = $this->getProperty('location');
      $address = array();

      if (!empty($location)) {
        $address = $this->geoCodeString($location);
      }

      // Use what the geoCoder generated if it is available.
      if (!empty($address['locality'])) {
        $this->city = $address['locality'];
      }
      if (!empty($address['administrative_area_level_1'])) {
        $this->state = $address['administrative_area_level_1'];
      }
      if (!empty($address['country'])) {
        $this->country = $address['country'];
      }

    }
    catch (Exception $e) {
      $message = '@fileid failed geocoding: @error';
      $vars = array(
        '@fileid' => $this->fileid,
        '@error' => $e->getMessage(),
      );
      MigrationMessage::makeMessage($message, $vars, WATCHDOG_WARNING, 2);
    }
  }

  /**
   * Getter $this->state property.
   */
  public function getState() {
    $state = $this->getProperty('state');
    $states = ObtainState::getStates();
    // Must return the abbreviation of a state.
    if (!empty($states[$state])) {
      return $states[$state];
    }
    elseif (in_array($state, $states)) {
      return $state;
    }
    else {
      return;
    }

  }

  /**
   * Clean the html right before pulling the body.
   */
  protected function cleanHtml() {
    parent::cleanHtml();
    // If the first paragraph in the content div says archive, lets remove it.
    $elem = HtmlCleanUp::matchText($this->queryPath, ".contentSub > div > p", "Archives");
    if ($elem) {
      $elem->remove();
    }

    // Build selectors to remove.
    $selectors = array(
      "#PRhead1",
      "#navWrap",
      "#Layer3",
      "#Layer4",
      "img",
      "h1",
      ".breadcrumb",
      ".newsLeft",
      "#widget",
      "#footer",
      "a[title='Printer Friendly']",
      "a[href='#top']",
    );
    HtmlCleanUp::removeElements($this->queryPath, $selectors);
  }

  /**
   * {@inheritdoc}
   */
  protected function setDefaultObtainersInfo() {
    parent::setDefaultObtainersInfo();

    $title = new ObtainerInfo("title");
    $title->addMethod('pluckAnySelectorUntilValid', array('h1', 10, 'html'));
    $this->addObtainerInfo($title);

    $date = new ObtainerInfo("date");
    $date->addMethod('pluckProbableDate');
    $this->addObtainerInfo($date);

    // Not all speech migrations have locations, register these without methods
    // so that they remain optional until methods are added in the migration
    // speciic source parser.
    $location = new ObtainerInfo('location');
    $this->addObtainerInfo($location);
    $city = new ObtainerInfo('city');
    $this->addObtainerInfo($city);
    $country = new ObtainerInfo('country');
    $this->addObtainerInfo($country);
    $state = new ObtainerInfo('state');
    $this->addObtainerInfo($state);
  }
}
