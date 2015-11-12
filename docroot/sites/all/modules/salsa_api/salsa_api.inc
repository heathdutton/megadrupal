<?php

/*
 * @file
 * Contains the API class for Salsa API.
 */

/**
 * Service class to execute API calls to Salsa.
 */
class SalsaAPI implements SalsaAPIInterface {

  /**
   * URL to the Salsa API service.
   *
   * @var string
   */
  protected $url;

  /**
   * Username for authentication.
   *
   * @var string.
   */
  protected $username;

  /**
   * Authentication password.
   *
   * @var string
   */
  protected $password;

  /**
   * Curl connection resource.
   *
   * @var resource
   */
  protected $connection;

  /**
   * Constructor.
   */
  public function __construct() {
    $this->url = variable_get('salsa_api_url', NULL);
    $this->username = variable_get('salsa_api_username', NULL);
    $this->password = variable_get('salsa_api_password', NULL);
  }

  /**
   * Implements SalsaAPIInterface::query().
   */
  public function query($script, $query, array $curl_arguments = array()) {
    $this->connect();

    // Apply the referer of the current page by default, this is used by salsa
    // in same cases, e.g. as Source_Detail for a new supporter.
    if (!isset($curl_arguments[CURLOPT_REFERER])) {
      $curl_arguments[CURLOPT_REFERER] = url(current_path(), array('absolute' => TRUE));
    }

    foreach ($curl_arguments as $name => $value) {
      curl_setopt($this->connection, $name, $value);
    }
    curl_setopt($this->connection, CURLOPT_URL, $this->url . $script);
    curl_setopt($this->connection, CURLOPT_POSTFIELDS, $query);
    $curl_query_result = curl_exec($this->connection);
    if ($error = curl_error($this->connection)) {
      throw new SalsaQueryException((string) $error);
    }
    else {
      return $curl_query_result;
    }
  }

  /**
   * Implements SalsaAPIInterface::delete().
   */
  public function delete($object, array $keys) {
    $query[] = 'xml';
    foreach ($keys as $key) {
      $query[] = 'object=' . $object;
      $query[] = 'key=' . $key;
    }

    $this->query('/delete', implode('&', $query));

    /// @todo Implement parsing of query results.

    // This is not possible right now because salsa API returns an invalid
    // response for supporter, donation and probably for some other objects.
    // In case of an invalid response, simplexml_load_string() function
    // displays warning messages.

    /*$xml = simplexml_load_string($result);
    if (!$xml) {
       throw new SalsaQueryException("Unable to retrieve response.");
    }*/

  }

  /**
   * Implements SalsaAPIInterface::deleteTag().
   */
  public function deleteTag($object, $key, $tag) {
    $script = '/deleteTag';
    $query  = 'table=' . $object . '&key=' . $key . '&tag=' . $tag;
    $this->query($script, $query);
  }

  /**
   * Implements SalsaAPIInterface::getCount().
   */
  public function getCount($object, array $conditions = array(), $columnCount = NULL) {
    if (!$columnCount) {
      $columnCount = $object . '_KEY';
    }
    $query[] = 'object=' . $object;
    if ($conditions) {
      $query[] = $this->buildConditionString($conditions);
    }
    $query[] = 'columnCount=' . $columnCount;
    $result = $this->query('/api/getCount.sjs', implode('&', $query));
    $xml = simplexml_load_string($result);
    return (int)$xml->{$object}->count;
  }

  /**
   * Implements SalsaAPIInterface:getCounts().
   */
  public function getCounts($object, array $groupBy = array(), array $conditions = array(), $columnCount = NULL, array $orderBy = array(), $limit = NULL) {
    if (!$columnCount) {
      $columnCount = $object . '_KEY';
    }
    $query[] = 'object=' . $object;
    if ($conditions) {
      $query[] = $this->buildConditionString($conditions);
    }
    if ($groupBy) {
      $query[] = 'groupBy=' . rawurlencode(implode(',', $groupBy));
    }
    if ($conditions) {
      $query[] = $this->buildConditionString($conditions);
    }
    $query[] = 'columnCount=' . $columnCount;
    if ($orderBy) {
      $query[] = 'orderBy=' . rawurlencode(implode(',', $orderBy));
    }
    if ($limit) {
      $query[] = 'limit=' . rawurldecode($limit);
    }
    $result = $this->parseResult($this->query('/api/getCounts.sjs', implode('&', $query)));
    if (!isset($result[$object]['count'])) {
      throw new SalsaQueryException((string)$result['h3']);
    }
    else {
      return $result[$object]['count'];
    }

    return $result;
  }

