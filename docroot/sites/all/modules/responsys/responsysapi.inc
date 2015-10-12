<?php

/**
 * @file
 * An API interface to the Responsys system.
 */

define('RESPONSYS_WSDL', 'https://ws2.responsys.net/webservices/wsdl/ResponsysWS_Level1.wsdl');
define('RESPONSYS_ENDPOINT', 'https://ws2.responsys.net/webservices/services/ResponsysWSService');
define('RESPONSYS_URI', 'urn:ws.rsys.com');
define('RESPONSYS_NO_UPDATE', 'NO_UPDATE');
define('RESPONSYS_REPLACE_ALL', 'REPLACE_ALL');
define('RESPONSYS_REPLACE_IF_EXISTING_BLANK', 'REPLACE_IF_EXISTING_BLANK');
define('RESPONSYS_REPLACE_IF_NEW_BLAN', 'REPLACE_IF_NEW_BLAN');

// Define EmailFormats.
define('RESPONSYS_TEXT_FORMAT', 'TEXT_FORMAT');
define('RESPONSYS_HTML_FORMAT', 'HTML_FORMAT');
define('RESPONSYS_MULTIPART_FORMAT', 'MULTIPART_FORMAT');
define('RESPONSYS_NO_FORMAT', 'NO_FORMAT');

/**
 * A command interface to Responsys.
 */
class ResponsysAPI {

  /**
   * public object $loginParameters
   *   The account username and password.
   */
  public $loginParameters;

  /**
   * public SoapClient $client
   *   The SoapClient reference.
   */
  public $client;

  /**
   * public string $sessionId
   */
  public $sessionId;

  /**
   * public string $logCallback
   *   A function name to execute to log errors.
   */
  public $logCallback;

  /**
   * public string $lastError
   *   The last reported error.
   */
  public $lastError = '';

  /**
   * Constructor, build a responsys api.
   *
   * @param string $username
   *   Your Responsys account username.
   * @param string $password
   *   Your Responsys account password.
   */
  public function __construct($username, $password) {
    $this->loginParameters = new stdClass();
    $this->loginParameters->username = $username;
    $this->loginParameters->password = $password;

    @$this->client = new SoapClient(RESPONSYS_WSDL, array(
      'location' => RESPONSYS_ENDPOINT,
      'uri' => RESPONSYS_URI,
      'trace' => TRUE,
    ));

    $this->login();
  }

  /**
   * Login and set the session id.
   */
  public function login() {
    if (empty($this->sessionId)) {
      $login_result = $this->client->login($this->loginParameters);
      $this->sessionId = $login_result->result->sessionId;

      $session_header = new SoapVar(array(
        'sessionId' => new SoapVar($this->sessionId, XSD_STRING, NULL, NULL, NULL, 'ws.rsys.com'),
      ), SOAP_ENC_OBJECT);

      $header = new SoapHeader('ws.rsys.com', 'SessionHeader', $session_header);
      $this->client->__setSoapHeaders(array($header));
      $jsession_id = $this->client->_cookies["JSESSIONID"][0];
      $this->client->__setCookie("JSESSIONID", $jsession_id);
    }
  }

  /**
   * Logout of responsys to preserve the session ids.
   */
  public function logout() {
    if (!empty($this->sessionId)) {
      $this->client->logout(array());

      // Clear the sessionId so we know we are logged out.
      $this->sessionId = '';
    }
  }

  /**
   * Get member objects for specific members in a specific list.
   *
   * @param string $folder
   *   The folder which contains the list.
   * @param string $list
   *   The name of the list in the folder.
   * @param array $ids
   *   A list of unique user ids to retrieve. What these
   *   ids refer to is defined by the query_column parameter
   *   Note there is a maximum of 200 results per query.
   * @param string $query_column
   *   Column to match. It can be RIID, EMAIL_ADDRESS,
   *   CUSTOMER_ID or MOBILE_NUMBER. Default is EMAIL_ADDRESS
   * @param array $field_list
   *   Fields to include in the return objects. Defaults to
   *   array('RIID_', 'EMAIL_ADDRESS_')
   *
   * @return array|bool
   *   A list of member objects with the fields requested
   *   or FALSE on error.
   */
  public function retrieveListMembers($folder, $list, array $ids, $query_column = 'EMAIL_ADDRESS', array $field_list = array('RIID_', 'EMAIL_ADDRESS_')) {
    if (empty($ids) || count($ids) > 200) {
      throw new ResponsysException('Responsys restrict maximum retrieveListQuery to 200 items and minimum of 1');
    }

    // Setup the query parameters object called args.
    $args = new stdClass();
    $args->list = new stdClass();
    $args->list->folderName = $folder;
    $args->list->objectName = $list;
    $args->queryColumn = $query_column;
    $args->fieldList = $field_list;
    $args->idsToRetrieve = $ids;

    $members = array();

    // Retrieve list members call.
    $result = $this->callMethod('retrieveListMembers', $args);

    if (!empty($result)) {
      $members = $this->convertFromResponsysList($result->recordData->fieldNames, $result->recordData->records);
    }

    return $members;
  }

