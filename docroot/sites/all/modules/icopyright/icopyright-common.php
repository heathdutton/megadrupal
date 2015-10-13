<?php
/**
 * @file
 * Common functions for communicating with the iCopyright servers
 */

// Which iCopyright server should we talk to via REST? The standard is license.icopyright.net, port 80,
// but you can target alternate infrastructures (normally for debugging purposes) by changing these variables.
// auth_user and auth_password are for servers that are locked down with HTTP basic authentication.
define('ICOPYRIGHT_SERVER', 'license.icopyright.net');
define('ICOPYRIGHT_PORT', 80);
define('ICOPYRIGHT_AUTH_USER', '');
define('ICOPYRIGHT_AUTH_PASSWORD', '');

/**
 * Return the iCopyright server and port that is handling the various services
 *
 * @param bool $secure 
 *      should we go over https?
 * @return the full server specification
 */
function icopyright_get_server($secure = FALSE) {
  $server = ($secure ? 'https' : 'http') . '://' . ICOPYRIGHT_SERVER;
  if (ICOPYRIGHT_PORT != 80) {
    $server .= ':' . ICOPYRIGHT_PORT;
  }
  return $server;
}

/**
 * Given a RESTful request to add a publisher, post it to iCopyright's servers
 *
 * @param  $postdata
 *      the package of settings to post
 * @param  $useragent
 *      the type and version of the plugin doing the posting, for example
 *      "iCopyright Drupal Plugin v6.x-1.0"
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return the response from iCopyright's servers
 */
function icopyright_post_new_publisher($postdata, $useragent, $email, $password) {
  // Create the new publisher
  $res = icopyright_post('/api/xml/publisher/addbrief', $postdata, $useragent);
  if(icopyright_check_response($res)) {
    $xml = @simplexml_load_string($res->response);
    $pid = $xml->publication_id;
    icopyright_post_syndication_service($pid, TRUE, $useragent, $email, $password);
  }
  return $res;
}

/**
 * Checks the response object for success code. Returns true if all is OK.
 *
 * @param  $res
 *      The response from a post
 * @return TRUE if all is OK
 */
function icopyright_check_response($res) {
  return ($res->http_code == '200');
}

/**
 * Updates a publication's feed URL. The URL must have one and only one asterisk.
 *
 * @param $pid
 *      the publication ID
 * @param $value
 *      the feed URL
 * @param $useragent
 *      a user agent string
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return the response from iCopyright's servers
 */
function icopyright_post_update_feed_url($pid, $value, $useragent, $email, $password) {
  $url = "/api/xml/publication/update/$pid";
  $postdata = 'feed_url=' . urlencode($value);
  $res = icopyright_post($url, $postdata, $useragent, icopyright_make_header($email, $password));
  return $res;
}

/**
 * Sets EZ-Excerpt on or off for the publication
 *
 * @param  $pid
 *      the publication ID
 * @param  $value
 *      whether EZ Excerpt should be on (true) or off (false)
 * @param  $useragent
 *      a user agent string
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return the response from iCopyright's servers in XML format
 */
function icopyright_post_ez_excerpt($pid, $value, $useragent, $email, $password) {
  $url = "/api/xml/publication/toolbar/$pid";
  $postdata = 'ez_excerpt_enabled=' . ($value == 0 ? 'false' : 'true');
  $res = icopyright_post($url, $postdata, $useragent, icopyright_make_header($email, $password));
  return $res;
}

/**
 * Changes the toolbar theme.
 *
 * @param $pid the publication ID
 * @param $theme the new theme: default, green, etc.
 * @param $background the new background theme
 * @param $useragent a user agent string identifying the plugin version
 * @param $email the email address of the user
 * @param $password the user's iCopyright password
 * @return the response from iCopyright's servers in XML format
 */
function icopyright_post_toolbar_theme($pid, $theme, $background, $useragent, $email, $password) {
  $url = "/api/xml/publication/toolbar/$pid";
  $postdata = "theme=$theme&background=$background";
  $res = icopyright_post($url, $postdata, $useragent, icopyright_make_header($email, $password));
  return $res;
}

/**
 * Returns an associative array of valid themes
 * @return an array of themes
 */
function icopyright_theme_options() {
  return array(
    'CLASSIC' => 'Classic',
    'CADET' => 'Cadet',
    'EARTH' => 'Earth',
    'OCEAN' => 'Ocean',
    'FOREST' => 'Forest',
    'AMARANTH' => 'Amaranth',
    'BLIZZARD' => 'Blizzard'
  );
}

/**
 * Returns an associative array of valid theme backgrounds
 * @return an array of backgrounds
 */
function icopyright_theme_backgrounds() {
  return array(
    'OPAQUE' => 'Opaque',
    'TRANSPARENT' => 'Transparent'
  );
}

/**
 * Enables or disables the syndication service for the publisher
 *
 * @param  $pid
 *      the publication ID
 * @param  $value
 *      whether syndication should be on (true) or off (false)
 * @param  $useragent
 *      the type and version of the plugin doing the posting, for example
 *      "iCopyright Drupal Plugin v6.x-1.0"
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return the response from iCopyright's servers in XML format
 */
function icopyright_post_syndication_service($pid, $value, $useragent, $email, $password) {
  return icopyright_post_service(101, $pid, $value, $useragent, $email, $password);
}

/**
 * Enables or disables the share service for the publisher
 *
 * @param  $pid
 *      the publication ID
 * @param  $value
 *      whether share should be on (true) or off (false)
 * @param  $useragent
 *      the type and version of the plugin doing the posting, for example
 *      "iCopyright Drupal Plugin v6.x-1.0"
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return the response from iCopyright's servers in XML format
 */
