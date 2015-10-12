<?php

/**
 * @file
 * Service call routines.
 */

/**
 * Subscription service endpoint URL.
 * @var string
 */
define(
  'AFFILIATE_STORE_SUBSCRIPTION_API_URL',
  AFFILIATE_STORE_API_URL . '/subscriptions'
);

/**
 * Response code when MD5 checksum mismatched.
 * @var int
 */
define('AFFILIATE_STORE_MD5_MISMATCH', -1010);
/**
 * Response code when gzip decoding failed.
 * @var int
 */
define('AFFILIATE_STORE_GZIP_DECODE_ERROR', -1020);
/**
 * Response code when JSON decoding failed.
 * @var int
 */
define('AFFILIATE_STORE_JSON_DECODE_ERROR', -1021);

/**
 * Revision number too old.
 * @var int
 */
define('EARNHIGHWAY_REV_OLD', 502);
/**
 * Index revision number too old.
 * @var int
 */
define('EARNHIGHWAY_IDX_REV_OLD', 503);

/**
 * Get affiliate store subscription list.
 *
 * @param array $params
 *   (optional) An associative array of service parameter name and value pairs.
 *   Default to empty.
 *
 * @return object
 *   Response object.
 */
function affiliate_store_service_subscription_index($params = array()) {
  $options = array(
    'service' => 'affiliate_store_subscription.index',
    'url' => AFFILIATE_STORE_SUBSCRIPTION_API_URL,
    'params' => $params,
  );
  return affiliate_store_service_call($options);
}

/**
 * Get an affiliate store subscription.
 *
 * @param mixed $id
 *   Subscription ID or an absolute URL to subscription resource.
 * @param array $params
 *   (optional) An associative array of service parameter name and value pairs.
 *   Default to empty.
 *
 * @return object
 *   Response object.
 */
function affiliate_store_service_subscription_retrieve($id, $params = array()) {
  $url = AFFILIATE_STORE_SUBSCRIPTION_API_URL . "/$id";
  $options = array(
    'service' => 'affiliate_store_subscription.retrieve',
    'url' => preg_match('#^https?://#', $id) ? $id : $url,
    'params' => $params,
  );
  return affiliate_store_service_call($options);
}

/**
 * Make a service call.
 *
 * @param array $options
 *   An associative array used to specify service call options with the
 *   following keys:
 *   - service: string Service method name.
 *   - url: string Service URL.
 *   - params: array (optional) An associative array of service parameter
 *     name and value pairs. Default to empty.
 *   - headers: array (optional) An associative array of HTTP header and value
 *     pairs. Default to Accept => application/json, Accept-Encoding => gzip.
 *   - method: string (optional) HTTP request method. Default to 'GET'.
 *   - data: string (optional) Data to include in the request. Default to NULL.
 *   - auth: boolean (optional) Whether service call needs authentication.
 *     Default to TRUE.
 *
 * @return object
 *   Response object.
 */
function affiliate_store_service_call($options) {
  // Set default values.
  $options += array(
    'params' => array(),
    'headers' => array(
      'Accept' => 'application/json',
      'Accept-Encoding' => 'gzip',
    ),
    'method' => 'GET',
    'data' => NULL,
    'auth' => TRUE,
  );

  // Add HTTP Authorization header if authentication is required.
  if ($options['auth']) {
    $header = affiliate_store_service_create_auth_header($options['service']);
    $options['headers']['Authorization'] = $header;
  }

  $response = drupal_http_request(
    url($options['url'], array('query' => $options['params'])),
    array(
      'headers' => $options['headers'],
      'method' => $options['method'],
      'data' => $options['data'],
    )
  );

  affiliate_store_include('common');
  // Uncompress data.
  if ($response->data && isset($response->headers['content-encoding'])) {
    $header = drupal_strtolower($response->headers['content-encoding']);
    if ($header === 'gzip') {
      $response->data = affiliate_store_gzdecode($response->data);
      if ($response->data === NULL) {
        $response->error = t('Cannot decode gzip data in service response');
        $response->code = AFFILIATE_STORE_GZIP_DECODE_ERROR;
        return $response;
      }
    }
  }
  // Check response payload integrity.
  if (isset($response->headers['content-md5'])) {
    $checksum = base64_encode(pack('H*', md5($response->data)));
    if ($checksum !== $response->headers['content-md5']) {
      $response->error = t('Service response checksum mismatched');
      $response->code = AFFILIATE_STORE_MD5_MISMATCH;
      return $response;
    }
  }
  // Decode data.
  if ($response->data && isset($response->headers['content-type'])) {
    $header = drupal_strtolower($response->headers['content-type']);
    if (strpos($header, 'application/json') !== FALSE) {
      $response->data = affiliate_store_json_decode($response->data, $error);
      if ($error) {
        $response->error = t('Cannot decode JSON data in service response');
        $response->code = AFFILIATE_STORE_JSON_DECODE_ERROR;
        return $response;
      }
    }
  }
  return $response;
}

