<?php
/**
 * @file
 * Wrapper class for Polaris PatronRegistrationCreate API call.
 * based on the original pac_polaris.inc file written by Isovera for Richland Library
 * adapted by Mark Pruitt: mspruitt@windstream.net
 */
if (!defined('POLARIS_API_ACCESS_ID')) {
  define('POLARIS_API_ACCESS_ID', variable_get('polaris_auto_register_access_id'));
}
if (!defined('POLARIS_API_ACCESS_KEY')) {
  define('POLARIS_API_ACCESS_KEY', variable_get('polaris_auto_register_access_key'));
}
if (!empty($conf['POLARIS_API_HOST'])) {
  $polaris_api_host = $conf['POLARIS_API_HOST'] . '/polaris/view.aspx';
}
else {
  $polaris_api_host = variable_get('polaris_auto_register_polaris_api_host');
}
if (!defined('POLARIS_API_HOST')) {
  define('POLARIS_API_HOST', $polaris_api_host);
}
if (!defined('POLARIS_API_STAFF_DOMAIN')) {
  define('POLARIS_API_STAFF_DOMAIN', variable_get('polaris_auto_register_polaris_api_staff_domain'));
}
if (!defined('POLARIS_API_STAFF_ID')) {
  define('POLARIS_API_STAFF_ID', variable_get('polaris_auto_register_polaris_api_staff_id'));
}
if (!defined('POLARIS_API_STAFF_USERNAME')) {
  define('POLARIS_API_STAFF_USERNAME', variable_get('polaris_auto_register_polaris_api_staff_username'));
}
if (!defined('POLARIS_API_STAFF_PASSWORD')) {
  define('POLARIS_API_STAFF_PASSWORD', variable_get('polaris_auto_register_polaris_api_staff_password'));
}

class PolarisAPIAutoRegister {

  // @todo These ought to be settings.
  protected static $accessID = POLARIS_API_ACCESS_ID; // Polaris Access ID
  protected static $accessKey = POLARIS_API_ACCESS_KEY; // Polaris Access Key (Test Server RLTRAIN)
  protected static $scheme = 'https';
  protected static $server = POLARIS_API_HOST;
  protected static $baseUrlPublic = '/PAPIService/REST/public/v1/1033/100/1';
  protected static $baseUrlProtected = '/PAPIService/REST/protected/v1/1033/100/1';
  protected static $staffPolarisId = POLARIS_API_STAFF_ID;
  protected static $staffDomain = POLARIS_API_STAFF_DOMAIN;
  protected static $staffUsername = POLARIS_API_STAFF_USERNAME;
  protected static $staffPassword = POLARIS_API_STAFF_PASSWORD;

  /**
   * Build polaris signature for auth header.
   *
   * @param string $http_method
   * @param string $url
   * @param string $date
   * @param string $pass
   * @param string $access_secret
   * @return string
   */
  private function buildSignature($http_method, $url, $date, $pass = '', $access_secret = '') {
    $signature = $http_method . $url . $date . $pass . $access_secret;
    return base64_encode(hash_hmac('sha1', $signature, self::$accessKey, TRUE));
  }

  /**
   * Call API with GET.
   *
   * @param $endpoint
   * @param string $pass
   * @param bool|string $raw_json
   * @param bool $protected
   * @param bool $as_staff
   * @internal param string $api_call
   * @return mixed
   */
  protected function getCallAPI($endpoint, $pass = '', $raw_json = FALSE, $protected = FALSE, $as_staff = FALSE) {
    return self::callAPI($endpoint, 'GET', $pass, NULL, $raw_json, $protected, $as_staff);
  }

  /**
   * Get date formated to RFC-1123.
   *
   * @return string
   */
  private function getPolarisDate() {
    return gmdate('r');
  }

  /**
   * Call API with POST.
   *
   * @param $endpoint
   * @param mixed $fields
   * @param string $pass
   * @param bool|string $raw_json
   * @param bool $protected
   * @param bool $as_staff
   * @internal param string $api_call
   * @internal param string $type
   * @return mixed
   */
  protected function postCallAPI($endpoint, $fields, $pass = '', $raw_json = FALSE, $protected = FALSE, $as_staff = FALSE, $xml = FALSE) {
    return self::callAPI($endpoint, 'POST', $pass, $fields, $raw_json, $protected, $as_staff, $xml);
  }

