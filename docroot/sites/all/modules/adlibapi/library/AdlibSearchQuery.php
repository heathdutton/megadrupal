<?php
/**
 * @file
 * Adlib search query class.
 */

class AdlibSearchQuery {

  /*
   * Todo items here.
   */
  // members
  /**
   *  parameters are the query parameters that are sent to the adlib server
   */
  protected $parameters;

  /**
   *  fields are the fields we want to receive from the adlibserver
   */
  protected $fields;

  /**
   *  allowed XML types are: 'grouped', 'unstructured'
   */
  protected $xmltype;

  /**
   * maximum number of results to return
   */
  protected $limit;

  /**
   * number of the first result to return
   */
  protected $startFrom;

  /**
   * Retrieve all fields
   */
  protected $retrieveAllFields = TRUE;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->parameters = array();
    $this->fields = array();
    // Set default xml type.
    $this->xmltype = "grouped";
  }

  /**
   * Add a parameter.
   *
   * @param string $name
   *   Name of the parameter.
   * @param string $value
   *   Value of the parameter.
   * @param string $compare
   *   Compare string.
   * @param string $booleanop
   *   String indicating boolean operator.
   */
  public function addParameter($name, $value = "", $compare = "", $booleanop = "") {
    $this->parameters[] = array(
      'name' => $name,
      'value' => $value,
      'compare' => $compare,
      'booleanop' => $booleanop);
  }

  /**
   * Add one field to the query.
   *
   * The query expectes a comma seperated string of fields.
   *
   * @param string $fieldname
   *   The fieldname as known to adlib.
   */
  public function addField($fieldname) {
    if (!in_array($fieldname, $this->fields)) {
      $this->fields[] = $fieldname;
    }
  }

  /**
   * Add array of fields to the fields array.
   *
   * @param array $fieldarray
   *   An array containing fields to add.
   */
  public function addFields($fieldarray) {
    foreach ($fieldarray as $fieldname) {
      $this->addField($fieldname);
    }
  }

  /**
   * Set limit: maximum amount of keys to return .
   *
   * @param int $limit
   *   Maximum amount of keys to return.
   */
  public function setLimit($limit) {
    $this->limit = $limit;
  }

  /**
   * Set startFrom: first key number to return in the result.
   *
   * @param int $start
   *   First key number to return.
   */
  public function startFrom($start) {
    $this->startFrom = $start;
  }


  /**
   * Set XML type.
   *
   * @param string $xmltype
   *   Xmltype is either structured or unstructured.
   */
  public function setXMLtype($xmltype) {
    $this->xmltype = $xmltype;
  }

  /**
   * Set all fields to be returned.
   *
   * @param bool $enable
   *   If set to true all fields are returned.
   */
  public function setRetrieveAllFields($enable = TRUE) {
    if (is_bool($enable)) {
      $this->retrieveAllFields = $enable;
    }
  }

  /**
   * Determine if all fields are to be returned.
   * @return bool
   *   Check if all fields are returned.
   */
  public function getRetrieveAllFields() {
    return $this->retrieveAllFields;
  }

  /**
   * Get the complete query.
   *
   * @return string
   *   The complete query string.
   */
  public function getQueryItems() {
    $params = $this->parameters2String();
    $items = $params;
    // Add fields (if all fields should be returned, don't add specific fields.
    if (count($this->fields) > 0 && !$this->retrieveAllFields) {
      $fields = '&fields=' . rawurlencode(implode(',', $this->fields));
      $items .= $fields;
    }
    // Add limit.
    if (isset($this->limit)) {
      $items .= '&limit=' . $this->limit;
    }

    // Add the number of the first record to return.
    if (isset($this->startFrom)) {
      $items .= '&startfrom=' . $this->startFrom;
    }

    // Add XMLtype.
    $items .= '&xmltype=' . $this->xmltype;
    return $items;
  }

  /**
   * Helper function.
   *
   * Converts the array with parameters to a string for the query.
   *
   * @return string
   *   The parameter array as one string.
   */
  protected function parameters2String() {
    $is_first = TRUE;
    $param_str = '';
    foreach ($this->parameters as $param) {
      /* There are 3 type of parameters:
       * 	1) <name> <operator> <value> e.g. modification > '1970-01-01'
       * 	2) <name> <value> 			 e.g. sort 'edit.date'
       * 	3) <name>					 e.g. creator->all (linked search)
       */
      // case 1
      if (!empty($param['value']) && !empty($param['compare'])) {
        if (!$is_first) {
          if (!empty($param['booleanop'])) {
            $param_str .= "%20" . rawurlencode($param['booleanop']) . "%20";
          }
        }
        // Only clauses of case 1 are joined by boolean operators.
        $param_str .= rawurlencode($param['name']) . $param['compare'] . "'" . rawurlencode($param['value']) . "'" . "%20";

      }
      // Case 2.
      elseif (!empty($param['value']) && empty($param['compare'])) {
        $param_str .= rawurlencode($param['name']) . "%20'" . $param['value'] . "'";
      }
      // Case 3.
      elseif (empty($param['value']) && empty($param['compare'])) {
        $param_str .= rawurlencode($param['name']) . "%20";
      }
      $is_first = FALSE;
    }
    return $param_str;
  }
}
