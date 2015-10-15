<?php
/**
 * @file
 * The include file for the linkedin_company module.
 */

  /**
   * Completes a LinkedIn API request and returns the response.
   *
   * @param string $url
   *   The url to be downloaded.
   * @param string $body
   *   The content to be sent with the request.
   * @param string $request_type
   *   The type of request to be sent. Used to set appropriate headers.
   *
   * @return bool|mixed
   *   The response from the request on success, FALSE on failure.
   */
function linkedin_company_http_request($url, $body = '', $request_type = '') {
  $options = array();

  if ($request_type == 'token_request') {
    $options['method'] = 'POST';
  }
  else {
    $options['headers']['Content-Type'] = 'text/xml;charset=utf-8';
  }

  if ($body) {
    $options['body'] = $body;
  }

  $response = drupal_http_request($url, $options);

  if (isset($response->error) && $response->code != '200') {
    // Some kind of an error happened.
    watchdog('linkedin_company', 'When trying to access the LinkedIn Company API the following error occurred: @http_code | @error', array(
    '@http_code' => $response->code,
    '@error' => $response->error,
    ), WATCHDOG_ERROR);

    return FALSE;
  }
  else {
    return $response->data;
  }

}
