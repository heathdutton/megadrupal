<?php

/**
 * NYTimes BestSellers API class
 * This is based on code originally written by
 * @author Sameer Borate <www.codediesel.com>
 * @license http://opensource.org/licenses/lgpl-license.php GNU Lesser General Public License Version 2.1
 */
    
class NYTBooks {

  /* API url */
  private $_api_url = "http://api.nytimes.com/svc/books/v2/lists";

  /* API key */
  private $_api_key = "";

  /* Set default page offset. */
  private $_offset = 0;

  /* Set default list publication date to the current date. */
  private $_date = "";

  /* Set default response type to json. But can be one of json | xml | sphp */
  private $_response_type = "json";

  /* Set default list type */
  private $_list_type = "Combined-Print-Fiction";

  function __construct($api_key) {
    $this->_api_key = $api_key;

    /* Default list date to current date */
    $this->_date = date("Y-m-d");
  }

  /** 
  * CURL to processes url and returns data 
  * 
  * @param  string    $data_url   NYT api url 
  * @return string response 
  */
  private function get_nyt_data($data_url) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $data_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_TIMEOUT, 30);
    $response = curl_exec($ch);
    return $response;
  }

  /** 
  * Returns a reponse of 'list' types for use with the query() function
  *
  * 
  * @return string response of list names
  */
  public function get_list_types() {
      return $this->get_nyt_data("{$this->_api_url}/names.{$this->_response_type}?api-key={$this->_api_key}");
  }

  /** 
  * Set response format. Can be json | xml | sphp
  * 
  * @param  string    $format
  */
  public function format($format) {
    $available_formats = array("json", "xml", "sphp");

    /* Default to XML if a wrong format is given */
    if(in_array($format, $available_formats)) {
      $this->_response_type = $format;
      return $this;
    }
  }

  /** 
  * Sets the starting point of the result set
  *
  * The first 20 results are shown by default.
  * To page through the results, set offset to the appropriate value
  * 
  * NOTE: Positive integer, multiple of 20
  * 
  * @param  string    $offset
  */
  public function offset($offset) {
    /* Ensure offset is a multiple of 20 */
    if($offset%20 == 0) {
      $this->_offset = $offset;
      return $this;
    }
  }

  /** 
  * Sets the list publication date
  *
  * @param  string    $date
  */
  public function listDate($date) {
    $this->_date = $date;
    return $this;
  }

  /** 
  * Sets the list type.
  * 
  * @param  string    $list_type
  */
  public function listType($list_type) {
    $this->_list_type = $list_type;
    return $this;
  }

  /** 
  * Query the NYTimes api
  * 
  * @param  string    $list_name
  * NOTE: You can get the list names by using the get_list_types() function above
  * 
  * @return string response
  */
  public function get_bestsellers() {
    return $this->get_nyt_data("{$this->_api_url}/{$this->_date}/{$this->_list_type}.{$this->_response_type}?offset={$this->_offset}&api-key={$this->_api_key}");
  }

  /**
   * Set the list name.
   */
  public function listName($list_name) {
    $this->_list_name = $list_name;
   return $this;
  }

  /**
   * Set the particular ISBN.
   */
  public function isbn($isbn) {
    $this->_isbn = $isbn;
   return $this;
  }

  /**
   * Add ISBN and specific list.
   */
  public function get_bestseller_by_isbn() {
    return $this->get_nyt_data("{$this->_api_url}.{$this->_response_type}?list-name={$this->_list_name}&date={$this->_date}&isbn={$this->_isbn}&offset={$this->_offset}&api-key={$this->_api_key}");
  }

}
    
?>