  /**
   * Builds a condition string that can be sent to Salsa.
   *
   * @param $conditions
   *   Array of conditions. The key is the column name, the value can be one of
   *   - A value: used as is, with the = operator.
   *   - An array of values: Imploded with , and the IN operator is used.
   *   - A value with a %: The LIKE operator is used.
   *   - An array with the keys #operator and #value. Supporter operators are
   *     =, >=, <=, <|>, LIKE, IN, NOT IN, IS NOT EMPTY, IS EMPTY. EMPTY is
   *     equal to an NULL OR empty value.
   * @return
   *   A conditions string.
   */
  function buildConditionString(array $conditions) {
    $condition_string = NULL;
    $condition_strings = array();
    foreach ($conditions as $key => $condition) {
      $operator = '=';
      if (is_array($condition) && isset($condition['#operator'])) {
        $operator = $condition['#operator'];
        $condition = $condition['#value'];
      }
      // Default to IN operator when we have an array but no explicit operator.
      else if (is_array($condition)) {
        if (count($condition) > 1) {
          $operator = 'IN';
        }
      }
      else if (strpos($condition, '%') !== FALSE) {
        $operator = 'LIKE';
      }
      // Convert an array of values to a comma separated list.
      if (is_array($condition)) {
	$condition = implode(',', $condition);
      }
      $condition_strings[] = $key . $operator . urlencode($condition);
    }
    if (!empty($condition_strings)) {
      $condition_string = 'condition=' . implode('&condition=', $condition_strings);
    }
    return $condition_string;
  }

  /**
   * Implements SalsaAPIInterface::upload().
   */
  public function upload($file, $properties) {
    $boundary = "----image_upload_" . REQUEST_TIME;

    $data = "--$boundary\r\n";
    $data .= "Content-Disposition: form-data; name=\"file\"; filename=\"" . basename($file->uri) . "\"\r\n";
    $data .= "Content-Type: " . $file->filemime . "\r\n\r\n";
    $data .= file_get_contents($file->uri) . "\r\n";

    foreach ($properties as $key => $value) {
      $data .= "--$boundary\r\n";
      $data .= "Content-Disposition: form-data; name=\"" . $key . "\"\r\n\r\n";
      $data .= $value . "\r\n";
    }
    $data .= "--$boundary--\r\n";

    $curl_arguments = array(CURLOPT_HTTPHEADER => array('Content-Type: multipart/form-data; boundary=' . $boundary));
    // @todo: Handle response. Requires manual header parsing, see
    // http://stackoverflow.com/questions/9183178/php-curl-retrieving-response-headers-and-body-in-a-single-request
    $this->query('/o/' . $properties['organization_KEY'] . '/p/salsa/upload', $data, $curl_arguments);
  }

  /**
   * Implements SalsaAPIInterface::getLeftJoin().
   */
  public function getLeftJoin($objects, array $conditions = array(), $limit = NULL, array $include = array(), array $orderBy = array(), array $groupBy = array()) {
    if ($objects) {
      $query[] = 'object=' . rawurlencode($objects);
    }
    if ($conditions) {
      $query[] = $this->buildConditionString($conditions);
    }
    if ($limit) {
      $query[] = 'limit=' . rawurldecode($limit);
    }
    if ($include) {
      $query[] = 'include=' . rawurlencode(implode(',', $include));
    }
    if ($groupBy) {
      $query[] = 'groupBy=' . rawurlencode(implode(',', $groupBy));
    }
    if ($orderBy) {
      $query[] = 'orderBy=' . rawurlencode(implode(',', $orderBy));
    }

    $result = $this->parseResult($this->query('/api/getLeftJoin.sjs', implode('&', $query)));

    $objectlist = str_replace(array('(', ')'), array('-', '-'), $objects);

    // If there are any items, convert them to objects and return.
    $salsaobjects = array();
    if ($result[$objectlist]['count'] > 0) {
      // If count is 1, item is the object.
      if ($result[$objectlist]['count'] == 1) {
	$result[$objectlist]['item'] = array($result[$objectlist]['item']);
      }
      $index = 0;
      foreach ($result[$objectlist]['item'] as $item) {
	$salsaobjects[$index] = (object)$item;
	$index++;
      }
    }
    return $salsaobjects;
  }

