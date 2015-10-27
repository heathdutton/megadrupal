<?php
/**
 * @file
 * Class definition of adlib search response.
 */

class AdlibSearchResponse extends AdlibBaseResponse {

  /**
   * The simpleXMLObject that is filled by the request
   */
  protected $xmlObject;

  /**
   * Number of records returned
   */
  protected $numberRecordsReturned;

  /**
   * Total number of records found
   */
  protected $numberRecordsfound;

  /**
   * The raw response
   */
  protected $raw;

  /**
   * Contructor.
   *
   * @param string $response_with_header
   *   The raw response we get from the adlib server.
   * @param array $http_info
   *   The HTTP headers created when the request was made.
   */
  public function __construct($response_with_header, $http_info = array()) {
    // Call base constructor.
    parent::__construct($response_with_header, $http_info);
    // Only if no errors occured, continue.
    if (!$this->getError()) {
      try {
        // Disable warnings from XML parser.
        libxml_use_internal_errors(TRUE);
        $this->xmlObject = simplexml_load_string($this->raw);
        // Check for error in XML.
        $error_xml = $this->xmlObject->diagnostic->error;
        if (!empty($error_xml->info)) {
          $this->error = TRUE;
          $this->errorString = $error_xml->info . ' : ' . $error_xml->message;
        }
        // Save number of records returnd.
        $this->numberRecordsReturned = (int) $this->xmlObject->diagnostic->hits_on_display;
        $this->numberRecordsfound = (int) $this->xmlObject->diagnostic->hits;
      }
      catch (Exception $e) {
        $this->error = TRUE;
        $this->errorString = t('Query did not return valid XML');
      }
      // Re-enable warnings from XML parser.
      libxml_use_internal_errors(FALSE);
    }
  }

  /**
   * Get the simpleXML Object.
   *
   * @return simpleXml
   *   The simple xml object.
   */
  public function getXMLObject() {
    return $this->xmlObject;
  }

  /**
   * Get fields as an array.
   *
   * @return string
   *   All the fields in a assiociative array.
   */
  public function getXMLArray() {
    // TODO: this method is only usefull when getFieldlist is asked.
    // Maybe it should be more general.
    // Traverse XML to find all the fields and put them in associative array.
    $fields = array();
    if (!$this->error && isset($this->xmlObject)) {
      $results = $this->xmlObject->xpath('/adlibXML/recordList/record');
      foreach ($results as $node) {
        $result = array();
        $tag = (string) $node->tag;
        if (isset($node->fieldName->value[0])) {
          $result['fieldName'] = (string) $node->fieldName->value[0];
        }
        if (isset($node->displayName->value[0])) {
          $result['displayName'] = (string) $node->displayName->value[0];
        }
        if (isset($node->length)) {
          $result['length'] = (integer) $node->length;
        }
        if (isset($node->type)) {
          $result['type'] = (string) $node->type;
        }
        if (isset($node->repeated)) {
          $result['repeated'] = $node->repeated == 'True' ? TRUE : FALSE;
        }
        if (isset($node->isLinked)) {
          $result['isLinked'] = $node->isLinked == 'True' ? TRUE : FALSE;
        }
        if (isset($node->isIndexed)) {
          $result['isIndexed'] = $node->isIndexed == 'True' ? TRUE : FALSE;
        }

        $fields[$tag] = $result;
      }
    }
    return $fields;
  }

  /**
   * Return the XML as a string.
   *
   * @return string
   *   The simpleXML object as a string
   */
  public function getXMLString() {
    if (isset($this->xmlObject)) {
      return $this->xmlObject->asXML();
    }
    else {
      return "";
    }
  }

  /**
   * Get the number of records.
   *
   * @return int
   *   Number of records returned by search.
   */
  public function getNumberOfRecords() {
    return $this->numberRecordsReturned;
  }

  /**
   * Get the number of matches.
   *
   * @return int
   *   Number of records that match thesearch
   */
  public function getNumberOfMatches() {
    return $this->numberRecordsfound;
  }

}
