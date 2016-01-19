<?php
/**
 * @file xml.inc
 *
 * XML handling and parsing of requests/responses.
 */

/**
 *  Send a POST request to the scheme operator.
 * @param $url
 *   URI to scheme operator
 * @param $xml
 *   XML request
 * @return mixed
 *   XML response
 */
function commerce_eps_send_xml_request($url, $xml) {
  // prepend XML DTD
  $xml = '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>' . "\r\n" . $xml;
  // initiate CURL request
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL, $url);
  curl_setopt($ch, CURLOPT_POST, 1);            // it's a POST request
  curl_setopt($ch, CURLOPT_POSTFIELDS, utf8_encode($xml));
  curl_setopt($ch, CURLOPT_TIMEOUT, 60);     //times out after 61s
  curl_setopt($ch, CURLOPT_HEADER, 0);
  curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);  // make sure a return value exists
  curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
  curl_setopt($ch, CURLOPT_HTTPHEADER, Array("Content-Type: text/xml; charset=utf-8"));

  // contact SO server
  $result = curl_exec($ch);
  curl_close($ch);

  return $result;
}

/**
 * Read XML stream from HTTP POST request
 * @return string
 *   XML post data from HTTP request
 */
function commerce_eps_get_xml_from_http_post() {
  $xmltext = $GLOBALS["HTTP_RAW_POST_DATA"]; 	// get raw post data
  if (!$xmltext) { // if raw is not existing, read a normal post
    while(list($key, $val) = each($HTTP_POST_VARS)) {
      $key = stripslashes($key);
      $val = stripslashes($val);
      $key = rawurldecode($key);

      // restore space of HTTP_POST_VARS
      if (($i = strpos($key, 'xml_',0)) > 0) {
        $key = str_replace('xml_', 'xml ', $key);
      }
      $val = rawurldecode($val);
      $xmltext .= "$key=$val";
    }
  }
  return utf8_decode($xmltext);
}

/**
 * Print an XML response using Drupal template/theme function.
 *
 * @param $template
 *   template for theme function
 * @param $vars
 *   theming variables
 */
function commerce_eps_print_xml_response($template, $vars) {
  header("Content-Type: text/xml; charset=utf-8");
  echo '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>' . "\r\n";
  print theme($template, $vars);
}

/**
 * @class EPSXMLHandler
 *
 * Provides a wrapper for an XML document.
 *
 * Internally XML is parsed into a DOM and XPath objects.
 */
class EPSXMLHandler {

  public $document = NULL;

  public function __construct($xml) {
    // create XML DOM document
    $this->document = new DOMDocument('1.0', 'utf-8');
    // We don't want to bother with white spaces
    $this->document->preserveWhiteSpace = false;
    $this->document->formatOutput = true;
    // load with response XML
    $this->document->loadXML($xml);
  }

  /**
   * Validate XML with a XML schema
   * @param string $schema
   * @return bool
   */
  public function validate($schema = 'schemas/EPSProtocol-V25.xsd') {
    if (!file_exists($schema)) { // try loading from relative location
      $schema = drupal_get_path('module', 'commerce_eps') . '/' . $schema;
    }
    if (file_exists($schema) && $this->document->schemaValidate($schema)) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Retrieve element information from XML document, either element or if provided attribute value
   * @param $element
   *   element name
   * @param int $index
   *   index value
   * @param $attribute
   *   attribute name
   * @return string
   *   element value
   */
  public function getElementValue($element, $index = 0, $attribute = NULL) {
    $xpath = new DOMXPath($this->document);
    $item = $xpath->query('//' . $element)->item($index);
    if (!isset($attribute)) {
      return trim($item->nodeValue);
    }
    else {
      return trim($item->getAttribute($attribute));
    }
  }
}