<?php
/**
 * @file
 * Functions related to retrieving and manipulating data from the EVE API.
 */

/**
 * Function to retrieve the XML.
 *
 * @param string $type
 *   Indicate what URL to use to retrieve the XML.
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return mixed
 *   Returns a valid XML object or ERROR information.
 */
function eve_api_get_xml($type, $query = array()) {
  $url = 'https://api.eveonline.com';

  switch ($type) {
    case 'ContactList':
      $url .= '/corp/ContactList.xml.aspx';
      break;

    case 'AllianceList':
      $url .= '/eve/AllianceList.xml.aspx';
      break;

    case 'CorporationSheet':
      $url .= '/corp/CorporationSheet.xml.aspx';
      break;

    case 'CharacterSheet':
      $url .= '/char/CharacterSheet.xml.aspx';
      break;

    case 'FacWarStats':
      $url .= '/char/FacWarStats.xml.aspx';
      break;

    case 'CharacterID':
      $url .= '/eve/CharacterID.xml.aspx';
      break;

    case 'CharacterName':
      $url .= '/eve/CharacterName.xml.aspx';
      break;

    case 'CharacterInfo':
      $url .= '/eve/CharacterInfo.xml.aspx';
      break;

    case 'callList':
      $url .= '/api/callList.xml.aspx';
      break;

    case 'SkillTree':
      $url .= '/eve/SkillTree.xml.aspx';
      break;

    case 'SkillInTraining':
      $url .= '/char/SkillInTraining.xml.aspx';
      break;

    case 'WalletJournal':
      $url .= '/char/WalletJournal.xml.aspx';
      break;

    case 'Contracts':
      $url .= '/char/Contracts.xml.aspx';
      break;

    case 'MailMessages':
      $url .= '/char/MailMessages.xml.aspx';
      break;

    case 'MailBodies':
      $url .= '/char/MailBodies.xml.aspx';
      break;

    case 'Notifications':
      $url .= '/char/Notifications.xml.aspx';
      break;

    case 'NotificationTexts':
      $url .= '/char/NotificationTexts.xml.aspx';
      break;

    case 'ErrorList':
      $url .= '/eve/ErrorList.xml.aspx';
      break;

    default:
    case 'APIKeyInfo':
      $url .= '/account/APIKeyInfo.xml.aspx';
      break;
  }

  $cache_name = 'xml' . $type . str_replace('=', '', str_replace('&', '', drupal_http_build_query($query)));
  $html = '';
  $response = array();

  if ($cached = cache_get($cache_name, 'cache')) {
    $html = $cached->data;
    $response['errorCode'] = $html->code;

    if (isset($html->error)) {
      $response['errorText'] = $html->error;
    }
    else {
      $response['errorText'] = 'OK';
    }
  }
variable_set('eve_api_cron_error_count_limit', 10);
  if (empty($html) || $type == 'SkillTree') {
    if (variable_get('eve_api_cron_error_count', 0) >= variable_get('eve_api_cron_error_count_limit', 10)) {
      variable_set('eve_api_cron_error_time', time() + variable_get('eve_api_cron_error_time_limit', 10 * 60));
      variable_set('eve_api_cron_error_count', 0);
      watchdog('eve_api', 'EVE API has failed 10 times to pull cron data, API cron resumes at ' . date('Y-m-d H:i:s', variable_get('eve_api_cron_error_time', 0)), NULL, WATCHDOG_EMERGENCY);
      return array(
        'errorCode' => 2,
        'errorText' => 'EVE API has failed 10 times to pull cron data, API cron resumes at ' . date('Y-m-d H:i:s', variable_get('eve_api_cron_error_time', 0)),
      );
    }
    elseif (variable_get('eve_api_cron_error_time', 0) >= time()) {
      return array(
        'errorCode' => 2,
        'errorText' => 'EVE API has failed 10 times to pull cron data, API cron resumes at ' . date('Y-m-d H:i:s', variable_get('eve_api_cron_error_time', 0)),
      );
    }

    $has_curl = function_exists('curl_init');
    $open_basedir = ini_get('open_basedir');

    if (!$has_curl || $open_basedir) {
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query($query),
        'timeout' => 45,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Accept-Encoding' => 'gzip',
        ),
      );

      $html = drupal_http_request($url, $options);
    }
    else {
      $options = array(
        'method' => 'POST',
        'data' => $query,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Accept-Encoding' => 'gzip',
        ),
      );

      $html = eve_api_curl_http_request($url, $options);
    }

    if (isset($html->error) && !empty($html->error)) {
      $response['errorText'] = $html->error;

      if (variable_get('eve_api_debug', FALSE)) {
        watchdog('eve_api', 'EVE API returned an error with the HTTP request. (' . $type . '?' . drupal_http_build_query($query) . ') Error: ' . $html->error . ' Code: ' . $html->code, NULL, WATCHDOG_ERROR);
      }
    }
    else {
      $response['errorText'] = 'OK';
    }

    $response['errorCode'] = $html->code;
  }

  // Fix for HTTP_REQUEST_TIMEOUT returning as -1
  if ($response['errorCode'] <= 0) {
    return array(
      'errorCode' => 3,
      'errorText' => $response['errorText'],
    );
  }

  if (FALSE !== strpos($html->data, '<!DOCTYPE html')) {
    variable_set('eve_api_cron_error_count', 1 + variable_get('eve_api_cron_error_count', 0));
    watchdog('eve_api', 'EVE API returned an error with the HTTP request. (' . $type . '?' . drupal_http_build_query($query) . ') Error: ' . $html->error . ' Code: ' . $html->code, NULL, WATCHDOG_ERROR);
    $response['errorCode'] = 4;
  }
  else {
    try {
      $response['errorCode'] = 0;
      $response['xml'] = @new SimpleXMLElement($html->data);
      variable_set('eve_api_cron_error_count', 0);
    }
    catch (Exception $e) {
      variable_set('eve_api_cron_error_count', 1 + variable_get('eve_api_cron_error_count', 0));
      watchdog('eve_api', 'EVE API returned an error with the HTTP request. (' . $type . '?' . drupal_http_build_query($query) . ') Error: ' . $e->getMessage(), NULL, WATCHDOG_ERROR);
      $response['errorCode'] = 5;
      $response['xml'] = NULL;
    }
  }

  if (isset($response['xml']->error)) {
    $error = $response['xml']->error->attributes();
    $response['errorCode'] = $error->code;
    $response['errorText'] = $response['xml']->error;

    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'EVE API returned an error with the API data. (' . $type . '?' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_NOTICE);
    }

    if (eve_api_error_code($response['errorCode'])) {
      variable_set('eve_api_cron_error_count', 1 + variable_get('eve_api_cron_error_count', 0));
    }
    else {
      variable_set('eve_api_cron_error_count', 0);
    }
  }
  elseif (isset($response['xml']->cachedUntil)) {
    variable_set('eve_api_cron_error_count', 0);
  }

  if (isset($response['xml']->cachedUntil)) {
    cache_set($cache_name, $html, 'cache', strtotime($response['xml']->cachedUntil));
  }

  unset($html);

  return $response;
}