  /**
   * Implements SalsaAPIInterface::getObject().
   */
  public function getObject($object, $key) {
    $query[] = 'object=' . rawurldecode($object);
    $query[] = 'key=' . rawurldecode($key);

    $result = $this->parseResult($this->query('/api/getObject.sjs', implode('&', $query)));

    if (isset($result[$object]['item']['result']) && ($result[$object]['item']['result'] == 'error')) {
      throw new SalsaQueryException((string)$result[$object]['item']['messages']);
    }
    else {
      return $result[$object]['item'];
    }
  }

  /**
   * Implements SalsaAPIInterface::getObjects().
   */
  public function getObjects($object, array $conditions = array(), $limit = NULL, array $include = array(), array $orderBy = array(), array $groupBy = array()) {
    $script = '/api/getObjects.sjs';
    $query[] = 'object=' . rawurldecode($object);
    if ($conditions) {
      $query[] = $this->buildConditionString($conditions);
    }
    if ($limit) {
      $query[] = 'limit=' . rawurldecode($limit);
    }
    if ($include) {
      $query[] = 'include=' . rawurlencode(implode(',', $include));
    }
    if ($groupBy) {
      $query[] = 'groupBy=' . rawurlencode(implode(',', $groupBy));
    }
    if ($orderBy) {
      $query[] = 'orderBy=' . rawurlencode(implode(',', $orderBy));
    }

    $result = $this->parseResult($this->query($script, implode('&', $query)));

    // If there are any items, convert them to objects and return.
    $objects = array();
    if ($result[$object]['count'] > 0) {
      // If count is 1, item is the object.
      if ($result[$object]['count'] == 1) {
        $result[$object]['item'] = array($result[$object]['item']);
      }

      foreach ($result[$object]['item'] as $item) {
        $objects[$item['key']] = (object)$item;
      }
    }
    return $objects;
  }

  /**
   * Implements SalsaAPIInterface::getReport().
   */
  public function getReport($key) {
    $query[] = 'report_KEY=' . rawurldecode($key);

    $result = $this->parseResult($this->query('/api/getReport.sjs', implode('&', $query)));

    $report = $result['report'];

    if (empty($report)) {
      throw new SalsaQueryException("Unable to retrieve report #" . $key . ". Does it exist?");
    }
    else {
     return $report;
    }
  }

  /**
   * Implements SalsaAPIInterface::describe().
   */
  public function describe($table) {
    $query[] = 'object=' . rawurldecode($table);

    $result = $this->parseResult($this->query('/api/describe.sjs', implode('&', $query)));

    $schema = $result[$table]['item'];

    if (empty($schema)) {
      throw new SalsaQueryException("Unable to retrieve schema for table " . $table . ". Does it exist?");
    }
    else {
      return $schema;
    }

  }

  /**
   * Implements SalsaAPIInterface::describe2().
   */
  public function describe2($table) {
    $query[] = 'object=' . rawurldecode($table);

    $result = $this->parseResult($this->query('/api/describe2.sjs', implode('&', $query)));

    $schema = $result[$table]['item'];

    if (empty($schema)) {
      throw new SalsaQueryException("Unable to retrieve schema for table " . $table . ". Does it exist?");
    }
    else {
      return $schema;
    }

  }

  /**
   * Implements SalsaAPIInterface::save().
   */
  public function save($object, array $fields = array(), array $links = array()) {
    $query[] = "xml";
    $query[] = 'object=' . rawurldecode($object);
    if ($fields) {
      foreach ($fields as $field => $value) {
	$query[] = rawurlencode($field) . "=" . rawurlencode($value);
      }
    }
    if ($links) {
      foreach ($links as $link) {
	$query[] = 'link=' . rawurlencode($link['link']);
	$query[] = 'linkKey=' . rawurlencode($link['linkkey']);
      }
    }

    // Parse the result, on success, there is a success tag that has an
    // attribute key.
    $result = simplexml_load_string($this->query("/save", implode('&', $query)));
    if (isset($result->success)) {
      return (int)$result->success['key'];
    }
    else {
      // The error message is in the error tag.
      throw new SalsaQueryException((string)$result->error);
    }
  }

