<?php
/**
 * @file
 * Response for adlib image requests. Based on base response.
 */

class AdlibImageResponse extends AdlibBaseResponse {
  /**
   * Contructor.
   */
  public function __construct($response_with_header, $http_info = array()) {
    parent::__construct($response_with_header, $http_info);
    try {
      // Disable warnings from XML parser.
      libxml_use_internal_errors(TRUE);
      $xml_object = simplexml_load_string($this->raw);
      /* check for error in XML */
      try {
        if (isset($xml_object->diagnostic)) {
          $error_xml = $xml_object->diagnostic->error;
          if (!empty($error_xml->info)) {
            $this->_error = TRUE;
            $this->_errorString = $error_xml->info . " : " . $error_xml->message;
          }
        }
      }
      catch (Exception $e) {
        // If we have an exception,
        // the request returned binary data and we have NO error
        // so in this case an emtpy catch is actually correct.
      }
    }
    catch (Exception $e) {
      // If we have an exception,
      // the request returned binary data and we have NO error
      // so in this case an emtpy catch is actually correct.
    }
    // Re-enable warnings from XML parser.
    libxml_use_internal_errors(FALSE);
  }

}