/**
 * Performs an HTTP request.
 *
 * This is a flexible and powerful HTTP client implementation. Correctly
 * handles GET, POST, PUT or any other HTTP requests. Handles redirects.
 * Based on "cURL HTTP Request v1.5" by Vincenzo and minorOffense
 * Based on "Yapeal (http://code.google.com/p/yapeal/)" by Kevin Burkholder
 * Modified for use with "Omni EVE API"
 *
 *
 * @param string $url
 *   A string containing a fully qualified URI.
 * @param array $options
 *   (optional) An array composed of various options.
 *
 * @return object
 *   An object that can have one or more of the following components:
 *   - request: A string containing the request body that was sent.
 *   - code: An integer containing the response status code, or the error code
 *     if an error occurred.
 *   - protocol: The response protocol (e.g. HTTP/1.1 or HTTP/1.0).
 *   - status_message: The status message from the response, if a response was
 *     received.
 *   - redirect_code: If redirected, an integer containing the initial response
 *     status code.
 *   - redirect_url: If redirected, a string containing the URL of the redirect
 *     target.
 *   - error: If an error occurred, the error message. Otherwise not set.
 *   - errno: If an error occurred, a cURL error number greater than 0.
 *     Otherwise set to 0.
 *   - headers: An array containing the response headers as name/value pairs.
 *     HTTP header names are case-insensitive (RFC 2616, section 4.2), so for
 *     easy access the array keys are returned in lower case.
 *   - data: A string containing the response body that was received.
 */
function eve_api_curl_http_request($url, array $options = array()) {
  $result = new stdClass();

  // Parse the URL and make sure we can handle the schema.
  $uri = @parse_url($url);

  if ($uri == FALSE) {
    $result->error = 'unable to parse URL';
    $result->code = -1001;
    return $result;
  }

  if (!isset($uri['scheme'])) {
    $result->error = 'missing schema';
    $result->code = -1002;
    return $result;
  }

  timer_start(__FUNCTION__);

  // Merge the default options.
  $options = $options + array(
    'data' => array(),
    'headers' => array(),
    'method' => 'POST',
    'timeout' => 45,
    'http_proxy' => variable_get('http_proxy'),
    'https_proxy' => variable_get('https_proxy'),
  );

  // Select the right proxy for the right protocol.
  $proxy = ('https' == $uri['scheme']) ? $options['https_proxy'] : $options['http_proxy'];

  // Nullify the proxy if the host to send the request to is part of the proxy's
  // exceptions.
  if ((!empty($proxy['exceptions'])) && (array_key_exists($uri['host'], $proxy['exceptions']))) {
    $proxy = NULL;
  }

  $curl = curl_version();
  $user_agent = 'Drupal (+http://drupal.org/)';
  $user_agent .= ' Omni EVE API/' . eve_api_version();
  $user_agent .= ' (' . PHP_OS . ' ' . php_uname('m') . ')';
  $user_agent .= ' libcurl/' . $curl['version'];
  $user_agent = trim($user_agent);

  $curl_opt = array(
    CURLOPT_HEADER => TRUE,
    CURLINFO_HEADER_OUT => TRUE,
    CURLOPT_TIMEOUT => $options['timeout'],
    CURLOPT_FOLLOWLOCATION => TRUE,
    CURLOPT_RETURNTRANSFER => TRUE,
    CURLOPT_MAXREDIRS => 5,
    CURLOPT_NOBODY => FALSE,
    CURLOPT_FORBID_REUSE => FALSE,
    CURLOPT_LOW_SPEED_LIMIT => 10,
    CURLOPT_LOW_SPEED_TIME => ceil($options['timeout'] / 4),
    CURLOPT_MAXCONNECTS => 5,
    CURLOPT_CONNECTTIMEOUT => ceil($options['timeout'] / 2),
    CURLOPT_REFERER => 'http://danlee.ca/git/',
    CURLOPT_UNRESTRICTED_AUTH => TRUE,
    CURLOPT_ENCODING => 'gzip',
    CURLOPT_BINARYTRANSFER => FALSE,
    CURLOPT_USERAGENT => $user_agent,
    CURLOPT_SSL_VERIFYPEER => TRUE,
    CURLOPT_SSL_VERIFYHOST => 2,
    // CURLOPT_CAINFO => drupal_get_path('module', 'eve_api') . '/eveonline.crt',
    CURLOPT_COOKIEJAR => drupal_get_path('module', 'eve_api') . '/cookies.txt',
  );

  $ssl_version = $curl['ssl_version'];
  $has_nss = (strpos($ssl_version, "NSS") > -1);

  if ($has_nss) {
    $curl_opt += array(CURLOPT_SSL_CIPHER_LIST => 'rsa_aes_128_sha,rsa_aes_256_sha,rsa_3des_sha,rsa_rc4_128_sha,rsa_rc4_128_md5');
  }
  else {
    $curl_opt += array(CURLOPT_SSL_CIPHER_LIST => 'AES128-SHA AES256-SHA DES-CBC3-SHA RC4-SHA RC4-MD5');
  }

  if (!empty($proxy)) {
    $proxy_options = array(
      CURLOPT_PROXY => $proxy['server'],
      CURLOPT_PROXYPORT => $proxy['port'],
    );
    // For the time being let's just support HTTP proxies with basic
    // authentication.
    if (isset($proxy['username']) && isset($proxy['password'])) {
      $proxy_options += array(
        CURLOPT_PROXYUSERPWD => implode(':', array(
          $proxy['username'],
          $proxy['password'],
        )),
        CURLOPT_PROXYTYPE => CURLPROXY_HTTP,
        CURLOPT_PROXYAUTH => CURLAUTH_BASIC,
      );
    }
    $curl_opt += $proxy_options;
  }

  $default_ports = array(
    'http' => 80,
    'feed' => 80,
    'https' => 443,
  );

  if (array_key_exists($uri['scheme'], $default_ports)) {
    if (!isset($uri['port'])) {
      $uri['port'] = $default_ports[$uri['scheme']];
    }
    // RFC 2616: "non-standard ports MUST, default ports MAY be included".
    // We don't add the standard port to prevent from breaking rewrite rules
    // checking the host that do not take into account the port number.
    $options['headers']['Host'] = $uri['host'] . ($uri['port'] != 80 ? ':' . $uri['port'] : '');
  }
  else {
    $result->error = 'invalid schema ' . $uri['scheme'];
    $result->code = -1003;
    return $result;
  }

  // Merge the default headers.
  $options['headers'] += array(
    'User-Agent' => 'Drupal (+http://drupal.org/)',
  );

  // Only add Content-Length if we actually have any content or if it is a POST
  // or PUT request. Some non-standard servers get confused by Content-Length in
  // at least HEAD/GET requests, and Squid always requires Content-Length in
  // POST/PUT requests.
  $content_length = strlen(str_replace('%2C', ',', drupal_http_build_query($options['data'])));
  if ($content_length > 0 || $options['method'] == 'POST' || $options['method'] == 'PUT') {
    $options['headers']['Content-Length'] = $content_length;
  }

  // If the server URL has a user then attempt to use basic authentication.
  if (isset($uri['user'])) {
    $options['headers']['Authorization'] = 'Basic ' . base64_encode($uri['user'] . (isset($uri['pass']) ? ':' . $uri['pass'] : ''));
  }

  // Set all the headers.
  $curl_opt[CURLOPT_HTTPHEADER] = array();
  foreach ($options['headers'] as $name => $value) {
    $curl_opt[CURLOPT_HTTPHEADER][] = $name . ": " . trim($value);
  }

  // Set the request method.
  switch ($options['method']) {
    case 'GET':
      $curl_opt[CURLOPT_HTTPGET] = TRUE;
      break;

    case 'POST':
      $curl_opt[CURLOPT_POST] = TRUE;
      if (!empty($options['data'])) {
        $curl_opt[CURLOPT_POSTFIELDS] = str_replace('%2C', ',', drupal_http_build_query($options['data']));
        $options['headers'] += array(
          'Content-Type' => 'application/x-www-form-urlencoded',
        );
      }
      break;

    case 'PUT':
      $curl_opt[CURLOPT_PUT] = TRUE;
      break;

    default:
      $result->error = 'invalid method ' . $options['method'];
      $result->code = -1004;
      return $result;
  }

  // Make the request.
  $ch = curl_init($url);
  curl_setopt_array($ch, $curl_opt);

  // Full response stored. To be parsed later on and split in protocol, code,
  // status message, and response headers.
  $result->data = trim(curl_exec($ch));
  $result->error = curl_error($ch);
  $result->errno = curl_errno($ch);

  // If there's been an error, do not continue.
  if ($result->error) {
    // Request timed out.
    if (CURLE_OPERATION_TIMEOUTED == $result->errno) {
      $result->code = HTTP_REQUEST_TIMEOUT;
      $result->error = 'request timed out';
      return $result;
    }
    $result->code = $result->errno;
    return $result;
  }

  // The last effective URL should correspond to the Redirect URL.
  $result->redirect_url = curl_getinfo($ch, CURLINFO_EFFECTIVE_URL);

  // Save the request sent into the result object.
  $result->request = curl_getinfo($ch, CURLINFO_HEADER_OUT);

  // Parse response headers from the response body.
  // Be tolerant of malformed HTTP responses that separate header and body with
  // \n\n or \r\r instead of \r\n\r\n.
  list($response, $result->data) = preg_split("/\r\n\r\n|\n\n|\r\r/", $result->data, 2);

  // Sometimes when making an HTTP request via proxy using cURL, you end up with
  // a multiple set of headers:
  // from the web server being the actual target, from the proxy itself, etc.
  // The following 'if' statement is to check for such a situation and make sure
  // we get a proper split between
  // actual response body and actual response headers both coming from the web
  // server.
  while ('HTTP/' == substr($result->data, 0, 5)) {
    list($response, $result->data) = preg_split("/\r\n\r\n|\n\n|\r\r/", $result->data, 2);
  }

  $response = preg_split("/\r\n|\n|\r/", $response);

  // Parse the response status line.
  list($protocol, $code, $status_message) = explode(' ', trim(array_shift($response)), 3);
  $result->protocol = $protocol;
  $result->status_message = $status_message;

  $result->headers = array();

  // Parse the response headers.
  while ($line = trim(array_shift($response))) {
    list($name, $value) = explode(':', $line, 2);
    $name = strtolower($name);
    if (isset($result->headers[$name]) && $name == 'set-cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$name] .= ',' . trim($value);
    }
    else {
      $result->headers[$name] = trim($value);
    }
  }

  $responses = array(
    100 => 'Continue',
    101 => 'Switching Protocols',
    200 => 'OK',
    201 => 'Created',
    202 => 'Accepted',
    203 => 'Non-Authoritative Information',
    204 => 'No Content',
    205 => 'Reset Content',
    206 => 'Partial Content',
    300 => 'Multiple Choices',
    301 => 'Moved Permanently',
    302 => 'Found',
    303 => 'See Other',
    304 => 'Not Modified',
    305 => 'Use Proxy',
    307 => 'Temporary Redirect',
    400 => 'Bad Request',
    401 => 'Unauthorized',
    402 => 'Payment Required',
    403 => 'Forbidden',
    404 => 'Not Found',
    405 => 'Method Not Allowed',
    406 => 'Not Acceptable',
    407 => 'Proxy Authentication Required',
    408 => 'Request Time-out',
    409 => 'Conflict',
    410 => 'Gone',
    411 => 'Length Required',
    412 => 'Precondition Failed',
    413 => 'Request Entity Too Large',
    414 => 'Request-URI Too Large',
    415 => 'Unsupported Media Type',
    416 => 'Requested range not satisfiable',
    417 => 'Expectation Failed',
    500 => 'Internal Server Error',
    501 => 'Not Implemented',
    502 => 'Bad Gateway',
    503 => 'Service Unavailable',
    504 => 'Gateway Time-out',
    505 => 'HTTP Version not supported',
  );
  // RFC 2616 states that all unknown HTTP codes must be treated the same as the
  // base code in their class.
  if (!isset($responses[$code])) {
    $code = floor($code / 100) * 100;
  }
  $result->code = $code;

  switch ($code) {
    case 200:
      // OK.
    case 304:
      // Not modified.
      break;

    case 301:
      // Moved permanently.
    case 302:
      // Moved temporarily.
    case 307:
      // Moved temporarily.
      $location = $result->headers['location'];
      $options['timeout'] -= timer_read(__FUNCTION__) / 1000;
      if ($options['timeout'] <= 0) {
        $result->code = HTTP_REQUEST_TIMEOUT;
        $result->error = 'request timed out';
      }
      elseif ($options['max_redirects']) {
        // Redirect to the new location.
        $options['max_redirects']--;
        $result = eve_api_curl_http_request($location, $options);
        $result->redirect_code = $code;
      }
      if (!isset($result->redirect_url)) {
        $result->redirect_url = $location;
      }
      break;

    default:
      $result->error = $status_message;
  }

  curl_close($ch);
  return $result;
}

