<?php

class MpxApi {

  /**
   * Make an authenticated mpx thePlatform API HTTP request.
   *
   * @param MpxAccount $account
   *   The mpx account making the request.
   * @param string $url
   *   The URL of the API request.
   * @param array $params
   *   (optional) An array of query parameters to add to the request.
   * @param array $options
   *   (optional) An array of additional options to pass through to
   *   drupal_http_request().
   *
   * @return mixed
   *   The data from the request if successful.
   *
   * @throws MpxApiException
   * @throws MpxHttpException
   *
   * @todo Rename to tokenRequest()
   */
  public static function authenticatedRequest(MpxAccount $account, $url, array $params = array(), array $options = array()) {
    try {
      $duration = isset($options['timeout']) ? $options['timeout'] : NULL;
      $params['token'] = $account->acquireToken($duration);
      return static::request($url, $params, $options);
    }
    catch (MpxApiException $exception) {
      // If the token is invalid, we should delete it from storage so that a
      // fresh token is fetched on the next request.
      if ($exception->getCode() == 401) {
        // Flag the token as expired so it will not be reused.
        $params['token']->expire = NULL;
        // Ensure the token will be deleted.
        drupal_register_shutdown_function(array($params['token'], 'delete'));
      }
      throw $exception;
    }
  }

  /**
   * Make an mpx thePlatform API HTTP request.
   *
   * @param $url
   *   The URL of the API request.
   * @param array $params
   *   (optional) An array of query parameters to add to the request.
   * @param array $options
   *   (optional) An array of additional options to pass through to
   *   drupal_http_request().
   *
   * @return mixed
   *   The data from the request if successful.
   *
   * @throws MpxHttpException
   */
  public static function request($url, array $params = array(), array $options = array()) {
    // If the URL already contains a query string, let's merge it into the
    // $params array so that it can easily be altered.
    if ($query = parse_url($url, PHP_URL_QUERY)) {
      $url = str_replace('?' . $query, '', $url);
      $params = drupal_get_query_array($query) + $params;
    }

    // Allow for altering the URL before making the request.
    drupal_alter('media_theplatform_mpx_api_request', $url, $params, $options);

    // Append the query parameters back onto the URL.
    if (!empty($params)) {
      $query = drupal_http_build_query($params);
      $url .= (strpos($url, '?') !== FALSE ? '&' : '?') . $query;
    }

    // Also invoke the deprecated hook now that the full URL has been built.
    // @todo Remove this hook invocation.
    drupal_alter('media_theplatform_mpx_feed_request', $url, $options);

    if (isset($options['method']) && $options['method'] === 'POST' && isset($options['data']) && is_array($options['data'])) {
      $options['data'] = http_build_query($options['data']);
    }

    $time = microtime(TRUE);
    $response = drupal_http_request($url, $options);
    $response->time_elapsed = microtime(TRUE) - $time;
    $response->url = $url;
    $response->params = $params;

    media_theplatform_mpx_debug($response, "MPX API request to " . preg_replace('/\?.*/', '', $url));

    if (!empty($response->error)) {
      throw new MpxHttpException("Error $response->code on request to $url: $response->error", (int) $response->code);
    }
    elseif (empty($response->data)) {
      throw new MpxApiException("Empty response from request to $url.");
    }

    if (isset($response->headers['content-type']) && preg_match('~^(application|text)/json~', $response->headers['content-type'])) {
      return static::processJsonResponse($response);
    }

    return $response->data;
  }

  /**
   * Process the data from an API response which contains JSON.
   *
   * @param object $response
   *   The response object from drupal_http_request().
   *
   * @return array
   *   The JSON-decoded array of data on success.
   *
   * @throws MpxHttpException
   * @throws MpxApiException
   */
  public static function processJsonResponse($response) {
    $data = json_decode($response->data, TRUE, 256);
    if ($data === NULL && json_last_error() !== JSON_ERROR_NONE) {
      if (function_exists('json_last_error_msg')) {
        throw new MpxHttpException("Unable to decode JSON response from request to {$response->url}: " . json_last_error_msg());
      }
      else {
        throw new MpxHttpException("Unable to decode JSON response from request to {$response->url}");
      }
    }

    $response->data = $data;
    if (!empty($data['responseCode']) && !empty($data['isException'])) {
      throw new MpxApiException("Error {$data['title']} on request to {$response->url}: {$data['description']}", (int) $data['responseCode']);
    }
    elseif (!empty($data[0]['entry']['responseCode']) && !empty($data[0]['entry']['isException'])) {
      throw new MpxApiException("Error {$data[0]['entry']['title']} on request to {$response->url}: {$data[0]['entry']['description']}", (int) $data[0]['entry']['responseCode']);
    }
    else {
      return $data;
    }
  }

}

class MpxHttpException extends MpxException { }

class MpxApiException extends MpxHttpException { }
