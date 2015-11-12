<?php

/**
 * @file
 * Flocknote SDK for PHP.
 *
 * The official Flocknote API integration SDK for PHP.
 *
 * @version 1.0-beta3
 * @see http://www.flocknote.com/help/api
 * @author Jeff Geerling
 */

class Flocknote {

  protected $api_app_id, $api_key;
  protected $username, $password;
  protected $authenticated;
  protected $endpoint = 'https://www.flocknote.com/api/';
  protected $operation = 'GET';
  protected $body = '';
  protected $id;
  protected $method, $sub_method;

  /**
   * Creates a new Flocknote instance.
   */
  public function __construct($api_app_id = '', $api_key = '') {
    $this->api_app_id = $api_app_id;
    $this->api_key = $api_key;
  }

  /**
   * Set username credentials.
   */
  public function setUserCredentials($username, $password) {
    $this->username = $username;
    $this->password = $password;
  }

  /**
   * Set alternate endpoint (only used for testing).
   */
  public function setEndpoint($endpoint) {
    $this->endpoint = $endpoint;
  }

  /**
   * Get the user credentials.
   *
   * @return (array)
   *   Array with the user's username and password.
   */
  public function getUserCredentials() {
    return array(
      'username' => $this->username,
      'password' => $this->password,
    );
  }

  /**
   * HTTP Operation getter and setter.
   *
   * Valid API operations follow basic CRUD->HTTP conventions:
   *   GET: Read operations.
   *   POST: Create operations.
   *   PUT: Update operations.
   *   DELETE: Delete operations.
   */
  public function setOperation($operation) {
    $this->operation = 'GET';
  }
  public function getOperation() {
    return $this->operation;
  }

  /**
   * API Method getter and setter.
   *
   * Valid API methods include:
   *   networks
   *   lists
   *   notes
   *   members
   *   login
   */
  public function setMethod($method) {
    $this->method = $method;
  }
  public function getMethod() {
    return $this->method;
  }

  /**
   * API Sub-method getter and setter.
   *
   * For certain methods, you can specify a sub-method to request particular
   * information relating to a particular piece of content. For example, if you
   * want to get members of list 123, you would send a request to:
   * api/lists/123/members. In this example, 'members' is the sub-method.
   *
   * Sub-methods include:
   *    lists:
   *      notes
   *      members
   */
  public function setSubMethod($sub_method) {
    $this->sub_method = $sub_method;
  }
  public function getSubMethod() {
    return $this->sub_method;
  }

  /**
   * ID getter and setter.
   *
   * The ID is used in requests to identify a particular piece of content or
   * member. Only use an ID if you wish to view or change a particular item.
   */
  public function setId($id) {
    $this->id = $id;
  }
  public function getId() {
    return $this->id;
  }

  /**
   * Body getter and setter.
   *
   * @param $body
   *   Either an array of information to be encoded (in which case, $encoded
   *   variable should be FALSE), or a JSON string (in which case, $encoded
   *   variable should be TRUE).
   * @param $encode
   *   Boolean telling method whether the $body is already encoded as JSON.
   *
   * The body is used in certain requests to update or create objects. Pass in
   * an array of values, which will be encoded as JSON by this method.
   */
  public function setBody($body, $encoded = FALSE) {
    $this->body = $encoded ? $body : json_encode($body);
  }
  public function getBody() {
    $this->body = $body;
  }

  /**
   * Clears the current operation, body, ID, method, and sub_method.
   *
   * Use this method after sending a request to clear out the old values so you
   * can use the same object to send a different request.
   */
  public function resetParams() {
    $this->operation = 'GET';
    $this->body = NULL;
    $this->id = NULL;
    $this->method = NULL;
    $this->sub_method = NULL;
  }

  /**
   * Check a user's login.
   *
   * If the credentials are valid, the 'authenticated' parameter will be set to
   * TRUE, and this method will return TRUE.
   *
   * @return (bool)
   *   TRUE if the credentials are valid, FALSE if invalid.
   */
  public function logIn() {
    // Send a request to 'login' and make sure result is 200 OK.
    $this->method = 'login';
    $result = $this->sendRequest();

    if ($result['http_code'] == 200) {
      $this->authenticated = TRUE;
      return TRUE;
    }
    else {
      $this->authenticated = FALSE;
      return FALSE;
    }
  }