  /**
   * Call the Polaris API with the specified parameters.
   *
   * @param string $endpoint
   *   The method endpoint.
   * @param string $method
   *   The HTTP method: GET, POST, PUT, etx.
   * @param string $pass
   *   The patron password.
   * @param object|string|array $fields
   *   Pass to cUrl as POSTFIELDS. Objects converted to JSON strings.
   * @param bool $raw_json
   *   Return a json object (FALSE) or string (TRUE).
   * @param bool $protected
   *   Execute method as part of the "protected" API.
   * @param bool $as_staff
   *   Executee public methods as a staff member in lieu of patron password.
   *
   * @throws Exception
   * @return mixed
   *   JSON object as stdClass or string.
   */
  protected function callAPI($endpoint, $method = 'GET', $pass = '', $fields = array(), $raw_json = FALSE, $protected = FALSE, $as_staff = FALSE, $xml = FALSE) {
    // Construct the API URL.
    $url = self::$scheme . '://' . self::$server;
    // The secret is necessary for the signature of protected methods as well as
    // public methods accessed as a staff user.
    $secret = '';
    // Set up the request parameters for protected vs. public methods.
    $headers = array();
    if ($protected) {
      // Use the protected API URL.
      $url .= self::$baseUrlProtected;
      // Append access token to URL for methods besides 'authenticator'.
      if (!(strstr($endpoint, 'authenticator'))) {
        $authentication = self::getStaffAuthentication();
        $url .= '/' . $authentication->AccessToken;
        $secret = $authentication->AccessSecret;
      }
    }
    else {
      // Use the public API URL.
      $url .= self::$baseUrlPublic;
      // Add access token to the header if accessing as a staff member.
      if ($as_staff) {
        $authentication = self::getStaffAuthentication();
        if (is_object($authentication)) {
          $headers[] = 'X-PAPI-AccessToken: ' . $authentication->AccessToken;
          $secret = $authentication->AccessSecret;
        }
        else {
          $headers[] = 'X-PAPI-AccessToken: ' . '';
          $secret = '';
        }
      }
    }
    // Add the endpoint to the URL.
    $url .= $endpoint;

    // Get the Polaris-formatted date.
    $date = self::getPolarisDate();
    // Generate the API signature.
    $signature = self::buildSignature($method, $url, $date, $pass, $secret);
    // Add default request headers.
    if ($xml != TRUE) {
      $headers = array_merge(array(
        'PolarisDate: ' . $date,
        'Authorization: PWS ' . self::$accessID . ':' . $signature,
        'Content-Type: application/json',
        'Accept: application/json',
      ), $headers);
      // If $fields is an object, convert it to a JSON string.
      if (is_object($fields)) {
        $fields = json_encode($fields);
      }
    }
    else {
      $headers = array_merge(array(
        'PolarisDate: ' . $date,
        'Authorization: PWS ' . self::$accessID . ':' . $signature,
        'Content-Type: text/xml',
        'Accept: text/xml',
      ), $headers);
      // If $fields is an object, convert it to an XML string.
      if (is_object($fields)) {
        $fields = self::xml_encode($fields);
      }
    }

    // Set up CURL option by HTTP $method.
    $curl_opts = array();
    switch ($method) {
      case 'GET':
      case 'DELETE':
        $curl_opts[CURLOPT_HTTPGET] = 1;
        break;

      case 'POST':
        $curl_opts[CURLOPT_POST] = 1;
        $curl_opts[CURLOPT_POSTFIELDS] = $fields;
        break;

      case 'PUT':
        $curl_opts[CURLOPT_CUSTOMREQUEST] = 'PUT';
        $curl_opts[CURLOPT_POSTFIELDS] = $fields;
        break;
    }
    // Set the cURL URL.
    $curl_opts[CURLOPT_URL] = $url;
    // Tell cURL to return the response instead of "print"'ing it out.
    $curl_opts[CURLOPT_RETURNTRANSFER] = 1;
    // Add custom HTTP headers to the request.
    $curl_opts[CURLOPT_HTTPHEADER] = $headers;
    // Ask for the HTTP response headers in the result.
    $curl_opts[CURLOPT_HEADER] = 1;

    // Initialize cURL connection object.
    $ch = curl_init();
    // Add options to the cURL request.
    curl_setopt_array($ch, $curl_opts);
    // Execute the external HTTP request and gather the response.
    $response = curl_exec($ch);
    // Get the length of the response headers so they can be parsed.
    $header_size = curl_getinfo($ch, CURLINFO_HEADER_SIZE);
    // Close the cURL request.
    curl_close($ch);
    // Populate the raw headers.
    $return_headers_raw = explode("\r\n", substr($response, 0, $header_size));
    // Populate the response body.
    $body = substr($response, $header_size);
    // Pull out the HTTP status code and message.
    list(, $code, $status_message) = explode(' ', trim(array_shift($return_headers_raw)), 3);
    // Parse the rest of the headers into an associative array.
    // Shamelessly stolen from drupal_http_request().
    // @see drupal_http_request()
    $return_headers = array();

    while ($line = trim(array_shift($return_headers_raw))) {
      list($name, $value) = explode(':', $line, 2);
      $name = strtolower($name);
      if (isset($return_headers[$name]) && $name == 'set-cookie') {
        // RFC 2109: the Set-Cookie response header comprises the token Set-
        // Cookie:, followed by a comma-separated list of one or more cookies.
        $return_headers[$name] .= ',' . trim($value);
      }
      else {
        $return_headers[$name] = trim($value);
      }
    }

    // Create a stdClass object from the response body.
    if ($xml != TRUE) {
      $json_body = json_decode($body);
    }
    else {
      $data = new SimpleXMLElement($body);
      $json_body = (object) drupal_json_decode(drupal_json_encode($data)); // Parse the XML into an object instead.
    }
    // If HTTP response is not 200 OK throw an error.
    if ($code != '200') {
      throw new Exception('Polaris HTTP Error: ' . $method . ' ' . $endpoint . ': ' . $status_message . "\r\n" . $body, intval($code));
    }
    // If Polaris API "ErrorCode" is less than zero throw an error.
    if ($json_body->PAPIErrorCode < 0 && ($json_body->PAPIErrorCode == -1 && $json_body->ErrorMessage != '')) {
      throw new Exception('Polaris API Error: ' . $method . ' ' . $endpoint . ': ' . $json_body->ErrorMessage . "\r\n" . $body, intval($json_body->PAPIErrorCode));
    }
    // Return either the raw or decoded json.
    return $raw_json ? $body : $json_body;
  }

