<?php

/**
 * @file
 * This file is a standalone proxy for BOSH.
 */

// BOSH requests may take 60 seconds to complete.
// We set greater number just to make sure.
@set_time_limit(80);

define('HTTP_SERVICE_UNAVAILABLE', 503);

// If settings file exists, include it; otherwise show error.
$settings_file = dirname(__FILE__) . '/http_bind_settings.php';
if (file_exists($settings_file)) {
  require_once $settings_file;
}
else {
  header('Content-Type: text/plain', TRUE, HTTP_SERVICE_UNAVAILABLE);
  echo 'Can not find settings file at ' . $settings_file . "\r\n";
  echo 'Please create a new file at that location with custom settings.';
  exit(1);
}

// Check if settings file provides required variables.
if (!isset($http_bind_settings) or !is_array($http_bind_settings)) {
  header('Content-Type: text/plain', TRUE, HTTP_SERVICE_UNAVAILABLE);
  echo '$settings_file variable is undefined.' .
       ' You should define this array in your http_bind_settings.php file';
  exit(1);
}

// Check if this script is called using POST method.
// (BOSH only works with POST method).
if (!isset($HTTP_RAW_POST_DATA)) {
  header('Content-Type: text/plain', TRUE, HTTP_SERVICE_UNAVAILABLE);
  echo 'This file should be called only with POST method.' .
       ' GET is not supported.';
  exit(1);
}

/**
 * Read settings.
 */
$bosh_url = $http_bind_settings['bosh_url'];

/**
 * Do proxy the request to BOSH server.
 */
$response = conversejs_http_request($bosh_url, array(), 'POST', $HTTP_RAW_POST_DATA);
print isset($response->data) ? $response->data : '';

/**
 * This is a standalone implementation of D6' drupal_http_request().
 *
 * Perform an HTTP request.
 *
 * This is a flexible and powerful HTTP client implementation. Correctly handles
 * GET, POST, PUT or any other HTTP requests. Handles redirects.
 *
 * @param string $url
 *   A string containing a fully qualified URI.
 * @param array $headers
 *   An array containing an HTTP header => value pair.
 * @param string $method
 *   A string defining the HTTP request to use.
 * @param string $data
 *   A string containing data to include in the request.
 * @param int $retry
 *   An integer representing how many times to retry the request in case of a
 *   redirect.
 *
 * @return \stdClass
 *   An object containing the HTTP request headers, response code, protocol.
 */
function conversejs_http_request($url, $headers = array(), $method = 'GET', $data = NULL, $retry = 3) {
  $result = new stdClass();

  // Parse the URL and make sure we can handle the schema.
  $uri = parse_url($url);

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

  switch ($uri['scheme']) {
    case 'http':
    case 'feed':
      $port = isset($uri['port']) ? $uri['port'] : 80;
      $host = $uri['host'] . ($port != 80 ? ':' . $port : '');
      $fp = @fsockopen($uri['host'], $port, $errno, $errstr, 15);
      break;

    case 'https':
      // Note: Only works for PHP 4.3 compiled with OpenSSL.
      $port = isset($uri['port']) ? $uri['port'] : 443;
      $host = $uri['host'] . ($port != 443 ? ':' . $port : '');
      $fp = @fsockopen('ssl://' . $uri['host'], $port, $errno, $errstr, 20);
      break;

    default:
      $result->error = 'invalid schema ' . $uri['scheme'];
      $result->code = -1003;

      return $result;
  }

  // Make sure the socket opened properly.
  if (!$fp) {
    // When a network error occurs, we use a negative number so it does not
    // clash with the HTTP status codes.
    $result->code = -$errno;
    $result->error = trim($errstr);

    return $result;
  }

  // Construct the path to act on.
  $path = isset($uri['path']) ? $uri['path'] : '/';
  if (isset($uri['query'])) {
    $path .= '?' . $uri['query'];
  }

  // Create HTTP request.
  $defaults = array(
    // RFC 2616: "non-standard ports MUST, default ports MAY be included".
    // We don't add the port to prevent from breaking rewrite rules checking the
    // host that do not take into account the port number.
    'Host' => "Host: $host",
    'User-Agent' => 'User-Agent: Drupal (+http://drupal.org/)',
  );

  // Only add Content-Length if we actually have any content or if it is a POST
  // or PUT request. Some non-standard servers get confused by Content-Length in
  // at least HEAD/GET requests, and Squid always requires Content-Length in
  // POST/PUT requests.
  $content_length = strlen($data);
  if ($content_length > 0 || $method == 'POST' || $method == 'PUT') {
    $defaults['Content-Length'] = 'Content-Length: ' . $content_length;
  }

  // If the server url has a user then attempt to use basic authentication.
  if (isset($uri['user'])) {
    $defaults['Authorization'] = 'Authorization: Basic ' . base64_encode($uri['user'] . (!empty($uri['pass']) ? ":" . $uri['pass'] : ''));
  }

  foreach ($headers as $header => $value) {
    $defaults[$header] = $header . ': ' . $value;
  }

  $request = $method . ' ' . $path . " HTTP/1.0\r\n";
  $request .= implode("\r\n", $defaults);
  $request .= "\r\n\r\n";
  $request .= $data;

  $result->request = $request;

  fwrite($fp, $request);

  // Fetch response.
  $response = '';
  while (!feof($fp) && $chunk = fread($fp, 1024)) {
    $response .= $chunk;
  }
  fclose($fp);

  // Parse response.
  list($split, $result->data) = explode("\r\n\r\n", $response, 2);
  $split = preg_split("/\r\n|\n|\r/", $split);

  list($protocol, $code, $status_message) = explode(' ', trim(array_shift($split)), 3);
  $result->protocol = $protocol;
  $result->status_message = $status_message;

  $result->headers = array();

  // Parse headers.
  while ($line = trim(array_shift($split))) {
    list($header, $value) = explode(':', $line, 2);
    if (isset($result->headers[$header]) && $header == 'Set-Cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$header] .= ',' . trim($value);
    }
    else {
      $result->headers[$header] = trim($value);
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
      $location = $result->headers['Location'];

      if ($retry) {
        $result = conversejs_http_request($result->headers['Location'], $headers, $method, $data, --$retry);
        $result->redirect_code = $result->code;
      }
      $result->redirect_url = $location;

      break;

    default:
      $result->error = $status_message;
  }

  $result->code = $code;
  return $result;
}