  /**
   * Get a note.
   *
   * @param $id
   *   Note ID to retrieve.
   *
   * @return $note
   *   An array of note information.
   */
  public function getNote($id) {
    $this->method = 'notes';
    $this->id = $id;

    // Send the request to and return the result body if note is found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Add a new note.
   *
   * @param $note
   *   Array of note information to be saved. Must include at least:
   *     - list_id
   *     - title
   *     - body
   *
   * @return (array)
   *   Same array passed in, and will include 'id' in array if note was created
   *   successfully. If note was not created successfully, the array will
   *   include an 'error' with a message.
   */
  public function addNote($note = array()) {
    // If there's already an ID set, someone's trying to add a note that exists!
    if (isset($note['id'])) {
      return $note;
    }

    // Make sure the proper fields were passed in.
    $required = array(
      'list_id',
      'title',
      'body',
    );
    if ($error_message = $this->checkArrayRequiredKeys('Note', $note, $required)) {
      $note['error'] = $error_message;
      return $note;
    }

    $this->method = 'notes';
    $this->operation = 'POST';

    // Build the request.
    $this->setBody($note);
    $response = $this->sendRequest();
    return $this->returnResponse($response, 201);
  }

  /**
   * Get a list.
   *
   * @param $id
   *   List ID to retrieve.
   *
   * @return $list
   *   An array of list information.
   */
  public function getList($id) {
    $this->method = 'lists';
    $this->id = $id;

    // Send the request and return the result body if list is found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get a list's most recent notes.
   *
   * @param $id
   *   List ID for which notes will be retrieved.
   *
   * @return $notes
   *   An array of notes for this list.
   */
  public function getListNotes($id) {
    $this->method = 'lists';
    $this->id = $id;
    $this->sub_method = 'notes';

    // Send a request and return the result body if notes were found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get a list's members.
   *
   * @param $id
   *   List ID for which members will be retrieved.
   *
   * @return $members
   *   An array of members for this list.
   */
  public function getListMembers($id) {
    $this->method = 'lists';
    $this->id = $id;
    $this->sub_method = 'members';

    // Send a request and return the result body if member was found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get a list's permissions for the current member.
   *
   * @param $id
   *   List ID for which permissions will be retrieved.
   *
   * @return $permissions
   *   An array of permissions for this list for the current member.
   */
  public function getListPermissions($id) {
    $this->method = 'lists';
    $this->id = $id;
    $this->sub_method = 'permissions';

    // Send a request and return the result body if permissions were found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get lists for current member.
   *
   * @return $lists
   *   An array of lists.
   */
  public function getListsForCurrentMember() {
    $this->method = 'lists';

    // Send the request and return the result body if lists are found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get a network.
   *
   * @param $id
   *   Network ID to retrieve.
   *
   * @return $network
   *   An array of network information.
   */
  public function getNetwork($id) {
    $this->method = 'networks';
    $this->id = $id;

    // Send the request and return the result body if network is found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get network information for current member.
   *
   * @return $networks
   *   An array of networks, along with each network's list information.
   */
  public function getNetworksForCurrentMember() {
    $this->method = 'networks';

    // Send the request and return the result body if networks are found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Get a member.
   *
   * @param $id (optional)
   *   Member ID to retrieve (current member if none given).
   *
   * @return $member
   *   An array of member information.
   */
  public function getMember($id = NULL) {
    $this->method = 'members';
    if ($id) {
      $this->id = $id;
    }

    // Send the request and return the result body if member is found.
    $response = $this->sendRequest();
    return $this->returnResponse($response);
  }

  /**
   * Add a member.
   *
   * @param $member
   *   Array of member information to be saved. Keys include:
   *     - first_name
   *     - last_name
   *     - email (required)
   *     - password
   *     - list_id (required)
   *
   * @return (array)
   *   Same array passed in, and will include 'id' in array if member was
   *   created successfully. If member was not created successfully, the array
   *   will include an 'error' with a message.
   */
  public function addMember($member = array()) {
    // If there's already an ID set, someone's trying to add a member that
    // exists!
    if (isset($member['id'])) {
      return $member;
    }

    // Make sure the proper fields were passed in.
    $required = array(
      'email',
      'list_id',
    );
    if ($error_message = $this->checkArrayRequiredKeys('Member', $member, $required)) {
      $member['error'] = $error_message;
      return $member;
    }

    $this->method = 'members';
    $this->operation = 'POST';

    // Build the request.
    $this->setBody($member);
    $response = $this->sendRequest();
    return $this->returnResponse($response, 201);
  }

  /**
   * Check a given array for given required keys.
   *
   * Each key must exist, and a value must be given for the key, otherwise, an
   * error message will be returned.
   *
   * @param $type
   *   The type of array, used for building message (e.g. 'Note' or 'Member').
   * @param $array
   *   Array to be checked.
   * @param $fields
   *   Array keys that should appear in $array.
   *
   * @return (mixed)
   *   FALSE if all keys are found. Error message (string) if a key is missing.
   */
  protected function checkArrayRequiredKeys($type, $array, $keys) {
    $missing_keys = array();

    foreach ($keys as $key) {
      if (empty($array[$key])) {
        $missing_keys[] = $key;
      }
    }

    // Return a message dependent on the number of missing keys found.
    $count = count($missing_keys);
    if ($count == 0) {
      return FALSE;
    }
    elseif ($count == 1) {
      return $type . " array is missing key: " . $missing_keys[0];
    }
    else {
      return $type . " array is missing keys: " . implode(', ', $missing_keys);
    }
  }

  /**
   * Send an API request with certain parameters.
   */
  protected function sendRequest() {
    $parameters = array(
      'app_id' => $this->api_app_id,
      'key' => $this->api_key,
      'user' => $this->username,
      'pass' => $this->password,
    );

    // Build the request URL.
    $url = $this->endpoint; // Base url.
    $url .= $this->method; // Add request method.
    // If there's a particular ID, add it to the URL.
    if (!empty($this->id)) {
      $url .= '/' . $this->id;
    }
    // If there's a sub-method, add it to the URL.
    if (!empty($this->sub_method)) {
      $url .= '/' . $this->sub_method;
    }
    $url .= '?' . http_build_query($parameters); // Add query params.

    // Set the proper request header.
    $header = array(
      'Content-Type: application/json',
      'Content-Length: ' . strlen($this->body),
    );

    // Set up the request with cURL.
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    // Add request body and set operation to POST if there is a body to POST.
    if ($this->operation == 'POST' && !empty($this->body)) {
      curl_setopt($ch, CURLOPT_POST, TRUE);
      curl_setopt($ch, CURLOPT_POSTFIELDS, $this->body);
    }

    // Send the request.
    $response = curl_exec($ch);
    $info = curl_getinfo($ch);
    curl_close($ch);

    // Pass back the result.
    return array(
      'response' => json_decode($response, TRUE),
      'http_code' => $info['http_code'],
      'content_type' => $info['content_type'],
      'content_length' => $info['download_content_length'],
      'total_time' => $info['total_time'],
      'request_url' => $info['url'],
    );
  }

  /**
   * Take a given response, and return the result, or FALSE.
   *
   * Use this method to ensure the response is what you expect and has some JSON
   * content. Only use this method if you're expecting this type of response.
   *
   * @param $result
   *   The result of an API request, as returned by sendRequest().
   * @param $success
   *   HTTP code expected if the result is successful.
   *
   * @return (mixed)
   *   JSON response data (array) if given, otherwise FALSE if the response was
   *   invalid.
   */
  protected function returnResponse($response, $success = 200) {
    // Make sure response returned proper code, and response has content.
    if ($response['http_code'] == $success && !empty($response['response'])) {
      return $response['response'];
    }
    return FALSE;
  }
}
