<?php
/**
 * @file
 * Provides functionality to connect with A12 webservices. Currently this is 
 * only used with the Enterprise Search module but can be used in the future to
 * connect to other services.
 */

class A12Connector {
  const WEBSERVICES_URL = 'https://services.axis12.com/xmlrpc.php';
  const VALIDATE_METHOD = 'a12_webservices.validate';

  protected $accessKey;
  protected $secretKey;
  protected $timestamp;
  protected $url;

  /**
   * Constructor.
   *
   * Sets up the keys, site and timestamp that is used to generate the headers
   * that are going to be passed to the A12 servers. 
   *
   * @param string $access_key
   *   The access key for the current users subscription.
   * @param string $secret_key
   *   The secret key for the current users subscription.
   * @param string $url
   *   The URL of the site that the user is connecting from. This must match the
   *   URL that we are making the request from.
   */
  public function __construct($access_key, $secret_key, $url) {
    $this->accessKey = $access_key;
    $this->secretKey = $secret_key;
    $this->timestamp = REQUEST_TIME;
    $this->url = $url;
  }

  /**
   * Perform the authentication request against the A12 XMLRPC server.
   * 
   * @throws A12ConnectorException
   * @return bool
   *   TRUE if the user has successfully authenticated against the server.
   */
  public function authenticate() {
    require_once './includes/xmlrpc.inc';
    require_once './includes/xmlrpcs.inc';
    $url = self::WEBSERVICES_URL;
    $headers = $this->getHeaders();
    $result = xmlrpc($url, array(self::VALIDATE_METHOD => array($headers)));

    if ($errno = xmlrpc_errno()) {
      $message = '@message (@errno): %server - %method - <pre>@data</pre>';
      $params = array(
        '@message' => xmlrpc_error_msg(),
        '@errno' => xmlrpc_errno(),
        '%server' => $url,
        '%method' => $method,
        '@data' => print_r($data, TRUE),
      );

      watchdog('a12_connect', $message, $params, WATCHDOG_ERROR);
    }

    $result = json_decode($result);

    if (!$result) {
      throw new A12ConnectorException('FND-600');
    }
    elseif (isset($result->error_code)) {
      throw new A12ConnectorException($result->error_code);
    }

    return TRUE;
  }

  /**
   * Prepares headers that will be used by A12 webservices.
   *
   * @see generateHash()
   *
   * @return array
   *   An array of headers that can be passed to an XMLRPC request.
   */
  public function getHeaders($url = NULL) {
    if (!$url) {
      $url = $this->url;
    }

    $headers = array();
    $headers['timestamp'] = $this->timestamp;
    $headers['identifier'] = $this->accessKey;
    $headers['site_url'] = $url;
    $headers['site_name'] = $_SERVER['SERVER_NAME'];
    $headers['ip'] = $_SERVER['SERVER_ADDR'];
    $headers['hash'] = $this->generateHash($url);

    return $headers;
  }

  /**
   * Generate a hash from the secret key, timestamp and URL.
   *
   * @param string $url
   *   The URL that we want to use to create the hash from. This must match the 
   *   URL of the page we are executing the request from otherwise there will be
   *   a hash mismatch.
   */
  protected function generateHash($url) {
    $time = $this->timestamp;
    $magic_key = $this->secretKey;
    $salt = substr(base_convert(sha1($url), 16, 36), 0, 6);
    $data = $time . ':' . $salt . ':' . $magic_key;
    return base64_encode(hash_hmac('sha1', $data, $magic_key, TRUE));
  }
}