/**
 * Function to retrieve individual characters from the database.
 *
 * @param string $character
 *   Unique character name used to retrieve information about the character
 *
 * @return array
 *   A complex array of a single character information.
 */
function eve_api_get_basic_character_info($character) {
  $result = db_query('SELECT c.uid, c.deleted, c.errorCode, c.errorText, c.apiID, c.characterName, c.characterID, c.corporationName, c.corporationID, c.corporationTicker, c.allianceID, c.allianceName, c.allianceTicker
    FROM {eve_api_characters} c
    WHERE c.characterName = :characterName OR c.characterID = :characterID', array(
      ':characterName' => (string) $character,
      ':characterID' => (int) $character,
    ));

  $api_data = array();

  if ($result->rowCount()) {
    foreach ($result->fetchAssoc() as $name => $data) {
      $api_data += array($name => strip_tags(decode_entities($data)));
    }
  }
  else {
    $api_data = FALSE;
  }

  return $api_data;
}

/**
 * Function to retrieve individual characters from the database.
 *
 * @param string $character
 *   Unique character name used to retrieve information about the character
 *
 * @return array
 *   A complex array of a single character information.
 */
function eve_api_get_character_info($character) {
  $result = db_query('SELECT c.accessMask, c.uid, st.skillInTraining, c.characterName, c.characterID
    FROM {eve_api_characters} c
    INNER JOIN {eve_api_skill_in_training} st ON st.characterID = c.characterID
    WHERE c.characterName = :characterName OR c.characterID = :characterID', array(
      ':characterName' => (string) $character,
      ':characterID' => (int) $character,
    ));

  $row = $result->fetchAssoc();

  if (empty($row)) {
    return FALSE;
  }

  $access_mask = $row['accessMask'];
  $is_training = $row['skillInTraining'];
  $uid = $row['uid'];
  $character_id = $row['characterID'];

  $query = db_select('eve_api_characters', 'c');
  $query->join('eve_api_character_sheet', 'cs', 'cs.characterID = c.characterID');
  $query->join('eve_api_character_info', 'ci', 'ci.characterID = c.characterID');
  $query = $query->fields('c', array(
      'uid',
      'deleted',
      'errorCode',
      'errorText',
      'apiID',
      'characterName',
      'characterID',
      'corporationName',
      'corporationID',
      'corporationTicker',
      'allianceID',
      'allianceName',
      'allianceTicker',
      'accessMask',
    ))
    ->fields('cs', array(
      'DoB',
      'race',
      'bloodLine',
      'ancestry',
      'gender',
      'cloneName',
      'balance',
      'intelligence',
      'memory',
      'charisma',
      'perception',
      'willpower',
    ))
    ->fields('ci', array(
      'skillPoints',
      'shipName',
      'shipTypeID',
      'shipTypeName',
      'corporationDate',
      'allianceDate',
      'lastKnownLocation',
      'securityStatus',
      'nextTrainingEnds',
    ));

  if (($access_mask & 131072) == 131072) {
    $query->join('eve_api_skill_in_training', 'st', 'st.characterID = c.characterID');

    if ($is_training != 0) {
      $query->join('eve_api_skill_tree', 'sts', 'sts.typeID = st.trainingTypeID');
      $query->fields('sts', array('typeName'));
    }

    $query->fields('st', array(
        'trainingEndTime',
        'trainingStartTime',
        'trainingStartSP',
        'trainingDestinationSP',
        'trainingToLevel',
        'skillInTraining',
      ));
    $query->addField('st', 'trainingTypeID', 'typeID');
  }

  $result = $query->condition('c.characterID', (int) $character_id, '=')->execute();

  $api_data = array();

  if ($result->rowCount()) {
    foreach ($result->fetchAssoc() as $name => $data) {
      $api_data += array($name => strip_tags(decode_entities($data)));
    }

    $result = db_query('SELECT csae.bonusName, csae.augmentatorValue
      FROM {eve_api_character_sheet_attribute_enhancers} csae
      LEFT JOIN {eve_api_characters} c ON c.characterID = csae.characterID
      WHERE c.characterID = :characterID', array(
        ':characterID' => (int) $character_id,
      ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $api_data += array($row->bonusName => strip_tags(decode_entities($row->augmentatorValue)));
      }
    }

    $result = db_query('SELECT cich.recordID, cich.corporationID, cich.startDate
      FROM {eve_api_character_info_corporation_history} cich
      LEFT JOIN {eve_api_characters} c ON c.characterID = cich.characterID
      WHERE c.characterID = :characterID', array(
        ':characterID' => (int) $character_id,
      ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $history_corp = array();

        foreach ($row as $name => $data) {
          $history_corp += array($name => strip_tags(decode_entities($data)));
        }

        $api_data['history'][] = $history_corp;
      }
    }

    $result = db_query('SELECT deleted, errorCode, errorText, characterName, characterID, characterName, characterID, corporationName, corporationID, corporationTicker, allianceID, allianceName, allianceTicker
      FROM {eve_api_characters}
      WHERE uid = :uid', array(
        ':uid' => (string) $uid,
      ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $characters = array();

        foreach ($row as $name => $data) {
          $characters += array($name => strip_tags(decode_entities($data)));
        }

        $api_data['characters'][] = $characters;
      }
    }

    $result = db_query('SELECT apiID, keyID, vCode, errorCode, errorText
      FROM {eve_api_keys}
      WHERE uid = :uid', array(
        ':uid' => (string) $uid,
      ));

    if ($result->rowCount()) {
      foreach ($result->fetchAll() as $row) {
        $api_keys = array();

        foreach ($row as $name => $data) {
          $api_keys += array($name => strip_tags(decode_entities($data)));
        }

        $api_data['api_keys'][] = $api_keys;
      }
    }
  }

  return $api_data;
}