  /**
   * Parse the query result returned by SalsaAPI::query().
   *
   * @param $result
   *   The query result.
   *
   * @return array|null
   *   The XML response converted to an array structure or NULL in case of an
   *   error.
   *
   * @throws SalsaQueryException
   */
  protected function parseResult($result) {
    if (preg_match("<div class='sjs error'>", $result)) {
      throw new SalsaQueryException(strip_tags($result));
    }
    else {
      return salsa_api_object2array(simplexml_load_string($result));
    }
    if (isset($result['error'])) {
      throw new SalsaQueryException($result['error']);
    }
  }

  /**
   * Connects and authenticates to Salsa API if there is no open connection yet.
   *
   * @return
   *   TRUE if the connection was successful.
   *
   * @throws SalsaConnectionException
   */
  public function connect() {
    // We already have an connection, just return TRUE.
    if ($this->connection) {
      return TRUE;
    }

    $temp_dir = file_directory_temp();
    $this->connection = curl_init();
    curl_setopt($this->connection, CURLOPT_POST, 1);
    curl_setopt($this->connection, CURLOPT_RETURNTRANSFER, 1);
    curl_setopt($this->connection, CURLOPT_CONNECTTIMEOUT, 10);
    curl_setopt($this->connection, CURLOPT_TIMEOUT, variable_get('salsa_api_curlopt_timeout', 10));
    curl_setopt($this->connection, CURLOPT_COOKIESESSION, TRUE);
    // Development: on windows dev server the certificate check fails, so we
    // disable the check.
    curl_setopt($this->connection, CURLOPT_SSL_VERIFYPEER, 0);

    curl_setopt($this->connection, CURLOPT_COOKIEFILE, '/' . $temp_dir . '/cookies_file');
    curl_setopt($this->connection, CURLOPT_COOKIEJAR, '/' . $temp_dir . '/cookies_file');
    curl_setopt($this->connection, CURLOPT_URL, $this->url . "/api/authenticate.sjs");
    curl_setopt($this->connection, CURLOPT_POSTFIELDS, "email=" . urlencode($this->username) . "&password=" . urlencode($this->password));
    $auth = curl_exec($this->connection);
    $response = simplexml_load_string($auth);
    if (isset($response->message) && (string) $response->message == 'Successful Login') {
      return TRUE;
    }
    else {
      // Connection failed, write log message and disconnect.
      watchdog('salsa', '%url/api/authenticate.sjs?email=**&password=** call result: %reply', array('%url' => $this->url, '%reply' => $auth), WATCHDOG_ERROR);
      $this->disconnect();
      throw new SalsaConnectionException((string) $response->error);
    }
  }

  /**
   * Disconnects and closes the connection.
   */
  public function disconnect() {
    if ($this->connection) {
      curl_close($this->connection);
    }
  }

  /**
   * Deconstructor.
   */
  public function __destruct() {
    // Close the connection if it has been opened.
    $this->disconnect();
  }
}

/**
 * Declare the interface 'SalsaAPIInterface'.
 */
interface SalsaAPIInterface {

  /**
   * Runs a query against Salsa using cURL
   *
   * @param $script
   *   The name of the Salsa script to call, e.g. getObject.sjs
   * @param $query
   *   POST data to send to Salsa.
   *
   * @return string
   *   The raw result of the cURL query if successful.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function query($script, $query);

  /**
   * Deletes a Salsa Objects.
   *
   * @param string $object
   *   Salsa entity object / table.
   * @param array $keys
   *   Primary key value.
   *
   * @throws SalsaQueryException
   *   An exception is thrown if the delete fails.
   */
  public function delete($object, array $keys);

