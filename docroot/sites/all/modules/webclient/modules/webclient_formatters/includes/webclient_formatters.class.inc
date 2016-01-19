<?php
/**
 * @file
 * Defines the webclient response formatters for various MIME types.
 */

/**
 * DOM response formatter.
 */
class WebResponseDOMFormatter extends WebResponseFormatter {

  /**
   * Get the formatted response for the specified MIME type.
   *
   * @param string $mime_type
   *   The MIME type that applies to the response data.
   * @param string $data
   *   The response data.
   *
   * @return DOMDocument
   *   A DOMDocument object containing the parsed response data.
   */
  public function format($mime_type, $data) {
    // Construct supported MIME types array.
    static $supported_mime_types = array('text/html' => TRUE, 'text/xml' => TRUE);
    // Check if the MIME type is supported by the formatter.
    if (isset($supported_mime_types[$mime_type])) {
      // Create DOM document.
      $document = new DOMDocument();
      // Determine what load function should be called.
      if ($mime_type == 'text/html') {
        // Parse the response data as HTML.
        @$document->loadHTML($data);
      }
      else {
        // Parse response data as XML.
        @$document->loadXML($data);
      }
      return $document;
    }
    // Default fallback: MIME type cannot be parsed.
    return NULL;
  }

}

/**
 * JSON response formatter.
 */
class WebResponseJSONFormatter extends WebResponseFormatter {

  /**
   * Get the formatted response for the specified MIME type.
   *
   * @param string $mime_type
   *   The MIME type that applies to the response data.
   * @param string $data
   *   The response data.
   *
   * @return mixed
   *   Mixed value depending on the received response data.
   */
  public function format($mime_type, $data) {
    // Return the decoded JSON data.
    return drupal_json_decode($data);
  }

}