  /**
   * Insert new members or update existing member fields in a given List.
   *
   * @param string $folder
   *   The folder which contains the list.
   * @param string $list
   *   The name of the list in the folder.
   * @param array $records
   *   A list of records to update. Each element in the array
   *   represents a user and this is made up of an associative array
   *   where the key is the field name and the value is the field
   *   value.
   * @param bool $insert
   *   When a record cannot be found to update a new record is created
   *   if this is set to TRUE. Default TRUE.
   * @param string $update_on_match
   *   One of the following constants
   *   RESPONSYS_NO_UPDATE,
   *   (default) RESPONSYS_REPLACE_ALL,
   *   RESPONSYS_REPLACE_IF_EXISTING_BLANK,
   *   RESPONSYS_REPLACE_IF_NEW_BLAN
   * @param string $match_column_1
   *   A string whose value is the name of a field
   *   to match on to determine if that record matches one
   *   in the $records array.
   *   Default is 'EMAIL_ADDRESS_'
   * @param string $match_column_2
   *   (optional) A string whose value is the name of a field
   *   to match on to determine if that record matches one
   *   in the $records array.
   *   Default is ''
   * @param string $match_column_3
   *   (optional) A string whose value is the name of a field
   *   to match on to determine if that record matches one
   *   in the $records array.
   *   Default is ''
   *
   * @TODO: deal with larger than 200 situation.
   */
  public function mergeListMembers($folder, $list, $records, $insert = TRUE, $update_on_match = RESPONSYS_REPLACE_ALL, $match_column_1 = 'EMAIL_ADDRESS_', $match_column_2 = '', $match_column_3 = '') {

    if (empty($match_column_1)) {
      throw new ResponsysException('There must be at least 1 colmun to match on');
    }

    $args = new stdClass();
    $args->list->folderName = $folder;
    $args->list->objectName = $list;

    $args->recordData = $this->convertToResponsysList($records);

    $merge_rules = array(
      'insertOnNoMatch' => $insert,
      'updateOnMatch' => $update_on_match,
    );

    if (!empty($match_column_1)) {
      $merge_rules['matchColumnName1'] = trim($match_column_1);
    }

    if (!empty($match_column_2)) {
      $merge_rules['matchColumnName2'] = trim($match_column_2);
    }

    if (!empty($match_column_3)) {
      $merge_rules['matchColumnName3'] = trim($match_column_3);
    }

    $args->mergeRule = $merge_rules;

    $response = $this->callMethod('mergeListMembers', $args);

    if (is_object($response) && !empty($response->errorMessage)) {
      throw new ResponsysException($response->errorMessage);
    }

    return $response;
  }

  /**
   * Use the triggerCampaignMessage call to send email messages to recipients.
   *
   * @param string $folder
   *   The folder the campaign resides in.
   * @param string $list
   *   The list which all reipients are a member.
   * @param string $campaign_name
   *   The name of the campaign document.
   * @param array $ids
   *   An array of receipient identifiers. The type
   *   of the ID is defined by the id_type parameter
   *   below.
   * @param string $id_type
   *   (optional) The type of the ids, either:
   *    - 'EMAIL_ADDRESS' (default)
   *    - 'RIID'
   *
   * @TODO deal with receiving 200 recipients.
   */
  public function triggerCampaignMessage($folder, $list, $campaign_name, array $ids, $id_type = 'EMAIL_ADDRESS') {

    $recipients = array();

    foreach ($ids as $id) {
      $recipient = new stdClass();
      $recipient->listName->folderName = $folder;
      $recipient->listName->objectName = $list;
      switch ($id_type) {
        case 'RIID':
          $recipient->recipientId = $id;
          break;

        case 'EMAIL_ADDRESS':
          $recipient->emailAddress = $id;
      }
      $recipient_data = new stdClass();
      $recipient_data->recipient = $recipient;
      $recipient_data->optionalData = NULL;
      $recipients[] = $recipient_data;
    }

    $args = new stdClass();
    $args->campaign->folderName = $folder;
    $args->campaign->objectName = $campaign_name;
    $args->recipientData = $recipients;

    $response = $this->callMethod('triggerCampaignMessage', $args);
  }

