<?php
/**
 * @file
 * Class to define and output the Tealium Universal Data Object (utag_data) from set name-value pairs.
 */

namespace Drupal\tealium;

class TealiumUtagData extends UniversalDataObject implements TealiumUtagDataInterface {

  private $cleanVariableNameCache = array();

  public function __toString() {
    // Encode <, >, ', &, and " using the json_encode() options parameter.
    return json_encode(
      $this->getAllDataSourceValues(),
      JSON_HEX_TAG | JSON_HEX_AMP | JSON_HEX_APOS | JSON_HEX_QUOT
    );
  }

  /**
   * @return null|string
   */
  public function getJavascriptVariables() {
    if (count($this->getAllDataSourceValues()) === 0) {
      return NULL;
    }
    else {
      return strval($this);
    }
  }

}