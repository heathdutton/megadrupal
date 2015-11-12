<?php
/**
 * @file
 * Class Interface for the Tealium Universal Data Object (utag_data).
 */

namespace Drupal\tealium;


interface TealiumUtagDataInterface extends UniversalDataObjectInterface {

  function getJavascriptVariables();

}