  /**
   * Use the triggerCustomEvent call to trigger a Custom Event for a recipient.
   */
  public function triggerCustomEvent($folder, $list, $event_id, $email) {
    $recipient = new stdClass();
    $recipient->listName->folderName = $folder;
    $recipient->listName->objectName = $list;
    $recipient->emailAddress = $email;

    $recipient_data = new stdClass();
    $recipient_data->recipient = $recipient;
    $recipient_data->optionalData = NULL;

    $args = new stdClass();
    $args->customEvent = new stdClass();
    $args->customEvent->eventId = $event_id;
    $args->customEvent->recipients[] = $recipient;
    $args->recipientData = array(
      $recipient_data,
    );

    return $this->callMethod('triggerCustomEvent', $args);
  }

  /**
   * Use the createFolder call to create a new empty folder on Responsys.
   *
   * @param string $folder
   *   The name of the folder to create
   *
   * @return bool
   *   TRUE on success
   */
  public function createFolder($folder) {
    $args = new stdClass();
    $args->folderName = $folder;
    return $this->callMethod('createFolder', $args);
  }

  /**
   * Delete a folder on Interact and all its contents.
   *
   * @param string $folder
   *   The name of the folder to create
   *
   * @return bool
   *   TRUE on success
   */
  public function deleteFolder($folder) {
    $args = new stdClass();
    $args->folderName = $folder;
    return $this->callMethod('deleteFolder', $args);
  }

  /**
   * List all top level folders.
   *
   * @return array
   *   An array of folders.
   */
  public function listFolders() {
    $folders = array();
    $results = $this->callMethod('listFolders');

    foreach ($results as $result) {
      if ($result->name[0] != '!') {
        // Include all folder names that do not
        // start with an exclamation mark.
        $folders[] = $result->name;
      }
    }

    return $folders;
  }

  /**
   * Use the createDocument call to create new documents in Responsys.
   *
   * @param string $folder
   *   The name of the folder the document is inside.
   * @param string $document_name
   *   The name of the document to delete.
   * @param string $document_content
   *   The contents of the new document.
   * @param string $enc_type
   *   Character encoding of the document. Default is UTF_8
   *
   * @return bool
   *   TRUE on success and FALSE on failure. Check getLastError()
   *   for details of fails.
   */
  public function createDocument($folder, $document_name, $document_content, $enc_type = 'UTF_8') {
    $args = new stdClass();
    $args->document->folderName = $folder;
    $args->document->objectName = $document_name;
    $args->content = $document_content;
    $args->characterEncoding = $enc_type;
    return $this->callMethod('createDocument', $args);
  }

  /**
   * Set the content of an existing document.
   *
   * @param string $folder
   *   The name of the folder the document is inside.
   * @param string $document_name
   *   The name of the document to delete.
   * @param string $document_content
   *   The contents of the new document.
   */
  public function setDocumentContent($folder, $document_name, $document_content) {
    $args = new stdClass();
    $args->document->folderName = $folder;
    $args->document->objectName = $document_name;
    $args->content = $document_content;
    return $this->callMethod('setDocumentContent', $args);
  }

  /**
   * Use the deleteDocument call to delete a document from an Interact account.
   *
   * @param string $folder
   *   The name of the folder the document is inside.
   * @param string $document_name
   *   The name of the document to delete.
   *
   * @return bool
   *   TRUE on success
   */
  public function deleteDocument($folder, $document_name) {
    $args = new stdClass();
    $args->document->folderName = $folder;
    $args->document->objectName = $document_name;
    return $this->callMethod('deleteDocument', $args);
  }

  /**
   * Use the deleteListMembers call to delete members from a List.
   *
   * @param string $folder
   *   The folder the list to delete from is in
   * @param string $list
   *   The name of the list to delete from.
   * @param array $member_ids
   *   The list of member ids to delete. These ids must all be of the same type
   *   and that type is defined by the $query_column_name below.
   * @param string $query_column_name
   *   The name of the column the ids represent. Can be one of 'RIID',
   *   'CUSTOMER_ID', 'EMAIL_ADDRESS', 'MOBILE_NUMBER'
   *   Default is 'EMAIL_ADDRESS'
   */
  public function deleteListMembers($folder, $list, array $member_ids, $query_column_name = 'EMAIL_ADDRESS') {
    $args = new stdClass();
    $args->list->folderName = $folder;
    $args->list->objectName = $list;
    $args->queryColumn = $query_column_name;
    $args->idsToDelete = $member_ids;
    return $this->callMethod('deleteListMembers', $args);
  }

