<?php
/**
 * @file
 * PHP Class to handle requests between client and capturemypage.com website.
 */


/**
 * Class cmpAPI.
 */
class cmpAPI {

  /**
   * Service key.
   *
   * @var string
   */
  private $serviceKey;

  /**
   * API endpoint.
   *
   * @var string
   */
  private $endpoint = 'http://api.capturemypage.com/api/v2/';

  /**
   * Constructor.
   *
   * @param string $serviceKey
   *  Service key.
   */
  function __construct($serviceKey = '') {
    // Set value of service key.
    $this->serviceKey = strtoupper($serviceKey);
  }

  /**
   * Make a request.
   *
   * @param string $action
   *  Action types:
   *  - get/html: if the action is "get/html", the API returns with the HTML
   *    source code of the website.
   *  - get/thumbnail: if the action is "get/thumbnail", the API returns with
   *    the details of generated preview screen-shot image of the website.
   *
   * @param array $params
   *  An option array to configure API.
   *
   * @return mixed
   */
  function makeRequest($action = 'get/html', $params = array(), $only_url = FALSE) {
    $data = array(
      'action' => $action,
      'servicekey' => $this->serviceKey,
      'params'     => $params,
    );

    $options = array(
      'method' => 'GET',
    );

    $query = $this->_http_build_query($data);
    $url = $this->endpoint . $action . '?' . $query;

    if ($only_url === TRUE) {
      return $url;
    }

    $response = $this->_http_request($url, $options);

    if (isset($response->error)) {
      return array(
          'error'   => $response->code,
          'message' => $response->error,
          'data'    => '',
      );
    }

    return json_decode($response->data, TRUE);
  }

  /**
   * Parses an array into a valid, rawurlencoded query string.
   *
   * This differs from http_build_query() as we need to rawurlencode()
   * (instead of urlencode()) all query parameters.
   *
   * @param $query
   *   The query parameter array to be processed, e.g. $_GET.
   *
   * @param $parent
   *   Internal use only. Used to build the $query array key for nested items.
   *
   * @return string
   *   A rawurlencoded string which can be used as or appended to the URL query
   *   string.
   */
  function _http_build_query(array $query, $parent = '') {
    $params = array();

    foreach ($query as $key => $value) {
      $key = ($parent ? $parent . '[' . rawurlencode($key) . ']' : rawurlencode($key));

      // Recurse into children.
      if (is_array($value)) {
        $params [] = $this->_http_build_query($value, $key);
      }
      // If a query parameter value is NULL, only append its key.
      elseif (!isset($value)) {
        $params [] = $key;
      }
      else {
        // For better readability of paths in query strings, we decode slashes.
        $params [] = $key . '=' . str_replace('%2F', '/', rawurlencode($value));
      }
    }

    return implode('&', $params);
  }