  /**
   * Authenticate a staff user.
   */
  public static function authenticateStaff() {
    $endpoint = '/authenticator/staff';
    $data = new stdClass();
    $data->Domain = self::$staffDomain;
    $data->Username = self::$staffUsername;
    $data->Password = self::$staffPassword;
    try {
      return self::postCallAPI($endpoint, $data, NULL, NULL, TRUE);
    }
    catch (Exception $e) {
      watchdog('PolarisAPI', 'Error while executing authenticateStaff: @exception',
        array('@exception' => $e->getMessage()),
        WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Retrieve the Staff account token/secret data from the cache or the API.
   */
  public static function getStaffAuthentication($reset = FALSE) {
    static $authentication = NULL;
    $cid = 'rcplpolaris:staffauthentication';
    if (!$reset) {
      if (empty($authentication)) {
        // If static cache is empty, retrieve from Drupal cache.
        $cached = cache_get($cid, 'cache');
        if ($cached && time() < $cached->expire) {
          $authentication = $cached->data;
        }
        else {
          // Retrieve new authentication data from the API.
          $authentication = self::authenticateStaff();
          if (!empty($authentication)) {
            // Parse the expiration date into unix micro-timestamp.
            //$authentication->AuthExpMicroTime = str_replace(array('/Date(', '-0500)/'), '', $authentication->AuthExpDate);
            // Save the authentication object in the cache.
            cache_set($cid, $authentication, 'cache', time() + 86400); // Cache the token for 24 hours.
          }
        }
      }
    }

    return $authentication;
  }

  /**
   * Create Patron Registration
   *
   *
   * @return array
   *   Returns an array with keys of status and message
   */

  public static function patronRegistrationCreate($logon_branch_id, $logon_user_id, $logon_workstation_id, $patron_branch_id, $postal_code, $zip_plus_four, $city, $state, $county, $country_id, $street_one, $street_two, $name_first, $name_last, $name_middle, $user1, $user2, $user3, $user4, $user5, $gender, $dob, $phone_voice1, $phone_voice2, $email_address, $language_id, $delivery_option_id, $user_name, $password, $barcode) {

    $endpoint = '/patron';
    $data = new stdClass();
    $data->LogonBranchID = $logon_branch_id;
    $data->LogonUserID = $logon_user_id;
    $data->LogonWorkstationID = $logon_workstation_id;
    $data->PatronBranchID = $patron_branch_id;
    $data->PostalCode = $postal_code;
    $data->ZipPlusFour = $zip_plus_four;
    $data->City = $city;
    $data->State = $state;
    $data->County = $county;
    $data->CountryID = $country_id;
    $data->StreetOne = $street_one;
    $data->StreetTwo = $street_two;
    $data->NameFirst = $name_first;
    $data->NameLast = $name_last;
    $data->NameMiddle = $name_middle;
    $data->User1 = $user1;
    $data->User2 = $user2;
    $data->User3 = $user3;
    $data->User4 = $user4;
    $data->User5 = $user5;
    $data->Gender = $gender;
    $data->Birthdate = $dob;
    $data->PhoneVoice1 = $phone_voice1;
    $data->PhoneVoice2 = $phone_voice2;
    $data->EmailAddress = $email_address;
    $data->LanguageID = $language_id;
    $data->DeliveryOptionID = $delivery_option_id;
    $data->UserName = $user_name;
    $data->Password = $password;
    $data->Password2 = $password;
    $data->Barcode = $barcode;

    try {
      $results = self::postCallAPI($endpoint, $data, NULL, FALSE, FALSE, TRUE);
      switch ($results->StatusType) {
        case 1:
          //error
          return array('status' => FALSE, 'message' => $results->Message);
          break;
        case 0: // this was 2; changed it to 0 to account for success msp
          //answer
          return array('status' => TRUE, 'message' => $results->Message, 'response' => $results);
          break;
        case 3:
          //conditional
          //call hold request reply, which recursively replies until an answer or error is returned
          return self::holdRequestReply($results->RequestGUID, $location_code, $results->StatusValue, $results->TxnQualifier,
          $results->TxnGroupQualifer);
          break;
      }
    }
    catch (Exception $e) {
      watchdog('PolarisAPI', 'There was an issue executing patronRegistrationCreate: @exception',
                array('@exception' => $e->getMessage()),
                WATCHDOG_ERROR);
      return array('status' => FALSE, 'message' => 'An error has occured');
    }
  }

  /**
   * Search the Polaris API for a Patron.
   */
  public static function patronSearchEmail($email) {
    $endpoint = '/search/patrons/Boolean?q=EM=' . $email;
    // values for getCallAPI:
    // $endpoint, $pass = '', $raw_json = FALSE, $protected = FALSE, $as_staff = FALSE
    return self::getCallAPI($endpoint, NULL, FALSE, TRUE);
  }
  
  public static function patronSearchBirthDateFirstNameAndPhone($dob, $fname, $phone) {
    // Note: The API returns an error whenever we try to use hyphens in the patron search, so avoid using those and try our best to find minimal matches.
    $phone_last_four = substr($phone, -4);
    $endpoint = '/search/patrons/Boolean?q=BD=*' . $dob . '*+AND+PATNF=' . $fname . '*+AND+PHONE=*' . $phone_last_four;
    return self::getCallAPI($endpoint, NULL, FALSE, TRUE);
  }

  public static function patronSearchPhone($phone) {
    // Note: The API returns an error whenever we try to use hyphens in the patron search so let's put double-quotes around the search string and then encode it.
    $endpoint = '/search/patrons/Boolean?q=PHONE=' . urlencode('"' . $phone . '"');
    return self::getCallAPI($endpoint, NULL, FALSE, TRUE);
  }

  /**
   * Get basic info about patron based on their barcode
   */
  public static function patronBasicDataGet($barcode) {
    $endpoint = '/patron/' . $barcode . '/basicdata';
    return self::getCallAPI($endpoint, NULL, NULL, FALSE, TRUE, TRUE);
  }

  /**
   * A function to help with holds. Need to encode XML like JSON.
   * From: http://tinyurl.com/n8cjynm
   */
  private function xml_encode($obj) {
    $xml = new XmlWriter();
    $xml->openMemory();
    $xml->startDocument('1.0');
    $xml->setIndent(true);
    if (property_exists($obj, 'TxnGroupQualifier')) { // If it's a reply to a conditional question from Polaris, do this wrapper.
      $xml->startElement('HoldRequestReplyData');
    }
    else { // Otherwise we're just making a hold request to start with. Do this wrapper.
      $xml->startElement('HoldRequestCreateData');
    }

    foreach ($obj as $key => $value) {
      if ($key == 'ActivationDate') {
        $value = str_replace(')/', '', str_replace('/Date(', '', $value));
      }
      $xml->writeElement($key, $value);
    }
    $xml->endElement();
    return $xml->outputMemory(TRUE);
  }

}
