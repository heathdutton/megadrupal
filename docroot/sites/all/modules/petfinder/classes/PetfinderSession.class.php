<?php
/**
 * @file
 *
 * Contains class definition for PetfinderSession, a utility class to create
 * and manage an API session with the Petfinder.com API service.
 */

class PetfinderSession {
  // Object properties
  var $token = NULL;
  var $api_key = NULL;
  var $secret_key = NULL;
  var $lastURL = NULL;

  // API return codes documented on http://www.petfinder.com/developers/api-docs
  const PFAPI_OK = 100;
  const PFAPI_ERR_INVALID = 200;
  const PFAPI_ERR_NOENT = 201;
  const PFAPI_ERR_LIMIT = 202;
  const PFAPI_ERR_LOCATION = 203;
  const PFAPI_ERR_UNAUTHORIZED = 300;
  const PFAPI_ERR_AUTHFAIL = 301;
  const PFAPI_ERR_INTERNAL = 999;

  /**
   * Creates a new session for subsequent API queries
   *
   * @param $api_key
   *   Developer API key for service calls
   * @param $secret_key
   *   Developer secret key for service calls
   * @param $signed
   *   Flag to determine whether or not a signed connection is used for queries against the service
   */
  public function init($api_key, $secret_key, $signed = FALSE) {
    // Screen args
    if (empty($api_key) || empty($secret_key)) {
      _petfinder_watchdog('Empty API or secret key value encountered, unable to initialize session', NULL, WATCHDOG_ERROR);
      return FALSE;
    }

    // Init properties
    $this->api_key = $api_key;
    $this->secret_key = $secret_key;
    $this->token = NULL;

    // If signed session is desired, get a token
    if ($signed) {
      $args = array(
        'key' => $api_key,
        'format' => 'json',
      );
      $args['sig'] = md5($this->secret_key . drupal_http_build_query($args));
      $resp = $this->call('auth.getToken', $args);
      if ($resp && !empty($resp['auth']['token'])) {
        $this->token = trim($resp['auth']['token']);
      }

      // Fail the open request if a signed connection was requested and we didn't get a valid session token
      if (empty($this->token)) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Main query method
   *
   * @param $method
   *   Petfinder API method, as defined on http://www.petfinder.com/developers/api-docs
   * @param $args
   *   Array of arguments to pass to the call
   *
   * @return
   *   Returns an array built from the JSON response received from the server,
   *   or FALSE if call to server could not be completed.  Note that the server
   *   might return a response even though an error occurred in the API call,
   *   so check for PFAPI_OK in the result array.
   */
  public function call($method, $args = array()) {
    // Build request URL
    $url = $this->_build_request_url($method, $args);
    //_petfinder_dpm($url);
    $this->lastURL = $url;

    // Check for cached data for all calls except the security token call
    if ('auth.getToken' != $method && $retval = $this->_cache($args)) {
      return $retval;
    }

    // Execute query
    $retval = FALSE;
    $resp = drupal_http_request($url);
    if (!empty($resp->data)) {
      if ($data = json_decode($resp->data, TRUE)) {
        $this->_clean_array($data);
        $retval = $data['petfinder'];
      }
      else {
        $json_err_msg = array(
          JSON_ERROR_NONE => t('No error has occurred'),
          JSON_ERROR_DEPTH => t('The maximum stack depth has been exceeded'),
          JSON_ERROR_STATE_MISMATCH => t('Invalid or malformed JSON'),
          JSON_ERROR_CTRL_CHAR => t('Control character error, possibly incorrectly encoded'),
          JSON_ERROR_SYNTAX => t('Syntax error'),
          JSON_ERROR_UTF8 => t('Malformed UTF-8 characters, possibly incorrectly encoded'),
        );
        $json_err_code = json_last_error();
        $err_msg = (isset($json_err_msg[$json_err_code])) ? $json_err_msg[$json_err_code] : t('Undocumented JSON decode error');
        _petfinder_watchdog('Unable to parse returned JSON data: %err', array('%err' => $err_msg), WATCHDOG_ERROR, $url);
      }
    }
    else {
      _petfinder_watchdog('Request to server failed', NULL, WATCHDOG_ERROR, $url);
    }

    // Store off response data in our page-level cache, except token requests
    if ('auth.getToken' != $method) {
      $this->_cache($args, $retval);
    }
    return $retval;
  }

  /**
   * Util function for building a Petfinder API request URL
   */
  protected function _build_request_url($method, $args) {
    // Tack on the API key and format if not already manually set
    $args['key'] = $this->api_key;
    $args['format'] = 'json';

    // If we've got a session token, generate the request signature using the secret key
    if (!empty($this->token)) {
      $args['token'] = $this->token;
      $args['sig'] = md5($this->secret_key . drupal_http_build_query($args));
    }

    // Build the URL from the final argument list
    $url = 'http://api.petfinder.com/' . $method . '?' . drupal_http_build_query($args);
    return $url;
  }

  /**
   * Util function to clean up arrays with strange indices
   */
  protected function _clean_array(&$data) {
    foreach ($data as $key => &$val) {
      if (empty($val)) {
        unset($data[$key]);
      }
      elseif (isset($val['$t'])) {
        $val = $val['$t'];
      }
      elseif (is_array($val)) {
        $this->_clean_array($val);
      }
    }

    return $data;
  }

  /**
   * Util function to manage query cache
   */
  protected function _cache($args, $data = NULL) {
    // Build hash key for this search to enable page-level caching of multiple searches
    // Sort on key, then serialize to try and maintain consistency across calls
    ksort($args);
    $cid = md5(serialize($args));

    // If we're given data, set it in the cache
    if ($data) {
      cache_set($cid, $data, 'cache_petfinder', CACHE_TEMPORARY);
    }

    // Otherwise execute a lookup
    else {
      $cache = cache_get($cid, 'cache_petfinder');
      if ($cache) {
        return $cache->data;
      }
    }

    return FALSE;
  }
};
