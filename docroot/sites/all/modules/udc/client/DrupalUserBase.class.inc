<?php
/**
 * @file
 * -----------------------------------------------------------------------------
 * Drupal 7 User Data Connector module - External script client base class
 * -----------------------------------------------------------------------------
 * @author Stefan Wilhelm
 * @package org.drupal.module.udc
 * @license GPL2
 */


// This is a lock to prevent that client classes are accidentally included. It
// can be bypassed for testing and requirements cbecks.
if (defined('DRUPAL_ROOT') && !defined('USER_DATA_CONNECTOR_ALLOW_CLIENT_INCLUDE')) {
  throw new Exception("User Data Connector Module client classes cannot be included in Drupal context (it makes no sense anyway).");
}

/**
 * Implements the base functionality to establish and test the connection to the
 * server script, as well as sending a request with arguments. Uses basic sockets
 * for HTTP connections or cURL for HTTPS connections. To send the basic request
 * to the server ( protected sendRequest() ), request data are converted into
 * JSON format, and the server script response payload data are JSON parsed/unserialized.
 * Additional status, error and debugging data are provided with the response.
 */
class DrupalUserBase {

  /**
   * The security token to be sent and matched
   * @var string
   */
  private static $serverToken = '';

  /**
   * Contains the site name you want to refer to. This is the name
   * of the subfolder in the DRUPAL_ROOT/sites directory, not necessarily
   * the domain name/host name.
   * @var string
   */
  private static $serverSite = '';

  /**
   * Contains the server script process status or NULL if not available.
   * @var mixed
   */
  private $serverResponseStatus = NULL;

  /**
   * Contains the server script process time or NULL if not available.
   * @var mixed
   */
  private $serverProcessTime = NULL;

  /**
   * Contains extended debug information send by the server script
   * Only available if debugging switched on.
   * @var mixed
   */
  private $serverDebug = NULL;

  /**
   * Contains the error send by the server script
   * @var string
   */
  private $serverError = '';

  /**
   * Sets the site you want to refer to. This is the name of the subfolder
   * in the DRUPAL_ROOT/sites directory, not necessarily the host name.
   * @param string $sitename
   */
  public static function setSite($sitename) {
    self::$serverSite = trim($sitename);
  }

  /**
   * Returns the site name referred to, see setSite for more information.
   * @return string
   */
  public static function getSite() {
    return self::$serverSite;
  }

  /**
   * Returns the number of millisecones the server script needed for execution
   * @return string
   */
  public function getServerProcessTime() {
    return $this->serverProcessTime;
  }

  /**
   * Returns error and debug information sent with the response
   * @return string
   */
  public function getDebug() {
    return $this->serverDebug;
  }

  /**
   * Returns server error information sent with the response
   * @return string
   */
  public function getError() {
    return $this->serverError;
  }

  /**
   * Sets the server token
   * @param string $serverScriptToken
   */
  public static function setToken($serverScriptToken) {
    self::$serverToken = (string) $serverScriptToken;
  }

  /**
   * Constructor
   * @return DrupalUserAuth
   */
  protected function __construct() {
  }

  /**
   * Send a ping request to check if the server script is up and the token
   * is correct. Returns TRUE on success, FALSE on fail. The error message
   * can be obtained with $this->getError();
   *
   * @return bool
   * @throws Exception
   */
  public function ping() {
    try {
      $this->sendRequest('ping');
      $err = $this->getError();
      return empty($err);
    } catch (Exception $e) {
      return FALSE;;
    }
  }

