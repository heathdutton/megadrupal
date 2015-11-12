<?php
/**
 * @file
 * Contains wrapper code for a Bazaarvoice object.
 */

/**
 * Class Bazaarvoice.
 */
class Bazaarvoice {

  private static $api_version = '5.4';
  private $pass_key;
  private $domain;
  private $errors;

  /**
   * Constructor method.
   */
  public function __construct() {
    if ($pass_key = variable_get('bazaarvoice_pass_key', FALSE)) {
      $this->pass_key = $pass_key;
    }

    $this->domain = 'http://' . (variable_get('bazaarvoice_mode', 'stg') == 'stg' ? 'stg.' : '') . 'api.bazaarvoice.com';
  }

  /**
   * Return the API version being used.
   *
   * @return string
   *   The API version.
   */
  public static function apiVersion() {
    return self::$api_version;
  }

  /**
   * Return errors from last API call.
   *
   * @return mixed
   *   Array of errors.
   */
  public function getApiErrors() {
    return $this->errors;
  }

  /**
   * Build an API endpoint url.
   *
   * @param string $endpoint
   *   API Endpoint.
   * @param array $additional_arguments
   *   Arguments to pass to the endpoint. key value pairs.
   *
   * @return bool|string
   *   Full API url or boolean FALSE.
   */
  private function buildUrl($endpoint, $additional_arguments = array()) {
    $url = FALSE;

    if ($this->pass_key) {
      // Define default arguments to pass.
      $arguments = array(
        'passkey=' . $this->pass_key,
        'ApiVersion=' . self::$api_version,
      );

      // Adding additional arguments?
      if (!empty($additional_arguments)) {
        // Loop through each to build correctly.
        foreach ($additional_arguments as $arg_name => $arg_value) {
          // If value is an array, then passing multiple versions of argument.
          if (is_array($arg_value)) {
            // Build each sub arguments.
            foreach ($arg_value as $sub_name => $sub_value) {
              // Add to arguments array.
              $arguments[] = $arg_name . '=' . $sub_name . ':' . $sub_value;
            }
          }
          // Single value for argument, add to arguments array.
          else {
            $arguments[] = $arg_name . '=' . $arg_value;
          }
        }
      }

      // Implode all of the arguments.
      $arguments = implode('&', $arguments);
      // Build the full domain.
      $url = $this->domain . '/data/' . $endpoint . '.json?' . $arguments;

    }
    else {
      watchdog('bazaarvoice', 'No passkey specified.');
    }

    return $url;
  }

  /**
   * Method makes an API call to Bazaarvoice.
   *
   * @param string $endpoint
   *   API endpoint to call.
   * @param array $arguments
   *   Arguments to pass to endpoint. A key value array.
   * @param string $method
   *   The method to use to make request. Either GET or POST.
   * @param int $cache_limit
   *   The time to cache the API request results.
   * @param array $request_options
   *   Addition options to send to the request (additional headers, etc...).
   *
   * @return mixed|null|string
   *   A Data object or null.
   */
  public function apiRequest($endpoint, $arguments = array(), $method = 'GET', $cache_limit = 60, $request_options = array()) {
    $data = NULL;

    // Build API url from endpoint.
    if ($request_url = $this->buildUrl($endpoint, $arguments)) {
      $cache_key = $request_url;
      // See if call was recently made.
      if ($cached = $this->getCachedData($cache_key)) {
        // Return the cached data.
        $data = $cached;
      }
      // Else, make an API request.
      else {
        // Build HTTP Request options.
        $options = array(
          'method' => ($method == 'POST' ? 'POST' : 'GET'),
          'timeout' => 60,
          'headers' => array(
            'Accept-Encoding' => 'gzip;q=0, deflate;q=0',
          ),
        );

        // Want to make a POST request?
        if ($options['method'] == 'POST') {
          // Need to split arguments from url and put in body data.
          list($request_url, $options['data']) = explode('?', $request_url);
          // Need to fake being a form because how http_request works.
          $options['headers'] = array('Content-Type' => 'application/x-www-form-urlencoded; charset=utf-8');
        }

        // Additional request options?
        if (!empty($request_options)) {
          $options = array_merge_recursive($options, $request_options);
        }

        // Make an HTTP Request.
        if ($result = drupal_http_request($request_url, $options)) {
          // Check headers for gzip.
          if (isset($result->headers['content-encoding']) && ($result->headers['content-encoding'] == 'gzip')) {
            // Deflate the data.
            $result->data = gzinflate(substr($result->data, 10, -8));
          }
          // A valid response returned?
          if ($result->code == 200) {
            $data = drupal_json_decode($result->data);
            $data['requestURL'] = $cache_key;
            // Store result into cache.
            $this->setCachedData($cache_key, $data, $cache_limit);
          }
          else {
            // Store errors.
            $this->errors = array($result->code => $result->error);
            // Log with watchdog.
            watchdog('bazaarvoice', '%code Error: %error , Request Url: %url', array(
              '%code' => $result->code,
              '%error' => $result->error,
              '%url' => $cache_key,
            ));
          }
        }
      }
    }

    // Were there errors?
    if (isset($data['HasErrors']) && $data['HasErrors']) {
      // Form errors?
      if (!empty($data['FormErrors'])) {
        $this->errors = $data['FormErrors'];
      }
      // Else general errors?
      elseif (!empty($data['Errors'])) {
        $this->errors = $data['Errors'];
      }
      // Set data to NULL.
      $data = NULL;
    }

    return $data;
  }

  /**
   * Method to retrieve cached data.
   *
   * @param string $key
   *   The unique string to use as a key.
   *
   * @return bool|mixed
   *   Cached data or boolean FALSE.
   */
  private function getCachedData($key) {
    $data = FALSE;
    // Generate a cache key hash.
    $cache_key = md5($key);
    // Attempt to retrieve from cache table.
    if ($cache = cache_get($cache_key, 'cache_bv')) {
      // Cached data retrieved. is it not expired?
      if (time() < $cache->expire) {
        // Decode cached data.
        $data = json_decode($cache->data);
      }
      // Expired, so remove from cache table.
      else {
        cache_clear_all($cache_key, 'cache_bv');
      }
    }

    return $data;
  }

  /**
   * Store data into cache.
   *
   * @param string $key
   *   The unique string to use as a key.
   * @param mixed $data
   *   The data to cache.
   * @param int $limit
   *   The time to store the data in cache.
   */
  private function setCachedData($key, $data, $limit = 60) {
    $cache_key = md5($key);
    cache_set($cache_key, $data, time() + $limit);
  }

}