  /**
   * Deletes a tag from a Salsa object.
   *
   * @param $object
   *   Salsa entity object / table.
   * @param $key
   *   Salsa entity key.
   * @param $tag
   *   Tag to be deleted.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function deleteTag($object, $key, $tag);

  /**
   * Uploads a file to Salsa.
   *
   * @param $file
   *   Uploaded file object.
   * @param $properties
   *   An array of properties (content disposition key/value pairs).
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function upload($file, $properties);

  /**
   * Performs a simple count of items in the Salsa database matching a given
   * condition.
   *
   * @param $object
   *   The Salsa object/table to query
   * @param array $conditions
   *   An array of conditions. The key is the column name, the value can be one
   *   of:
   *   - A value: used as is, with the = operator.
   *   - An array of values: Imploded with , and the IN operator is used.
   *   - A value with a %: The LIKE operator is used.
   *   - An array with the keys #operator and #value. Supporter operators are
   *     =, >=, <=, <|>, LIKE, IN, NOT IN, IS NOT EMPTY, IS EMPTY. EMPTY is
   *     equal to an NULL OR empty value.
   * @param $columnCount
   *   The column to count.
   *
   * @return int
   *   The count of items matching the given condition.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function getCount($object, array $conditions = array(), $columnCount = NULL);

  /**
   * Performs a grouped count of items in the Salsa database matching a given
   * condition.
   *
   * @param $object
   *   The Salsa object/table to query.
   * @param array $groupBy
   *   An array of columns to group by.
   * @param array $conditions
   *   An array of conditions. The key is the column name, the value can be one
   *   of:
   *   - A value: used as is, with the = operator.
   *   - An array of values: Imploded with , and the IN operator is used.
   *   - A value with a %: The LIKE operator is used.
   *   - An array with the keys #operator and #value. Supporter operators are
   *     =, >=, <=, <|>, LIKE, IN, NOT IN, IS NOT EMPTY, IS EMPTY. EMPTY is
   *     equal to an NULL OR empty value.
   * @param $columnCount
   *   The column to count.
   * @param array $orderBy
   *   An array of columns to order by. Use -column to sort descending.
   * @param $limit
   *   Limit the number of returned objects, Either an integer or "offset,limit"
   *   No more than 100 objects can be returned at once.
   *
   * @return array
   *   Array of various counts. If no groupBy column is specified, inside the
   *   returned array are four string elements:
   *   - count: The number of items matching the condition
   *   - sum: The sum of the items in columnCount in the result set
   *   - max: The largest value in the columnCount in the result set
   *   - min: The smallest value in the columnCount in the result set
   *   If one or more groupBy columns are specified, the returned array contains
   *   a numbered array for each group of the result set. Inside this array are
   *   five string elements:
   *   - [groupBy column name]: The value of the named groupBy column used to
   *     group the result set
   *   - count: The number of items in this group of the result set
   *   - sum: The sum of the items in the columnCount in this group of the
   *     result set
   *   - max: The largest value in the columnCount in this group of the result
   *     set
   *   - min: The smallest value in the columnCount in this group of the result
   *     set
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function getCounts($object, array $groupBy = array(), array $conditions = array(), $columnCount = NULL, array $orderBy = array(), $limit = NULL);

  /**
   * Queries across multiple Salsa objects by performing a left join.
   *
   * @param string $objects
   *   String of the objects and how they should be joined, in the form of
   *   tableA(joinField)tableB, e.g. supporter(supporter_KEY)donation. See
   *   https://help.salsalabs.com/entries/23353157-Using-getLeftJoin-sjs-and-conditions
   *   for more information.
   * @param array $conditions
   *   Array of conditions. The key is the column name, the value can be one of:
   *   - A value: used as is, with the = operator.
   *   - An array of values: Imploded with , and the IN operator is used.
   *   - A value with a %: The LIKE operator is used.
   *   - An array with the keys #operator and #value. Supporter operators are
   *     =, >=, <=, <|>, LIKE, IN, NOT IN, IS NOT EMPTY, IS EMPTY. EMPTY is
   *     equal to an NULL OR empty value.
   * @param $limit
   *   Limit the number of returned objects, Either an integer or "offset,limit"
   *   No more than 100 objects can be returned at once.
   * @param array $include
   *   An array of columns that should be included in the response. Everything
   *   is returned if empty.
   * @param array $orderBy
   *   An array of columns to order by. Use -column to sort descending.
   * @param array $groupBy
   *   An array of columns to group by.
   *
   * @return array
   *   Array of objects.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function getLeftJoin($objects, array $conditions = array(), $limit = NULL, array $include = array(), array $orderBy = array(), array $groupBy = array());

  /**
   * Retrieves a single Salsa object.
   *
   * @param $object
   *   Name of the salsa object/table, e.g. supporter.
   * @param $key
   *   Key of the object to return, e.g. supporter_KEY.
   *
   * @return array
   *   Array containing the Salsa object requested.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function getObject($object, $key);

  /**
   * Returns multiple Salsa objects matching a given condition.
   *
   * @param $object
   *   Name of the salsa object/table, e.g. supporter.
   * @param array $conditions
   *   Array of conditions. The key is the column name, the value can be one of
   *   - A value: used as is, with the = operator.
   *   - An array of values: Imploded with , and the IN operator is used.
   *   - A value with a %: The LIKE operator is used.
   *   - An array with the keys #operator and #value. Supporter operators are
   *     =, >=, <=, <|>, LIKE, IN, NOT IN, IS NOT EMPTY, IS EMPTY. EMPTY is
   *     equal to an NULL OR empty value.
   * @param array $limit
   *   Limit the number of returned objects, Either an integer or "offset,limit"
   *   No more than 100 objects can be returned at once.
   * @param array $include
   *   An array of columns that should be included in the response. Everything
   *   is returned if empty.
   * @param array $orderBy
   *   An array of columns to order by. Use -column to sort descending.
   * @param array $groupBy
   *   An array of columns to group by.
   *
   * @return array
   *   Array of objects.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function getObjects($object, array $conditions = array(), $limit = NULL, array $include = array(), array $orderBy = array(), array $groupBy = array());

  /**
   * Retrieves a report from Salsa.
   *
   * @param $key
   *   The report_KEY of the report to retrieve.
   *
   * @return array
   *   An array containing the report. This array contains a 'row' array, which
   *   contains an array for each row of the report. Inside the 'row' array is
   *   an associative array containing each column/field and value in that row.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function getReport($key);

  /**
   * Retrieves the database schema of a Salsa object.
   *
   * @param $table
   *   The name of the Salsa object for which to retrieve the schema.
   *
   * @return array
   *   An array containing the object's database schema. Each numbered array
   *   element describes one column of the database, and contains six arrays,
   *   each containing a string describing one attribute of that column. Any
   *   attribute without a value is set to NULL.
   *    - Field: The field name
   *    - Type: The SQL datatype for the field (e.g. int(16))
   *    - Null: Are null values allowed? (YES, NO)
   *    - Key: Is the field a MySQL key? (PRI, UNI, MUL)
   *    - Default: The default value to use if none is supplied
   *    - Extra: Any extra attributes for the column (e.g. auto_increment)
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function describe($table);

  /**
   * Retrieves the database schema of a Salsa object, in a different format
   * than used by describe().
   *
   * @param $table
   *   The name of the Salsa object for which to retrieve the schema.
   *
   * @return array
   *   An array containing the object's database schema. Each numbered array
   *   element describes one column of the database, and contains five arrays,
   *   each containing a string describing one attribute of that column. Any
   *   attribute without a value is set to NULL.
   *    - name: The machine-readable name of the field
   *    - nullable: Are null values allowed (0 or 1)
   *    - type: The SQL datatype of the field (e.g., int, varchar)
   *    - defaultValue: The default value to use if none is supplied
   *    - label: The human-readable name of the field
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function describe2($table);

  /**
   * Save Salsa objects.
   *
   * @param $object
   *   Name of the Salsa object/table to save to, e.g. supporter.
   * @param array $fields
   *   An associative array of fields (keys) and values to save.
   * @param array $links
   *   An array containing associative arrays of link tables and linkKeys. Used
   *   when saving to more than one table at a time.
   *
   * @return int
   *   The key of the updated or newly created object.
   *
   * @throws SalsaQueryException
   * @throws SalsaConnectionException
   */
  public function save($object, array $fields = array(), array $links = array());
}

/**
 * Exception thrown in case an error is returned by Salsa.
 */
class SalsaQueryException extends Exception { }

/**
 * Exception thrown when the initial connection or authentication failed.
 */
class SalsaConnectionException extends SalsaQueryException { }