  /**
   * Send the request to the server script and update the instance variables
   * of this object as return values.
   *
   * @param string $request
   * @param array $data = array
   * @throws Exception
   */
  protected function sendRequest($request, array $data = array()) {
    if (!is_array($data)) {
      throw new Exception('Drupal user request data must be an assoc. array');
    }

    $data = array(
      'request' => $request,
      'site' => self::$serverSite,
      'token' => strval(self::$serverToken),
      'data' => $data
    );

    // Dedicated for the simple test, as the server script does not know the
    // table prefix it shall refer to.
    if (isset($GLOBALS['simpletest_udc_db_prefix'])) {
      $data['simpletest-db-prefix'] = $GLOBALS['simpletest_udc_db_prefix'];
    }

    $data = http_build_query($data);
    $host = defined('USER_DATA_CONNECTOR_USE_HTTP_HOST_INSTEAD_OF_LOCALHOST') && USER_DATA_CONNECTOR_USE_HTTP_HOST_INSTEAD_OF_LOCALHOST ? $_SERVER['HTTP_HOST'] : 'localhost';
    $addr = $_SERVER['SERVER_ADDR'];

    if (defined('USER_DATA_CONNECTOR_MODULE_PATH')) {
      $path = trim(USER_DATA_CONNECTOR_MODULE_PATH, '/ ') . '/server/server.php';
    }
    else if (strpos(__FILE__, $_SERVER['DOCUMENT_ROOT']) === FALSE) {
      throw new Exception("The path to the server script does not contain the server document root (\$_SERVER[DOCUMENT_ROOT])");
    }
    else {
      $path = trim(str_replace(array('\\', '//'), '/', str_ireplace($_SERVER['DOCUMENT_ROOT'], '', dirname(dirname(__FILE__)))), ' /') . '/server/server.php';
    }

    // CURL capability
    if (function_exists('curl_init')) {
      $curl = curl_init();
      curl_setopt_array($curl, array(
        CURLOPT_HEADER => TRUE,
        CURLOPT_URL => (defined('USER_DATA_CONNECTOR_USE_SSL') && USER_DATA_CONNECTOR_USE_SSL ? 'https' : 'http') . "://$host/$path",
        CURLOPT_POSTFIELDS => $data,
        CURLOPT_RETURNTRANSFER => TRUE,
        CURLOPT_SSL_VERIFYPEER => FALSE,
        CURLOPT_CONNECTTIMEOUT => 10,
        CURLOPT_TIMEOUT => 10,
        CURLOPT_USERAGENT => 'dp-user-connector-client',
        CURLOPT_FORBID_REUSE => TRUE,
        CURLOPT_NOPROGRESS => TRUE,
      ));
      $raw_response = curl_exec($curl);
      curl_close($curl);
    }
    else if (defined('USER_DATA_CONNECTOR_USE_SSL') && USER_DATA_CONNECTOR_USE_SSL) {
      throw new Exception("You need cURL to use SSL connections");
    }
    else {
      // No SSL --> Socket
      $fp = fsockopen("$addr", 80, $errno, $errstr, 5);
      if (!$fp) {
        throw new Exception("Cannot check user: $errstr");
      }
      else {
        fputs($fp, "POST $path HTTP/1.1\r\n");
        fputs($fp, "Host: $host\r\n");
        fputs($fp, "Content-type: application/x-www-form-urlencoded\r\n");
        fputs($fp, "Content-length: " . strlen($data) . "\r\n");
        fputs($fp, "Connection: close\r\n\r\n");
        fputs($fp, $data);
        $raw_response = '';
        while (!feof($fp)) {
          $raw_response .= fgets($fp, 128);
        }
      }
    }

    // Parse the response
    $r_string = str_replace("\r\n", "\n", $raw_response);
    $raw_response = explode("\n", $r_string);
    if (strpos($raw_response[0], '200 OK') === FALSE) {
      if (strpos($raw_response[0], '401') !== FALSE) {
        throw new Exception("Cannot check user: Server refused connection, check your script token.");
      }
      else {
        throw new Exception("Cannot check user: Server returned {$raw_response[0]} for request $addr/$path");
      }
    }
    else {
      while (!empty($raw_response) && reset($raw_response) != '') {
        array_shift($raw_response);
      }

      if (!empty($raw_response)) array_shift($raw_response);

      if (empty($raw_response)) {
        throw new Exception("Cannot check user: Server did not text data");
      }
      else {
        $response = @json_decode(reset($raw_response), TRUE);
        if (!is_array($response)) {
          $this->serverDebug = $r_string;
          throw new Exception('Server response format incorrect. $this->debug contains the response text.');
        }
        else {
          $this->serverResponseStatus = $response['status'];
          $this->serverProcessTime = isset($response['processtime']) ? $response['processtime'] : NULL;
          $this->serverDebug = isset($response['debug']) ? $response['debug'] : NULL;
          $this->serverError = isset($response['error']) ? $response['error'] : NULL;
          return is_array($response['data']) ? $response['data'] : array();;
        }
      }
    }
    return FALSE;
  }
}
