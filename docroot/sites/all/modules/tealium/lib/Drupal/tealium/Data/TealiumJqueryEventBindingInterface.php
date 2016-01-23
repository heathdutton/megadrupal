<?php
/**
 * @file
 * Class Interface for binding Tealium utag_data to a jQuery selector event.
 */

namespace Drupal\tealium\Data;

use Drupal\tealium\TealiumUtagData;

interface TealiumJqueryEventBindingInterface {

  public function setJQuerySelector($selector);

  public function getJQuerySelector();

  public function setDomEvent($event_name);

  public function getDomEvent();

  public function setTrackType($type);

  public function getTrackType();

  public function setTealiumData(TealiumUtagData $dataTealiumSourcevalues);

  public function getTealiumData();

}