  /**
   * Create or update a specified document.
   *
   * @param string $folder
   *   The name of the folder the document is inside.
   * @param string $document_name
   *   The name of the document to delete.
   * @param string $document_content
   *   The contents of the new document.
   * @param string $enc_type
   *   Character encoding of the document. Default is UTF_8
   * @param bool $allow_recurse
   *   Internal use only.
   *
   * @return bool
   *   TRUE if successful
   */
  public function saveDocument($folder, $document_name, $document_content, $enc_type = 'UTF_8', $allow_recurse = TRUE) {
    // First try to create the document.
    $result = $this->createDocument($folder, $document_name, $document_content, $enc_type);

    if (!$result) {
      if ($this->getLastErrorCode() == 'FOLDER_NOT_FOUND') {
        // If it has failed due to no folder, make it
        // and try again.
        if ($allow_recurse && $this->createFolder($folder)) {
          return $this->saveDocument($folder, $document_name, $document_content, $enc_type, FALSE);
        }

        // Problem creating the folder.
        return FALSE;
      }
      elseif ($this->getLastErrorCode() == 'DOCUMENT_ALREADY_EXISTS') {

        // If it failed because there is already a doc then update instead.
        $result = $this->setDocumentContent($folder, $document_name, $document_content);

      }
    }

    return $result === TRUE;
  }

  /**
   * Call a method on the SoapClient.
   *
   * @return mixed
   *   An array of result objects or a single
   *   result. Depends on the method.
   */
  public function callMethod($method_name) {

    try {

      $parameters = func_get_args();
      array_shift($parameters);

      $result = call_user_func_array(array($this->client, $method_name), $parameters);

      if (property_exists($result, 'result')) {
        return $result->result;
      }
      else {
        // For methods such as mergeListMembers just return
        // the whole response as it is not contained in a
        // neat little reponse package.
        return $result;
      }

    }
    catch (SoapFault $e) {
      $type = $e->getMessage();
      $detail = empty($e->detail) ? '' : $e->detail->$type->exceptionMessage;
      $message = "{$type}: {$detail}";

      $this->lastError = $message;
      $this->lastErrorCode = empty($e->detail->$type->exceptionCode) ? $e->faultcode : $e->detail->$type->exceptionCode;

      throw new ResponsysException($message);
    }

    return array();
  }

  /**
   * Get the last error reported.
   *
   * @return string
   *   Last error message or an empty string.
   */
  public function getLastError() {
    return $this->lastError;
  }

  /**
   * Get the last error code reported.
   *
   * @return string
   *   Last error code or an empty string.
   */
  public function getLastErrorCode() {
    return $this->lastErrorCode;
  }

  /**
   * Converts the return packet from responsys into an understandable array.
   *
   * @param array|string $field_names
   *   An array of field names
   *   e.g. array(0 => 'RIID_', 1 => 'EMAIL_ADDRESS_')
   *   In the case of just one column being requested,
   *   this is a string withe column name in it.
   * @param array $records
   *   An array of returned record objects. Within each
   *   element is a subarray with the key 'fieldValues'
   *   This array has the actual data for the record
   *   e.g.
   *   array(0 => array('fieldValues' => array(0 => 1, 1 => 'joe@example.com')))
   *
   * @return array
   *   A sane array of data. e.g.
   *   array(0 => array('RIID_' => 1, 'EMAIL_ADDRESS_' => 'joe@example.com'))
   */
  public function convertFromResponsysList($field_names, $records) {
    $results = array();

    // $records will be an object in the case of one record returned
    // or an array if multiple records were returned. Standardise
    // on an array.
    $records = is_object($records) ? array($records) : $records;

    if (is_string($field_names)) {
      // Only one column was requested.
      foreach ($records as $record) {
        $results[] = array(
          $field_names => $record->fieldValues,
        );
      }
    }
    else {
      // Multiple columns were returned.
      foreach ($records as $record) {
        $result = array();
        foreach ($record->fieldValues as $key => $field_value) {
          $result[$field_names[$key]] = $field_value;
        }
        $results[] = $result;
      }
    }

    return $results;
  }

  /**
   * Does the opposite of convertFromResponsysList.
   *
   * @return object
   *   A Responsys recordData object from the record list
   */
  public function convertToResponsysList($records) {
    $record_data = new stdClass();
    $record_data->fieldNames = array();
    $record_data->records = array();

    $keys = array();

    foreach ($records as $record) {
      $new_record = array();

      foreach ($record as $field_name => $field_value) {
        if (!array_key_exists($field_name, $keys)) {
          // Set a numeric key for the field.
          $record_data->fieldNames[] = $field_name;
          $keys[$field_name] = array_search($field_name, $record_data->fieldNames);
        }

        // Get the numeric key for the field_name.
        $key = $keys[$field_name];

        // Add the value to the new record at the numeric
        // key position.
        $new_record[$key] = $field_value;
      }

      // Add the new record to the list of records
      // as a record object.
      $record_data->records[] = (object) array('fieldValues' => $new_record);
    }

    return $record_data;
  }

  /**
   * When the object is destroyed, logout from Responsys.
   */
  public function __destruct() {
    $this->logout();
  }

}

/**
 * Exceptions generated by the Responsys API.
 */
class ResponsysException extends Exception {}
