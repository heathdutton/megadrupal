<?php

/**
 * @file
 * Highrise api.
 */

class Highrise extends HighriseAPI {

  protected function checkForErrors($type, $expected_status_codes = 200) {
    $error = FALSE;
    if (!is_array($expected_status_codes))
    $expected_status_codes = array($expected_status_codes);

    if (!in_array($this->getLastReturnStatus(), $expected_status_codes)) {
      switch ($this->getLastReturnStatus()) {
        case 404:
          return $error = TRUE;
          break;
        case 403:
          throw new Exception("Access denied to $type resource");
          break;
        case 507:
          throw new Exception("Cannot create $type: Insufficient storage in your Highrise Account");
          break;

        default:
          throw new Exception("API for $type returned Status Code: " . $this->getLastReturnStatus() . " Expected Code: " . implode(",", $expected_status_codes));
          break;
      }
    }
  }

  /* Users */


  public function findMe() {
    $xml = $this->getUrl("/me.xml");
    $status = $this->checkForErrors("User");
    if ($status == FALSE) {
      $xml_obj = simplexml_load_string($xml);
      $user = new HighriseUser();
      $user->loadFromXMLObject($xml_obj);
      return $user;
    }
  }
}