function icopyright_post_share_service($pid, $value, $useragent, $email, $password) {
  return icopyright_post_service(200, $pid, $value, $useragent, $email, $password);
}

/**
 * Enables or disables a service by ID for the publisher
 *
 * @param  $sid
 *      the service ID
 * @param  $pid
 *      the publication ID
 * @param  $value
 *      whether share should be on (true) or off (false)
 * @param  $useragent
 *      the type and version of the plugin doing the posting, for example
 *      "iCopyright Drupal Plugin v6.x-1.0"
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return the response from iCopyright's servers in XML format
 */
function icopyright_post_service($sid, $pid, $value, $useragent, $email, $password) {
  $url = "/api/xml/service/offer/$sid";
  $queryargs = array();
  array_push($queryargs, "publication=$pid");
  array_push($queryargs, 'enable=' . ($value == 0 ? 'false' : 'true'));
  $res = icopyright_post($url, join('&', $queryargs), $useragent, icopyright_make_header($email, $password));
  return $res;
}

/**
 * Given an email address and a password, create the appropriate headers for authentication to change
 * Conductor settings
 *
 * @param  $email
 *      the email address of the user
 * @param  $password
 *      the user's iCopyright password
 * @return headers to use
 */
function icopyright_make_header($email, $password) {
  $header_encode = base64_encode("$email:$password");
  return array("Authorization: Basic $header_encode");
}

/**
 * General helper function to post RESTfully to iCopyright. Returns an object with the following
 * fields: response (the text back from the server); http_code (the code, like 200 or 404); http_expl
 * (the http string corresponding to that code); curl_code (the curl error code)
 *
 * @param $url
 *      the URL to post to
 * @param $postdata
 *      the data that we're sending up
 * @param $useragent
 *      the user agent doing the requesting -- should be the plugin and version number
 * @param $headers
 *      headers to include for authentication, if any
 * @return object results of the post as specified
 */
function icopyright_post($url, $postdata, $useragent = NULL, $headers = NULL) {
  $rs_ch = curl_init(icopyright_get_server(TRUE) . $url);
  curl_setopt($rs_ch, CURLOPT_SSL_VERIFYPEER, false);
  
  // If the server is locked down (for testing, for example) use auth tokens
  if ((ICOPYRIGHT_AUTH_USER != NULL) && (ICOPYRIGHT_AUTH_PASSWORD != NULL)) {
    $token = ICOPYRIGHT_AUTH_USER . ':' . ICOPYRIGHT_AUTH_PASSWORD;
    curl_setopt($rs_ch, CURLOPT_USERPWD, $token);
    curl_setopt($rs_ch, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
  }
  curl_setopt($rs_ch, CURLOPT_POST, 1);
  curl_setopt($rs_ch, CURLOPT_POSTFIELDS, $postdata);
  curl_setopt($rs_ch, CURLOPT_FOLLOWLOCATION, 1);
  curl_setopt($rs_ch, CURLOPT_HEADER, 0);
  if ($headers != NULL) {
    curl_setopt($rs_ch, CURLOPT_HTTPHEADER, $headers);
  }
  curl_setopt($rs_ch, CURLOPT_RETURNTRANSFER, 1);
  curl_setopt($rs_ch, CURLOPT_TIMEOUT, 60);
  if ($useragent != NULL) {
    curl_setopt($rs_ch, CURLOPT_USERAGENT, $useragent);
  }

  // Fetch the respopnse
  $rv = new stdClass();
  if($fullresponse = curl_exec($rs_ch)) {
    $rv->response = str_replace('ns0:', '', $fullresponse);
    // Got a response, but sometimes the response went to the server but is still an error, with the code embedded:
    // in other words, they return 200 but there's an error message in the payload
    $xml = @simplexml_load_string($rv->response);
    $status = $xml->status;
    $rv->http_code = (string)$status['code'];
  } else {
    $rv->response = $fullresponse;
    $rv->http_code = curl_getinfo($rs_ch, CURLINFO_HTTP_CODE);
  }
  $responses = array(
    100 => 'Continue', 101 => 'Switching Protocols',
    200 => 'OK', 201 => 'Created', 202 => 'Accepted', 203 => 'Non-Authoritative Information', 204 => 'No Content', 205 => 'Reset Content', 206 => 'Partial Content',
    300 => 'Multiple Choices', 301 => 'Moved Permanently', 302 => 'Found', 303 => 'See Other', 304 => 'Not Modified', 305 => 'Use Proxy', 307 => 'Temporary Redirect',
    400 => 'Bad Request', 401 => 'Unauthorized', 402 => 'Payment Required', 403 => 'Forbidden', 404 => 'Not Found', 405 => 'Method Not Allowed', 406 => 'Not Acceptable', 407 => 'Proxy Authentication Required', 408 => 'Request Time-out', 409 => 'Conflict', 410 => 'Gone', 411 => 'Length Required', 412 => 'Precondition Failed', 413 => 'Request Entity Too Large', 414 => 'Request-URI Too Large', 415 => 'Unsupported Media Type', 416 => 'Requested range not satisfiable', 417 => 'Expectation Failed',
    500 => 'Internal Server Error', 501 => 'Not Implemented', 502 => 'Bad Gateway', 503 => 'Service Unavailable', 504 => 'Gateway Time-out', 505 => 'HTTP Version not supported'
  );
  $rv->http_expl = $responses[$rv->http_code];
  $rv->curl_errno = curl_errno($rs_ch);
  $rv->curl_error = curl_error($rs_ch);
  curl_close($rs_ch);

  return $rv;
}