  /**
   * Performs an HTTP request.
   *
   * This is a flexible and powerful HTTP client implementation. Correctly
   * handles GET, POST, PUT or any other HTTP requests. Handles redirects.
   *
   * @param $url
   *   A string containing a fully qualified URI.
   * @param array $options
   *   (optional) An array that can have one or more of the following elements:
   *   - headers: An array containing request headers to send as name/value pairs.
   *   - method: A string containing the request method. Defaults to 'GET'.
   *   - data: A string containing the request body, formatted as
   *     'param=value&param=value&...'. Defaults to NULL.
   *   - max_redirects: An integer representing how many times a redirect
   *     may be followed. Defaults to 3.
   *   - timeout: A float representing the maximum number of seconds the function
   *     call may take. The default is 30 seconds. If a timeout occurs, the error
   *     code is set to the HTTP_REQUEST_TIMEOUT constant.
   *   - context: A context resource created with stream_context_create().
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
   *   - headers: An array containing the response headers as name/value pairs.
   *     HTTP header names are case-insensitive (RFC 2616, section 4.2), so for
   *     easy access the array keys are returned in lower case.
   *   - data: A string containing the response body that was received.
   */
  function _http_request($url, array $options = array()) {
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

    $this->_timer_start(__FUNCTION__);

    // Merge the default options.
    $options += array(
      'headers'       => array(),
      'method'        => 'GET',
      'data'          => NULL,
      'max_redirects' => 3,
      'timeout'       => 30.0,
      'context'       => NULL,
    );

    // Merge the default headers.
    $options['headers'] += array(
      'User-Agent' => 'CMP (+http://capturemypage.com/)',
    );

    // stream_socket_client() requires timeout to be a float.
    $options['timeout'] = (float) $options['timeout'];

    // Use a proxy if one is defined and the host is not on the excluded list.
    $proxy_server = '';
    if ($proxy_server && $this->_http_use_proxy($uri['host'])) {
      // Set the scheme so we open a socket to the proxy server.
      $uri['scheme'] = 'proxy';
      // Set the path to be the full URL.
      $uri['path'] = $url;
      // Since the URL is passed as the path, we won't use the parsed query.
      unset($uri['query']);

      // Add in username and password to Proxy-Authorization header if needed.
      if ($proxy_username = '') {
        $proxy_password = '';
        $options['headers']['Proxy-Authorization'] = 'Basic ' . base64_encode($proxy_username . (!empty($proxy_password) ? ":" . $proxy_password : ''));
      }
      // Some proxies reject requests with any User-Agent headers, while others
      // require a specific one.
      $proxy_user_agent = '';
      // The default value matches neither condition.
      if ($proxy_user_agent === NULL) {
        unset($options['headers']['User-Agent']);
      }
      elseif ($proxy_user_agent) {
        $options['headers']['User-Agent'] = $proxy_user_agent;
      }
    }

    switch ($uri['scheme']) {
      case 'proxy':
        // Make the socket connection to a proxy server.
        $socket = 'tcp://' . $proxy_server . ':' . 8080;
        // The Host header still needs to match the real request.
        $options['headers']['Host'] = $uri['host'];
        $options['headers']['Host'] .= isset($uri['port']) && $uri['port'] != 80 ? ':' . $uri['port'] : '';
        break;

      case 'http':
      case 'feed':
        $port = isset($uri['port']) ? $uri['port'] : 80;
        $socket = 'tcp://' . $uri['host'] . ':' . $port;
        // RFC 2616: "non-standard ports MUST, default ports MAY be included".
        // We don't add the standard port to prevent from breaking rewrite rules
        // checking the host that do not take into account the port number.
        $options['headers']['Host'] = $uri['host'] . ($port != 80 ? ':' . $port : '');
        break;

      case 'https':
        // Note: Only works when PHP is compiled with OpenSSL support.
        $port = isset($uri['port']) ? $uri['port'] : 443;
        $socket = 'ssl://' . $uri['host'] . ':' . $port;
        $options['headers']['Host'] = $uri['host'] . ($port != 443 ? ':' . $port : '');
        break;

      default:
        $result->error = 'invalid schema ' . $uri['scheme'];
        $result->code = -1003;
        return $result;
    }

    if (empty($options['context'])) {
      $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout']);
    }
    else {
      // Create a stream with context. Allows verification of a SSL certificate.
      $fp = @stream_socket_client($socket, $errno, $errstr, $options['timeout'], STREAM_CLIENT_CONNECT, $options['context']);
    }

    // Make sure the socket opened properly.
    if (!$fp) {
      // When a network error occurs, we use a negative number so it does not
      // clash with the HTTP status codes.
      $result->code = -$errno;
      $result->error = trim($errstr) ? trim($errstr) : 'Error opening socket ' . $socket;

      return $result;
    }

    // Construct the path to act on.
    $path = isset($uri['path']) ? $uri['path'] : '/';
    if (isset($uri['query'])) {
      $path .= '?' . $uri['query'];
    }

    // Only add Content-Length if we actually have any content or if it is a POST
    // or PUT request. Some non-standard servers get confused by Content-Length in
    // at least HEAD/GET requests, and Squid always requires Content-Length in
    // POST/PUT requests.
    $content_length = strlen($options['data']);
    if ($content_length > 0 || $options['method'] == 'POST' || $options['method'] == 'PUT') {
      $options['headers']['Content-Length'] = $content_length;
    }

    // If the server URL has a user then attempt to use basic authentication.
    if (isset($uri['user'])) {
      $options['headers']['Authorization'] = 'Basic ' . base64_encode($uri['user'] . (isset($uri['pass']) ? ':' . $uri['pass'] : ':'));
    }

    $request = $options['method'] . ' ' . $path . " HTTP/1.0\r\n";
    foreach ($options['headers'] as $name => $value) {
      $request .= $name . ': ' . trim($value) . "\r\n";
    }
    $request .= "\r\n" . $options['data'];
    $result->request = $request;
    // Calculate how much time is left of the original timeout value.
    $timeout = $options['timeout'] - $this->_timer_read(__FUNCTION__) / 1000;
    if ($timeout > 0) {
      stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
      fwrite($fp, $request);
    }

    // Fetch response. Due to PHP bugs like http://bugs.php.net/bug.php?id=43782
    // and http://bugs.php.net/bug.php?id=46049 we can't rely on feof(), but
    // instead must invoke stream_get_meta_data() each iteration.
    $info = stream_get_meta_data($fp);
    $alive = !$info['eof'] && !$info['timed_out'];
    $response = '';

    while ($alive) {
      // Calculate how much time is left of the original timeout value.
      $timeout = $options['timeout'] - $this->_timer_read(__FUNCTION__) / 1000;
      if ($timeout <= 0) {
        $info['timed_out'] = TRUE;
        break;
      }
      stream_set_timeout($fp, floor($timeout), floor(1000000 * fmod($timeout, 1)));
      $chunk = fread($fp, 1024);
      $response .= $chunk;
      $info = stream_get_meta_data($fp);
      $alive = !$info['eof'] && !$info['timed_out'] && $chunk;
    }
    fclose($fp);

    if ($info['timed_out']) {
      $result->code = -1;
      $result->error = 'request timed out';
      return $result;
    }
    // Parse response headers from the response body.
    // Be tolerant of malformed HTTP responses that separate header and body with
    // \n\n or \r\r instead of \r\n\r\n.
    list($response, $result->data) = preg_split("/\r\n\r\n|\n\n|\r\r/", $response, 2);
    $response = preg_split("/\r\n|\n|\r/", $response);

    // Parse the response status line.
    $response_status_array = $this->_parse_response_status(trim(array_shift($response)));
    $result->protocol = $response_status_array['http_version'];
    $result->status_message = $response_status_array['reason_phrase'];
    $code = $response_status_array['response_code'];

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
      case 200: // OK
      case 304: // Not modified
        break;
      case 301: // Moved permanently
      case 302: // Moved temporarily
      case 307: // Moved temporarily
        $location = $result->headers['location'];
        $options['timeout'] -= $this->_timer_read(__FUNCTION__) / 1000;
        if ($options['timeout'] <= 0) {
          $result->code = -1;
          $result->error = 'request timed out';
        }
        elseif ($options['max_redirects']) {
          // Redirect to the new location.
          $options['max_redirects']--;
          $result = $this->_http_request($location, $options);
          $result->redirect_code = $code;
        }
        if (!isset($result->redirect_url)) {
          $result->redirect_url = $location;
        }
        break;
      default:
        $result->error = $result->status_message;
    }

