<?php
/**
 * @file
 * The connector class. This class handles the calls to adlib
 */

class AdLibConnector {
  /**
   * The URL on which the Adlib server is reachable
   */
  protected $baseurl;

  /**
   * Name of the database on the server.
   */
  protected $database;
  /**
   * Name of the image server.
   */
  protected $imageserver;

  /**
   * User credentials.
   */
  protected $user;
  protected $pwd;
  // Domain of which the user is a memeber.
  protected $domain;
  // Indicate if the user is authenticated.
  protected $isAuthenticated;

  /**
   * Errorhandling
   */
  // Last call that was made to the Adlib server.
  public $lastCall;
  // Indicate if there was a error during the last call.
  public $hasError;
  // String nwhich contains the last error.
  public $lastError;

  /**
   * Contructor.
   *
   * @param string $baseurl
   *   The base url to adlib.
   * @param string $database
   *   Database name.
   * @param string $imageserver
   *   Url of the imageserver.
   * @param string $user
   *   The username.
   * @param string $pwd
   *   The password
   * @param string $domain
   *   The domain.
   */
  public function __construct($baseurl, $database = '', $imageserver = '', $user = '', $pwd = '', $domain = '') {
    $this->baseurl = $baseurl;
    $this->database = $database;
    $this->imageserver = $imageserver;
    $this->user = $user;
    $this->pwd = $pwd;
    $this->domain = $domain;
    $this->isAuthenticated = FALSE;
    $this->hasError = FALSE;
    $this->lastError = "";
  }

  /**
   * Set database.
   *
   * @param string $dbname
   *   The machine name for the database.
   */
  public function setDatabase($dbname) {
    $this->database = $dbname;
  }

  /**
   * Authenticate user and start session.
   */
  public function startSession() {
    $xml = NULL;
    if (isset($this->user) && isset($this->pwd)) {
      $fullurl = $this->baseurl . '?database=' . $this->database . '&command=login&username=' . urlencode($this->user) . '&password=' . urlencode($this->pwd);
      $this->doCall($fullurl);
      // Start the session.
      $fullurl = $this->baseurl . '?database=' . $this->database . 'command=startsession';
      $this->doCall($fullurl);
    }
    else {
      $xml = simplexml_load_string(formatErrorFromString('No valid user and password'));
    }
    return $xml;
  }

  /**
   * End the session.
   */
  public function endSession() {
    $fullurl = $this->baseurl . '?command=endsession';
    $this->doCall($fullurl);
    if (!$this->hasError) {
      $fullurl = $this->baseurl . '?command=logout';
      $this->doCall($fullurl);
    }
  }

  /**
   * Perform a search on de database, paramaters are given in string.
   *
   * @param string $searchparameters
   *   String with all the arguments for the raw search
   * @param string $xmltype
   *   The type of XML to return.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query
   */
  public function rawQueryServer($searchparameters, $xmltype = 'grouped') {
    $fullurl = $this->baseurl . '?database=' . urlencode($this->database) . '&search=' . urlencode($searchparameters) . '&xmltype=' . urlencode($xmltype);
    $response = $this->doCall($fullurl);
    return $response;
  }


  /**
   * Perform as query with the parameters in the object.
   *
   * @param AdlibSearchQuery $query
   *   The object with the query to perform.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  public function performQuery($query) {
    // Construct parameters.
    $query_items = $query->getQueryItems();
    $fullurl = $this->baseurl . '?database=' . urlencode($this->database) . '&search=' . $query_items;
    $response = $this->doCall($fullurl);
    return $response;
  }

  /**
   * Get the version of the Adlib server we are conntected to.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  public function getVersion() {
    $fullurl = $this->baseurl . '?command=getversion';
    $response = $this->doCall($fullurl);
    return $response;
  }

  /**
   * Function to returns all the fields in the current database.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  public function getFieldList() {
    $fullurl = $this->baseurl . '?database=' . $this->database . '&command=getmetadata';
    $response = $this->doCall($fullurl);
    return $response;
  }

  /**
   * Returns the fields in the current database as associative array.
   *
   * @deprecated
   * @return array
   *   Associative array with all the available fields.
   */
  public function getFieldListAsArray() {
    // Get fields in XML.
    $response = $this->getFieldList();
    $xml = $response->getXMLObject();
    // Traverse XML to find all the fields and put them in associative array.
    $fields = array();

    $results = $xml->xpath('/adlibXML/recordList/record');
    foreach ($results as $node) {
      $tag = (string) $node->tag;
      $field_name = (string) $node->fieldName->value[0];
      $display_name = (string) $node->displayName->value[0];
      $fields[$tag] = array('fieldName' => $field_name, 'displayName' => $display_name);
    }
    return $fields;
  }

  /**
   * Get all records altered after given date. Date format is 'Y-m-d'.
   *
   * @param string $date
   *   Alter date.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  public function getAlteredRecordsByDate($date) {
    $fullurl = $this->baseurl . '?database=' . urlencode($this->database) . '&limit=1000&search=' . urlencode("modification greater '$date'");
    $response = $this->doCall($fullurl);
    return $response;
  }

  /**
   * ListDatabases.
   *
   * Get a list of adlib databases that can be accessed using the baseurl.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  public function listDatabases() {
    $fullurl = $this->baseurl . "?command=listdatabases";
    $response = $this->doCall($fullurl);
    return $response;
  }

  /**
   * Get the metadata of the fields in the adlib database.
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  public function getMetadata() {
    $fullurl = $this->baseurl . "?command=getmetadata&database=" . urlencode($this->database);
    $response = $this->doCall($fullurl);
    return $response;
  }

  /**
   * Get an image from the image server.
   *
   * @param AdlibImageQuery $image_query
   *   Image query object containing at the least the filename
   */
  public function getImage($image_query) {
    $fullurl = $this->baseurl . '?command=getcontent&server=' . $this->imageserver . '&' . $image_query->getQueryString();
    $response = $this->doCall($fullurl, 'image');
    return $response;
  }

  /**
   * Helper function to actually perform calls to AdLib server.
   *
   * @param string $fullurl
   *   The complete URL.
   * @param string $type
   *   (optional) Type of response (search or image)
   *
   * @return AdlibSearchResponse
   *   Object with the result of the query.
   */
  protected function doCall($fullurl, $type = 'search') {
    // Sanitize the url.
    $fullurl = drupal_strip_dangerous_protocols($fullurl);
    $this->lastCall = $fullurl;

    /*
     * Note that curl is used instead of the default drupal_http_request.
     * The reason is that there has been problems connecting to the adlib
     * system using drupal_http_request.
     */
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $fullurl);
    curl_setopt($ch, CURLOPT_HEADER, TRUE);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($ch, CURLOPT_USERAGENT, 'MuS search proxy');

    $rawdata = curl_exec($ch);
    $http_info = curl_getinfo($ch);

    switch ($type) {
      case 'image':
        // Create AdlibImageResponse object,
        // any request errors are handled in the constructor.
        $response = new AdlibImageResponse($rawdata, $http_info);
        break;

      default:
        // Create AdlibSearchResponse object,
        // any request errors are handled in the constructor.
        $response = new AdlibSearchResponse($rawdata, $http_info);
        break;
    }
    return $response;
  }
}