/**
 * Create service authentication parameters.
 *
 * @param string $service
 *   Service method name.
 *
 * @return array
 *   An associative array with the following keys:
 *   - signature
 *   - domain
 *   - timestamp
 *   - nonce
 */
function affiliate_store_service_create_auth_params($service) {
  affiliate_store_include('common');
  $timestamp = (string) time();
  $domain = affiliate_store_id();
  $nonce = user_password(32);
  return array(
    'signature' => affiliate_store_hash_hmac(
      'sha256',
      implode(';', array($timestamp, $domain, $nonce, $service)),
      variable_get('affiliate_store_api_key', '')
    ),
    'domain' => $domain,
    'timestamp' => $timestamp,
    'nonce' => $nonce,
  );
}

/**
 * Create service HTTP Authorization header.
 *
 * @param string $service
 *   Service method name.
 *
 * @return string
 *   HTTP Authorization header value.
 *
 * @see affiliate_store_service_create_auth_params()
 */
function affiliate_store_service_create_auth_header($service) {
  $params = affiliate_store_service_create_auth_params($service);
  ksort($params);
  $pairs = array();
  foreach ($params as $key => $value) {
    $pairs[] = $key . '="' . $value . '"';
  }
  return 'KeyAuth ' . implode(', ', $pairs);
}

/**
 * General service error handler.
 *
 * This is the default error handler that should be called when specific error
 * handler fails to catch or handle the error cause.
 *
 * @param object $response
 *   Service response object from the call that results in error.
 * @param array &$errors
 *   A reference to an array for returning error messages. Each error will be
 *   logged by watchdog automatically.
 */
function affiliate_store_service_error($response, &$errors) {
  // Ignore errors that are either temporary or depend on circumstances.
  $ignore = array(
    // Not really an error, it means OK for conditional GET.
    304,
    // Service (temporarily) unavailable, rate limiting, or service maintenance.
    503,
    // Assume temporary transmission error.
    AFFILIATE_STORE_MD5_MISMATCH,
  );

  if (in_array($response->code, $ignore)) {
    // Ignore.
  }
  elseif (($response->code >= 100) && ($response->code < 200)) {
    // Not used.
  }
  elseif (($response->code >= 200) && ($response->code < 300)) {
    // Response seems OK.
  }
  elseif (($response->code >= 300) && ($response->code < 400)) {
    // Not used, or no need to handle redirection.
  }
  elseif (($response->code >= 400) && ($response->code < 500)) {
    // Client error.
    watchdog(
      'affiliate store',
      'Client error: @error. Contact the module author if problem persist. ' .
      '!debug',
      array(
        '@error' => $response->error,
        '!debug' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>',
      ),
      WATCHDOG_ERROR
    );

    // Collect error details if included in response.
    if (!empty($response->data['errors'])) {
      foreach ($response->data['errors'] as $error) {
        $message = $error['message'];
        $message .= !empty($error['details']) ? ': ' . $error['details'] : '';
        $errors[] = check_plain($message);
      }
    }
    else {
      $errors[] = check_plain($response->error);
    }
  }
  elseif (($response->code >= 500) && ($response->code < 600)) {
    // Server error.
    watchdog(
      'affiliate store',
      'Hub server error: @error. Contact !hub if problem persist. !debug',
      array(
        '@error' => $response->error,
        '!hub' => AFFILIATE_STORE_HUB,
        '!debug' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>',
      ),
      WATCHDOG_ERROR,
      l(
        AFFILIATE_STORE_HUB, AFFILIATE_STORE_HUB_URL,
        array('attributes' => array('target' => '_blank'))
      )
    );
    $errors[] = check_plain($response->error);
  }
  elseif (($response->code <= 0) && ($response->code > -1000)) {
    // This should corresponse to low level system error codes as converted in
    // drupal_http_request(). Assume temporary and ignore.
  }
  elseif ($response->code <= -1000) {
    // Module error. Custom error codes used by Drupal and this module.
    watchdog(
      'affiliate store',
      'Module error: @error. Double check your Affiliate Store module ' .
      'setup and settings to ensure they are correct (see module ' .
      'README.txt). Contact the module author if problem persist. !debug',
      array(
        '@error' => $response->error,
        '!debug' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>',
      ),
      WATCHDOG_ERROR,
      l(t('settings'), 'admin/structure/affiliate-store')
    );
    $errors[] = check_plain($response->error);
  }
}