    return $result;
  }

  /**
   * Splits an HTTP response status line into components.
   *
   * See the @link http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html status
   * line definition @endlink in RFC 2616.
   *
   * @param string $response
   *   The response status line, for example 'HTTP/1.1 500 Internal Server Error'.
   *
   * @return array
   *   Keyed array containing the component parts. If the response is malformed,
   *   all possible parts will be extracted. 'reason_phrase' could be empty.
   *   Possible keys:
   *   - 'http_version'
   *   - 'response_code'
   *   - 'reason_phrase'
   */
  function _parse_response_status($response) {
    $response_array = explode(' ', trim($response), 3);
    // Set up empty values.
    $result = array(
      'reason_phrase' => '',
    );
    $result['http_version'] = $response_array[0];
    $result['response_code'] = $response_array[1];
    if (isset($response_array[2])) {
      $result['reason_phrase'] = $response_array[2];
    }
    return $result;
  }

  /**
   * Helper function for determining hosts excluded from needing a proxy.
   *
   * @return string $host
   *   TRUE if a proxy should be used for this host.
   */
  function _http_use_proxy($host) {
    $proxy_exceptions = array('localhost', '127.0.0.1');
    return !in_array(strtolower($host), $proxy_exceptions, TRUE);
  }

  /**
   * Starts the timer with the specified name.
   *
   * If you start and stop the same timer multiple times, the measured intervals
   * will be accumulated.
   *
   * @param $name
   *   The name of the timer.
   */
  function _timer_start($name) {
    global $timers;

    $timers[$name]['start'] = microtime(TRUE);
    $timers[$name]['count'] = isset($timers[$name]['count']) ? ++$timers[$name]['count'] : 1;
  }

  /**
   * Reads the current timer value without stopping the timer.
   *
   * @param $name
   *   The name of the timer.
   *
   * @return string
   *   The current timer value in ms.
   */
  function _timer_read($name) {
    global $timers;

    if (isset($timers[$name]['start'])) {
      $stop = microtime(TRUE);
      $diff = round(($stop - $timers[$name]['start']) * 1000, 2);

      if (isset($timers[$name]['time'])) {
        $diff += $timers[$name]['time'];
      }
      return $diff;
    }

    return $timers[$name]['time'];
  }

  /**
   * @param $var
   * @return string
   */
  function _json_encode($var) {
    // The PHP version cannot change within a request.
    static $php530;

    if (!isset($php530)) {
      $php530 = version_compare(PHP_VERSION, '5.3.0', '>=');
    }

    if ($php530) {
      // Encode <, >, ', &, and " using the json_encode() options parameter.
      return json_encode($var, JSON_HEX_TAG | JSON_HEX_APOS | JSON_HEX_AMP | JSON_HEX_QUOT);
    }

    return $this->_json_encode_helper($var);
  }

  /**
   * @param $var
   * @return string
   */
  function _json_encode_helper($var) {
    switch (gettype($var)) {
      case 'boolean':
        return $var ? 'true' : 'false'; // Lowercase necessary!

      case 'integer':
      case 'double':
        return $var;

      case 'resource':
      case 'string':
        // Always use Unicode escape sequences (\u0022) over JSON escape
        // sequences (\") to prevent browsers interpreting these as
        // special characters.
        $replace_pairs = array(

          // ", \ and U+0000 - U+001F must be escaped according to RFC 4627.
          '\\'           => '\u005C',
          '"'            => '\u0022',
          "\x00"         => '\u0000',
          "\x01"         => '\u0001',
          "\x02"         => '\u0002',
          "\x03"         => '\u0003',
          "\x04"         => '\u0004',
          "\x05"         => '\u0005',
          "\x06"         => '\u0006',
          "\x07"         => '\u0007',
          "\x08"         => '\u0008',
          "\x09"         => '\u0009',
          "\x0a"         => '\u000A',
          "\x0b"         => '\u000B',
          "\x0c"         => '\u000C',
          "\x0d"         => '\u000D',
          "\x0e"         => '\u000E',
          "\x0f"         => '\u000F',
          "\x10"         => '\u0010',
          "\x11"         => '\u0011',
          "\x12"         => '\u0012',
          "\x13"         => '\u0013',
          "\x14"         => '\u0014',
          "\x15"         => '\u0015',
          "\x16"         => '\u0016',
          "\x17"         => '\u0017',
          "\x18"         => '\u0018',
          "\x19"         => '\u0019',
          "\x1a"         => '\u001A',
          "\x1b"         => '\u001B',
          "\x1c"         => '\u001C',
          "\x1d"         => '\u001D',
          "\x1e"         => '\u001E',
          "\x1f"         => '\u001F',
          // Prevent browsers from interpreting these as as special.
          "'"            => '\u0027',
          '<'            => '\u003C',
          '>'            => '\u003E',
          '&'            => '\u0026',
          // Prevent browsers from interpreting the solidus as special and
          // non-compliant JSON parsers from interpreting // as a comment.
          '/'            => '\u002F',
          // While these are allowed unescaped according to ECMA-262, section
          // 15.12.2, they cause problems in some JSON parsers.
          "\xe2\x80\xa8" => '\u2028', // U+2028, Line Separator.
          "\xe2\x80\xa9" => '\u2029', // U+2029, Paragraph Separator.
        );

        return '"' . strtr($var, $replace_pairs) . '"';

      case 'array':
        // Arrays in JSON can't be associative. If the array is empty or if it
        // has sequential whole number keys starting with 0, it's not associative
        // so we can go ahead and convert it as an array.
        if (empty($var) || array_keys($var) === range(0, sizeof($var) - 1)) {
          $output = array();
          foreach ($var as $v) {
            $output[] = $this->_json_encode_helper($v);
          }
          return '[ ' . implode(', ', $output) . ' ]';
        }
        break;

      // Otherwise, fall through to convert the array as an object.
      case 'object':
        $output = array();
        foreach ($var as $k => $v) {
          $output[] = $this->_json_encode_helper(strval($k)) . ':' . $this->_json_encode_helper($v);
        }
        return '{' . implode(', ', $output) . '}';

      default:
        return 'null';
    }
  }
}