/**
 * Function to retrieve individual characters skills from the database.
 *
 * @param int $character_id
 *   Unique character id used to retrieve information about the character
 *
 * @return array
 *   A complex array of a single character skills.
 */
function eve_api_get_character_skills($character_id) {
  $result = db_query('SELECT css.level, css.skillpoints, st.typeName, sg.groupName, sg.groupID, st.typeID
    FROM {eve_api_character_sheet_skills} css
    INNER JOIN {eve_api_skill_tree} st ON st.typeID = css.typeID
    INNER JOIN {eve_api_skill_tree_groups} sg ON sg.groupID = st.groupID
    WHERE css.characterID = :characterID ORDER BY sg.groupName, st.typeName ASC', array(
      ':characterID' => (int) $character_id,
    ));

  $api_data = array();

  if ($result->rowCount()) {
    foreach ($result->fetchAll() as $row) {
      $api_data[(string) $row->groupName][] = array(
        'level' => check_plain((int) $row->level),
        'groupID' => check_plain((int) $row->groupID),
        'typeID' => check_plain((int) $row->typeID),
        'typeName' => check_plain((string) $row->typeName),
        'skillpoints' => check_plain((int) $row->skillpoints),
      );
    }
  }

  return $api_data;
}

/**
 * Function to retrieve individual characters API info.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of multiple character information.
 */
function eve_api_get_api_key_info_api($query) {
  $response = eve_api_get_xml('APIKeyInfo', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting API Key Info API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_info = $response['xml']->result->key->attributes();
  $api_data['accessMask'] = (int) $api_info->accessMask;
  $api_data['type'] = (string) $api_info->type;
  $api_data['expires'] = ((string) $api_info->expires) ? TRUE : FALSE;
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->key->rowset->children() as $rows) {
    $row = $rows->attributes();

    $query = array('corporationID' => (int) $row->corporationID);
    $corp_info = eve_api_get_corporation_sheet_api($query);

    if (isset($corp_info['error'])) {
      return array('error' => $corp_info['error']);
    }
    else {
      $api_data['characters'][(int) $row->characterID] = array_merge(array(
        'characterName' => (string) $row->characterName,
        'characterID' => (int) $row->characterID,
        'corporationName' => (string) $row->corporationName,
        'corporationID' => (int) $row->corporationID,
      ), $corp_info);
    }
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters skills in training.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character information.
 */
function eve_api_get_skill_in_training_api($query) {
  $response = eve_api_get_xml('SkillInTraining', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Skill in Training API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->children() as $result) {
    $api_data['info'][(string) $result->getName()] = (string) $result;
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters wallet journal.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character information.
 */
function eve_api_get_wallet_journal_api($query) {
  $response = eve_api_get_xml('WalletJournal', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Wallet Journal API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  $count = 0;

  foreach ($response['xml']->result->rowset->children() as $rows) {
    foreach ($rows->attributes() as $name => $value) {
      $api_data['wallet_journal'][(int) $count][(string) $name] = (string) $value;
    }

    $count++;
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters contracts.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character information.
 */
function eve_api_get_contracts_api($query) {
  $response = eve_api_get_xml('Contracts', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Contracts API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  $count = 0;

  foreach ($response['xml']->result->rowset->children() as $rows) {
    foreach ($rows->attributes() as $name => $value) {
      $api_data['contracts'][(int) $count][(string) $name] = (string) $value;
    }

    $count++;
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters mail headers.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character information.
 */
function eve_api_get_mail_messages_api($query) {
  $response = eve_api_get_xml('MailMessages', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Mail Messages API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->rowset->children() as $rows) {
    $row = $rows->attributes();

    foreach ($rows->attributes() as $name => $value) {
      $api_data['mail'][(int) $row->messageID][(string) $name] = (string) $value;
    }
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters mail bodies.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character information.
 */
function eve_api_get_mail_bodies_api($query) {
  $response = eve_api_get_xml('MailBodies', $query);

  if (!isset($response['xml']->result) && !isset($response['xml']->result->missingMessageIDs)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Mail Bodies API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  if (isset($response['xml']->result->missingMessageIDs)) {
    $api_data['missingMessageIDs'] = (string) $response['xml']->result->missingMessageIDs;
  }

  foreach ($response['xml']->result->rowset->children() as $rows) {
    $row = $rows->attributes();

    $api_data['mail_body'][(int) $row->messageID]['message'] = (string) $rows;

    foreach ($rows->attributes() as $name => $value) {
      $api_data['mail_body'][(int) $row->messageID][(string) $name] = (string) $value;
    }
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters character info.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character information.
 */
function eve_api_get_character_info_api($query) {
  $response = eve_api_get_xml('CharacterInfo', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Character Sheet API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->children() as $results) {
    switch ((string) $results->getName()) {
      case 'rowset':
        foreach ($results->children() as $rows) {
          $row = $rows->attributes();

          $api_data['employmentHistory'][(int) $row->recordID] = array(
            'recordID' => (int) $row->recordID,
            'corporationID' => (int) $row->corporationID,
            'startDate' => (string) $row->startDate,
          );
        }
        break;

      default:
        $api_data['info'][(string) $results->getName()] = (string) $results;
        break;
    }
  }

  return $api_data;
}

/**
 * Function to retrieve a single characters character sheet.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of character sheet information.
 */
function eve_api_get_character_sheet_api($query) {
  $response = eve_api_get_xml('CharacterSheet', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Character Sheet API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->children() as $results) {
    switch ((string) $results->getName()) {
      case 'rowset':
        $rowset = $results->attributes();

        foreach ($results->children() as $rows) {
          $row = $rows->attributes();

          switch ((string) $rowset->name) {
            case 'skills':
              $api_data['skills'][] = array(
                'typeID' => (int) $row->typeID,
                'skillpoints' => (int) $row->skillpoints,
                'level' => (int) $row->level,
                'published' => (int) $row->published,
              );
              break;

            case 'certificates':
              $api_data['certificates'][]['certificateID'] = (int) $row->certificateID;
              break;

            case 'corporationTitles':
              $api_data['corporationTitles'][] = array(
                'titleID' => (int) $row->titleID,
                'titleName' => (string) $row->titleName,
              );
              break;

            default:
              $api_data['roles'][(string) $rowset->name][] = array(
                'roleID' => (int) $row->roleID,
                'roleName' => (string) $row->roleName,
              );
              break;
          }
        }
        break;

      case 'attributeEnhancers':
        foreach ($results->children() as $rows) {
          foreach ($rows->children() as $row) {
            $api_data['attributeEnhancers'][(string) $rows->getName()][(string) $row->getName()] = (string) $row;
          }
        }
        break;

      case 'attributes':
        foreach ($results->children() as $rows) {
          $api_data['info'][(string) $rows->getName()] = (int) $rows;
        }
        break;

      default:
        $api_data['info'][(string) $results->getName()] = (string) $results;
        break;
    }
  }

  return $api_data;
}

/**
 * Function to retrieve skill definitions.
 *
 * @return array
 *   A complex array of skill definitions.
 */
function eve_api_get_skill_tree_api() {
  $response = eve_api_get_xml('SkillTree');

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Skill Tree API Data. Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->rowset->children() as $rowsets) {
    $rowset = $rowsets->attributes();

    $api_data['group'][(int) $rowset->groupID] = array(
      'groupID' => (int) $rowset->groupID,
      'groupName' => (string) $rowset->groupName,
    );

    foreach ($rowsets->rowset->children() as $rows) {
      $row = $rows->attributes();

      $api_data['type'][(int) $row->typeID] = array(
        'typeID' => (int) $row->typeID,
        'typeName' => (string) $row->typeName,
        'groupID' => (int) $row->groupID,
      );
    }
  }

  return $api_data;
}

/**
 * Function to retrieve the alliance tags.
 *
 * @param int $alliance_id
 *   The Unique ID for an alliance.
 *
 * @return string
 *   The Alliance ticker for the $allianceID.
 */
function eve_api_get_alliance_ticker($alliance_id) {
  $result = db_query('SELECT allianceTicker FROM {eve_api_alliance_list_tickers} WHERE allianceID = :id', array(
    ':id' => (int) $alliance_id,
  ));
  $row = $result->fetchAssoc();

  return $row['allianceTicker'];
}

/**
 * Function to retrieve corporation API Info.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A complex array of corporate information.
 */
function eve_api_get_corporation_sheet_api($query) {
  $response = eve_api_get_xml('CorporationSheet', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Corporation Sheet API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array(
    'corporationID' => (int) $response['xml']->result->corporationID,
    'corporationName' => (string) $response['xml']->result->corporationName,
    'corporationTicker' => (string) $response['xml']->result->ticker,
    'allianceID' => (int) $response['xml']->result->allianceID,
    'allianceName' => (string) $response['xml']->result->allianceName,
    'allianceTicker' => (string) eve_api_get_alliance_ticker((int) $response['xml']->result->allianceID),
    'ceoName' => (string) $response['xml']->result->ceoName,
    'ceoID' => (int) $response['xml']->result->ceoID,
  );

  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  return $api_data;
}

/**
 * Function to retrieve Character/Alliance/Corporation ID's from a name.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A simple array containing unique name and ID information.
 */
function eve_api_get_character_id_api($query) {
  $response = eve_api_get_xml('CharacterID', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Character ID API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->rowset->children() as $rows) {
    $row = $rows->attributes();
    $name = strtoupper((string) $row->name);

    $api_data[$name] = array(
      'name' => (string) $row->name,
      'characterID' => (int) $row->characterID,
    );
  }

  return $api_data;
}

/**
 * Function to retrieve character names from character ids.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A simple array containing unique name and ID information.
 */
function eve_api_get_character_name_api($query) {
  $response = eve_api_get_xml('CharacterName', $query);

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Character Name API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];

  foreach ($response['xml']->result->rowset->children() as $rows) {
    $row = $rows->attributes();

    $api_data[(int) $row->characterID] = (string) $row->name;
  }

  return $api_data;
}

/**
 * Function to retrieve updated list of EVE API Error Codes.
 *
 * @param array $query
 *   The URL variables to pass through the URL.
 *
 * @return array
 *   A simple array containing unique error code and text messages.
 */
function eve_api_get_error_list_api() {
  $response = eve_api_get_xml('ErrorList');

  if (!isset($response['xml']->result)) {
    if (variable_get('eve_api_debug', FALSE)) {
      watchdog('eve_api', 'Failed getting Error List API Data. (' . drupal_http_build_query($query) . ') Error Code: ' . $response['errorCode'] . ' Error Message: ' . $response['errorText'], NULL, WATCHDOG_WARNING);
    }

    unset($response['xml']);
    return array('error' => TRUE) + $response;
  }

  $api_data = array();
  $api_data['errorCode'] = (int) $response['errorCode'];
  $api_data['errorText'] = (string) $response['errorText'];
  $api_data['errors'] = array();

  foreach ($response['xml']->result->rowset->children() as $rows) {
    $row = $rows->attributes();

    $api_data['errors'][(int) $row->errorCode] = (string) $row->errorText;
  }

  return $api_data;
}

/**
 * Function to compare the API against the Alliance Blue List.
 *
 * @param array $characters
 *   A complex array full of character information.
 *
 * @return bool
 *   Indicates a character is blue to the alliance.
 */
function eve_api_verify_blue($characters) {
  if (isset($characters['characterID'])) {
    if (($characters['allianceID'] == variable_get('eve_api_allianceID', 0) && variable_get('eve_api_allianceID', 0) != 0) || $characters['corporationID'] == variable_get('eve_api_corporationID', 0)) {
      return TRUE;
    }

    $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID', array(
      ':corporationID' => (int) $characters['corporationID'],
    ));

    if ($result->rowCount()) {
      return TRUE;
    }

    $result = db_query('SELECT contactID FROM {eve_api_contact_list} WHERE standing >= :standing AND contactID IN (:contactIDs)', array(
      ':standing' => (float) variable_get('eve_api_required_standing', 0.1),
      ':contactIDs' => array(
        (int) $characters['characterID'],
        (int) $characters['corporationID'],
        (int) $characters['allianceID'],
      ),
    ));

    if ($result->rowCount()) {
      return TRUE;
    }
  }
  elseif (isset($characters['characters'])) {
    foreach ($characters['characters'] as $character) {
      if (($character['allianceID'] == variable_get('eve_api_allianceID', 0) && variable_get('eve_api_allianceID', 0) != 0) || $character['corporationID'] == variable_get('eve_api_corporationID', 0)) {
        return TRUE;
      }

      $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID', array(
        ':corporationID' => (int) $character['corporationID'],
      ));

      if ($result->rowCount()) {
        return TRUE;
      }

      $result = db_query('SELECT contactID FROM {eve_api_contact_list} WHERE standing >= :standing AND contactID IN (:contactIDs)', array(
        ':standing' => (float) variable_get('eve_api_required_standing', 0.1),
        ':contactIDs' => array(
          (int) $character['characterID'],
          (int) $character['corporationID'],
          (int) $character['allianceID'],
        ),
      ));

      if ($result->rowCount()) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/**
 * Function to generate an array of Blue Characters to the Alliance.
 *
 * @param array $characters
 *   A complex array full of character information.
 *
 * @return array
 *   A simple array of characters blue to the alliance.
 */
function eve_api_list_valid_characters($characters, $blue_only = TRUE) {
  $chars = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      if (!$blue_only) {
        $chars[] = $character['characterName'];
        continue;
      }

      $result = db_query('SELECT contactID FROM {eve_api_contact_list} WHERE standing > 0 AND contactID IN (:contactIDs)', array(
        ':contactIDs' => array(
          (int) $character['characterID'],
          (int) $character['corporationID'],
          (int) $character['allianceID'],
        ),
      ));

      if ($result->rowCount()) {
        $chars[] = $character['characterName'];
        continue;
      }

      $result = db_query('SELECT corporationID FROM {eve_api_alliance_list_corporations} WHERE corporationID = :corporationID', array(
        ':corporationID' => (int) $character['corporationID'],
      ));

      if ($result->rowCount()) {
        $chars[] = $character['characterName'];
      }
    }
  }

  return $chars;
}

/**
 * Function to check if the Characters already exist in the DB.
 *
 * @param array $characters
 *   A complex array full of character information.
 *
 * @return mixed
 *   A simple array of characters found or a bool FALSE.
 */
function eve_api_characters_exist($characters) {
  $chars = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      $result = db_query('SELECT characterID FROM {eve_api_characters} WHERE deleted = 0 AND characterID = :characterID', array(
        ':characterID' => (int) $character['characterID'],
      ));

      if ($result->rowCount()) {
        $chars[] = check_plain($character['characterName']);
      }
    }
  }

  if (empty($chars)) {
    $chars = FALSE;
  }

  return $chars;
}

/**
 * Function to check if the Characters is in a corp.
 *
 * @param int $character_id
 *   A unique character id.
 * @param int $corporation_id
 *   A unique corporation id.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_character_is_in_corp($character_id, $corporation_id) {
  $query = db_select('eve_api_characters', 'c');

  $result = $query->fields('c', array(
      'characterID',
    ))
    ->condition('c.corporationID', $corporation_id, '=')
    ->condition('c.characterID', (int) $character_id, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Function to check if the Characters is a Director.
 *
 * @param int $character_id
 *   A unique character id.
 * @param int $corporation_id
 *   A unique corporation id.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_character_is_director($character_id, $corporation_id = NULL) {
  $query = db_select('eve_api_character_sheet_roles', 'cr');

  if (isset($corporation_id)) {
    $query->join('eve_api_characters', 'c', 'cr.characterID = c.characterID');
    $query->condition('c.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('cr', array(
      'characterID',
    ))
    ->condition('cr.roleID', 1, '=')
    ->condition('cr.characterID', (int) $character_id, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  $query = db_select('eve_api_role_ceo_director', 'crw');

  if (isset($corporation_id)) {
    $query->join('eve_api_characters', 'c', 'crw.characterID = c.characterID');
    $query->condition('c.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('crw', array(
      'characterID',
    ))
    ->condition('crw.isDirector', 1, '=')
    ->condition('crw.characterID', (int) $character_id, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Function to check if the Characters is a CEO.
 *
 * @param int $character_id
 *   A unique character id.
 * @param int $corporation_id
 *   A unique corporation id.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_character_is_ceo($character_id, $corporation_id = NULL) {
  $query = db_select('eve_api_alliance_list_corporations', 'ac');

  if (isset($corporation_id)) {
    $query->condition('ac.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('ac', array(
      'ceoID',
    ))
    ->condition('ac.ceoID', (int) $character_id, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  $query = db_select('eve_api_role_ceo_director', 'crw');

  if (isset($corporation_id)) {
    $query->join('eve_api_characters', 'c', 'crw.characterID = c.characterID');
    $query->condition('c.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('crw', array(
      'characterID',
    ))
    ->condition('crw.isCEO', 1, '=')
    ->condition('crw.characterID', (int) $character_id, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Function to check if the Characters is in a corp.
 *
 * @param int $uid
 *   A unique drupal id.
 * @param int $corporation_id
 *   A unique corporation id.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_user_is_in_corp($uid, $corporation_id) {
  $query = db_select('eve_api_characters', 'c');
  $query->join('users', 'u', 'c.characterID = u.characterID');

  $result = $query->fields('c', array(
      'characterID',
    ))
    ->condition('c.corporationID', $corporation_id, '=')
    ->condition('u.uid', (int) $uid, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Function to check if the Characters is a Director.
 *
 * @param int $uid
 *   A unique drupal id.
 * @param int $corporation_id
 *   A unique corporation id.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_user_is_director($uid, $corporation_id = NULL) {
  $query = db_select('eve_api_character_sheet_roles', 'cr');
  $query->join('users', 'u', 'cr.characterID = u.characterID');

  if (isset($corporation_id)) {
    $query->join('eve_api_characters', 'c', 'cr.characterID = c.characterID');
    $query->condition('c.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('cr', array(
      'characterID',
    ))
    ->condition('cr.roleID', 1, '=')
    ->condition('u.uid', (int) $uid, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  $query = db_select('eve_api_role_ceo_director', 'crw');
  $query->join('users', 'u', 'crw.characterID = u.characterID');

  if (isset($corporation_id)) {
    $query->join('eve_api_characters', 'c', 'crw.characterID = c.characterID');
    $query->condition('c.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('crw', array(
      'characterID',
    ))
    ->condition('crw.isDirector', 1, '=')
    ->condition('u.uid', (int) $uid, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Function to check if the Characters is a CEO.
 *
 * @param int $uid
 *   A unique drupal id.
 * @param int $corporation_id
 *   A unique corporation id.
 *
 * @return bool
 *   TRUE or FALSE.
 */
function eve_api_user_is_ceo($uid, $corporation_id = NULL) {
  $query = db_select('eve_api_alliance_list_corporations', 'ac');
  $query->join('users', 'u', 'ac.ceoID = u.characterID');

  if (isset($corporation_id)) {
    $query->condition('ac.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('ac', array(
      'ceoID',
    ))
    ->condition('u.uid', (int) $uid, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  $query = db_select('eve_api_role_ceo_director', 'crw');
  $query->join('users', 'u', 'crw.characterID = u.characterID');

  if (isset($corporation_id)) {
    $query->join('eve_api_characters', 'c', 'crw.characterID = c.characterID');
    $query->condition('c.corporationID', $corporation_id, '=');
  }

  $result = $query->fields('crw', array(
      'characterID',
    ))
    ->condition('crw.isCEO', 1, '=')
    ->condition('u.uid', (int) $uid, '=')
    ->execute();

  if ($result->rowCount()) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Function that converts an Error ID to a readable message.
 *
 * @param int $error_id
 *   An error ID number.
 *
 * @return string
 *   An error message.
 */
function eve_api_api_error_msg($error_id, $error_text) {
  switch ($error_id) {
    case 0:
      $message = t('OK');
      break;

    case 1:
      $message = t('Invalid API Mask/Type/Expiry');
      break;

    case 2:
      // Too Many Failed API Requests, we will wait 10 minutes to try again.
      $message = t('API Temporarily Down');
      break;

    case 3:
      // API Request Timed Out.
      $message = t('API Temporarily Down');
      break;

    case 4:
      // Invalid response, response received as a HTML document.
      $message = t('API Temporarily Down');
      break;

    case 5:
      // Invalid response cannot parse as XML.
      $message = t('API Temporarily Down');
      break;

    case 403:
      $message = t('API Key Does Not Exist');
      break;

    default:
      $result = db_query('SELECT errorCode, errorText
        FROM {eve_api_error_list}
        WHERE errorCode = :errorCode', array(
          ':errorCode' => (int) $error_id,
        ));

      $row = $result->fetchAssoc();

      if (empty($row)) {
        $message = t('API Temporarily Down');
      }
      else {
        $message = check_plain($error_text);
      }
      break;
  }

  return $message;
}

/**
 * Function that retrieves a simple list of API's associated with a user id.
 *
 * @param int $uid
 *   A unique Drupal User ID.
 *
 * @return mixed
 *   A simple array of character and api key information.
 */
function eve_api_list_api_simple($uid) {
  $uid = (int) $uid;
  $api_list = array();

  $result_a = db_query('SELECT apiID, keyID, vCode, errorCode, updated, created FROM {eve_api_keys} WHERE uid = :uid', array(
    ':uid' => $uid,
  ));

  if ($result_a->rowCount()) {
    foreach ($result_a->fetchAll() as $row_a) {
      if (eve_api_error_code_deleted($row_a->errorCode)) {
        continue;
      }

      $result_b = db_query('SELECT characterID, characterName, corporationID, corporationName, corporationTicker, allianceID, allianceName, allianceTicker FROM {eve_api_characters} WHERE deleted = 0 AND uid = :uid AND apiID = :apiID', array(
        ':uid' => $uid,
        ':apiID' => (int) $row_a->apiID,
      ));

      if ($result_b->rowCount()) {
        foreach ($result_b->fetchAll() as $row_b) {
          $api_list['characters'][$row_b->characterID] = array(
            'characterID' => check_plain($row_b->characterID),
            'characterName' => strip_tags(decode_entities($row_b->characterName)),
            'corporationID' => check_plain($row_b->corporationID),
            'corporationName' => strip_tags(decode_entities($row_b->corporationName)),
            'corporationTicker' => strip_tags(decode_entities($row_b->corporationTicker)),
            'allianceID' => check_plain($row_b->allianceID),
            'allianceName' => strip_tags(decode_entities($row_b->allianceName)),
            'allianceTicker' => strip_tags(decode_entities($row_b->allianceTicker)),
          );
        }
      }
    }
  }

  return $api_list;
}

/**
 * Function that inserts a new API Key.
 *
 * @param object $account
 *   A Drupal user object containing.
 * @param int $key_id
 *   A unique EVE API Key ID.
 * @param string $v_code
 *   A unique EVE API Verification Code.
 *
 * @return mixed
 *   A mixed array of data regarding the success/failure.
 */
function eve_api_create_key($account, $key_id, $v_code) {
  $created = date('Y-m-d H:i:s', time());
  $uid = (int) $account->uid;

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if (isset($characters['error'])) {
    watchdog('eve_api', 'Error occured during registration. (' . drupal_http_build_query($query) . ')<br /><pre>' . print_r($characters, TRUE) . '</pre>', NULL, WATCHDOG_WARNING);

    return FALSE;
  }

  if (!isset($characters['characters'])) {
    watchdog('eve_api', 'Error occured, should never happen. (' . drupal_http_build_query($query) . ')<br /><pre>' . print_r($characters, TRUE) . '</pre>', NULL, WATCHDOG_DEBUG);

    return FALSE;
  }

  $whitelist = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      $whitelist[] = (int) $character['characterID'];
    }
  }

  $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
    ':characterIDs' => $whitelist,
  ));

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
  $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

  if ($result->rowCount()) {
    if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
      $characters['errorCode'] = 1;
      $characters['errorText'] = 'Invalid API Mask/Type/Expiry';
    }
  }
  else {
    if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
      $characters['errorCode'] = 1;
      $characters['errorText'] = 'Invalid API Mask/Type/Expiry';
    }
  }

  $api_id = db_insert('eve_api_keys')->fields(array(
    'uid' => (int) $uid,
    'keyID' => (int) $key_id,
    'vCode' => (string) $v_code,
    'updated' => (string) $created,
    'created' => (string) $created,
    'errorCode' => $characters['errorCode'],
    'errorText' => $characters['errorText'],
  ))->execute();

  if ($characters['errorCode'] == 1) {
    return FALSE;
  }

  foreach ($characters['characters'] as $character) {
    db_merge('eve_api_characters')->key(array('characterID' => (int) $character['characterID']))->fields(array(
      'characterID' => (int) $character['characterID'],
      'uid' => (int) $uid,
      'apiID' => (int) $api_id,
      'characterName' => (string) $character['characterName'],
      'corporationID' => (int) $character['corporationID'],
      'corporationName' => (string) $character['corporationName'],
      'corporationTicker' => (string) $character['corporationTicker'],
      'allianceID' => (int) $character['allianceID'],
      'allianceName' => (string) $character['allianceName'],
      'allianceTicker' => (string) $character['allianceTicker'],
      'accessMask' => (int) $characters['accessMask'],
      'errorCode' => $characters['errorCode'],
      'errorText' => $characters['errorText'],
      'deleted' => 0,
    ))->execute();

    if ($account->name == (string) $character['characterName']) {
      $character_data = $character;
    }
  }

  return isset($character_data) ? $character_data : array('not_found' => TRUE);
}

/**
 * Function that modifies an existing API Key.
 *
 * @param object $account
 *   A Drupal user object containing.
 * @param int $key_id
 *   A unique EVE API Key ID.
 * @param string $v_code
 *   A unique EVE API Verification Code.
 * @param int $api_id
 *   A unique API ID.
 *
 * @return mixed
 *   A mixed array of data regarding the success/failure.
 */
function eve_api_modify_key($account, $key_id, $v_code, $api_id) {
  $updated = date('Y-m-d H:i:s', time());
  $uid = (int) $account->uid;

  $query = array(
    'keyID' => $key_id,
    'vCode' => $v_code,
  );

  $characters = eve_api_get_api_key_info_api($query);

  if ($api_id == 0) {
    return TRUE;
  }

  if (isset($characters['error'])) {
    db_merge('eve_api_keys')->key(array('apiID' => (int) $api_id))->fields(array(
      'apiID' => (int) $api_id,
      'errorCode' => $characters['errorCode'],
      'errorText' => $characters['errorText'],
      'updated' => (string) $updated,
    ))->execute();

    if (eve_api_error_code_deleted($characters['errorCode'])) {
      db_update('eve_api_characters')->fields(array(
        'errorCode' => $characters['errorCode'],
        'errorText' => $characters['errorText'],
        'deleted' => 1,
      ))->condition('apiID', (int) $api_id, '=')->execute();
      return TRUE;
    }
    else {
      db_update('eve_api_characters')->fields(array(
        'errorCode' => $characters['errorCode'],
        'errorText' => $characters['errorText'],
      ))->condition('apiID', (int) $api_id, '=')->execute();
      return FALSE;
    }
  }

  if (!isset($characters['characters'])) {
    watchdog('eve_api', 'Error occured, should never happen. (' . drupal_http_build_query($query) . ')<br /><pre>' . print_r($characters, TRUE) . '</pre>', NULL, WATCHDOG_WARNING);
    return TRUE;
  }

  $whitelist = array();

  if (!empty($characters)) {
    foreach ($characters['characters'] as $character) {
      $whitelist[] = (int) $character['characterID'];
    }
  }

  $result = db_query('SELECT characterID FROM {eve_api_whitelist} WHERE characterID IN (:characterIDs)', array(
    ':characterIDs' => $whitelist,
  ));

  $allow_expires = variable_get('eve_api_require_expires', FALSE) ? FALSE : !empty($characters['expires']);
  $allow_type = variable_get('eve_api_require_type', TRUE) ? $characters['type'] != 'Account' : FALSE;

  if ($result->rowCount()) {
    if ($allow_expires || ($characters['accessMask'] & 8388680) != 8388680) {
      $characters['errorCode'] = 1;
      $characters['errorText'] = 'Invalid API Mask/Type/Expiry';
    }
  }
  else {
    if ($allow_expires || $allow_type || ($characters['accessMask'] & variable_get('eve_api_access_mask', 268435455)) != variable_get('eve_api_access_mask', 268435455)) {
      $characters['errorCode'] = 1;
      $characters['errorText'] = 'Invalid API Mask/Type/Expiry';
    }
  }

  db_update('eve_api_characters')->fields(array(
    'errorCode' => $characters['errorCode'],
    'errorText' => $characters['errorText'],
    'deleted' => 1,
  ))->condition('apiID', (int) $api_id)->execute();
  db_merge('eve_api_keys')->key(array('apiID' => (int) $api_id))->fields(array(
    'apiID' => (int) $api_id,
    'uid' => (int) $uid,
    'keyID' => (int) $key_id,
    'vCode' => (string) $v_code,
    'updated' => (string) $updated,
    'errorCode' => $characters['errorCode'],
    'errorText' => $characters['errorText'],
  ))->execute();

  if ($characters['errorCode'] == 1) {
    return TRUE;
  }

  foreach ($characters['characters'] as $character) {
    db_merge('eve_api_characters')->key(array('characterID' => (int) $character['characterID']))->fields(array(
      'characterID' => (int) $character['characterID'],
      'uid' => (int) $uid,
      'apiID' => (int) $api_id,
      'characterName' => (string) $character['characterName'],
      'corporationID' => (int) $character['corporationID'],
      'corporationName' => (string) $character['corporationName'],
      'corporationTicker' => (string) $character['corporationTicker'],
      'allianceID' => (int) $character['allianceID'],
      'allianceName' => (string) $character['allianceName'],
      'allianceTicker' => (string) $character['allianceTicker'],
      'accessMask' => (int) $characters['accessMask'],
      'errorCode' => $characters['errorCode'],
      'errorText' => $characters['errorText'],
      'deleted' => 0,
    ))->execute();
  }

  return FALSE;
}

/**
 * Function that checks if a specific Error Code means we need to stop.
 *
 * @param int $code
 *   An EVE API Error Code.
 *
 * @return bool
 *   TRUE or FALSE if the code means to stop.
 */
function eve_api_error_code($code) {
  $error_codes = array(
    200,
    201,
    202,
    203,
    204,
    205,
    206,
    207,
    208,
    209,
    210,
    211,
    212,
    213,
    220,
    221,
    222,
    223,
    901,
    902,
    903,
    904,
    999,
    1001,
  );

  if (in_array($code, $error_codes)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Function that checks if a specific Error Code is Deleted/Invalid.
 *
 * @param int $code
 *   An EVE API Error Code.
 *
 * @return bool
 *   TRUE or FALSE if the code means to stop.
 */
function eve_api_error_code_deleted($code) {
  $code = (int) $code;

  $error_codes = array(
    202,
    203,
    204,
    205,
    210,
    211,
    212,
    222,
    521,
  );

  if (in_array($code, $error_codes)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
