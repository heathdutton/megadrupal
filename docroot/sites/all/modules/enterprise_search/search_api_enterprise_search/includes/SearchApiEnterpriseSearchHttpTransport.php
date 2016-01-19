<?php

/**
 * @file
 * HTTP transport class for Solr.
 */

/**
 * HTTP transport for connections to the Acquia Search Service.
 */
class SearchApiEnterpriseSearchHttpTransport extends SearchApiSolrHttpTransport {

  /**
   * Modify a solr base url and construct authenticator header.
   *
   * @param $url
   *  The solr url beng requested - passed by reference and may be altered.
   * @param $string
   *  A string - the data to be authenticated, or empty to just use the path
   *  and query from the url to build the authenticator.
   * @param $derived_key
   *  Optional string to supply the derived key.
   *
   * @return
   *  An array containing the string to be added as the content of the
   *  Cookie header.
   * 
   * @see enterprise_search_auth_headers
   * 
   */
  function authHeaders(&$url, $string = '', $derived_key = NULL) {
    return enterprise_search_auth_headers($url, $string, $scheme);
  }

  /**
   * Modify the url and add headers appropriate to authenticate.
   */
  public function prepareRequest(&$url, &$options, $use_data = TRUE) {
    if ($use_data && isset($options['data'])) {
      $options['headers'] = $this->authHeaders($url, $options['data']);
    }
    else {
      $options['headers'] = $this->authHeaders($url);
    }
    if (empty($options['headers'])) {
      throw new Exception('Invalid authentication string.');
    }
    $options['User-Agent'] = 'a12_find/'. A12_FIND_VERSION;
    return TRUE;
  }

  /**
   * Validate the hmac for the response body.
   *
   * @return
   *  The response object.
   */
  public function authenticateResponse($response, $nonce, $url) {
//    $hmac = $this->extractHmac($response->headers);
//    if (!$this->validResponse($hmac, $nonce, $response->data)) {
//      throw new Exception('Authentication of search content failed url: '. $url);
//    }
    return $response;
  }

  /**
   * Look in the headers and get the hmac_digest out
   * @return string hmac_digest
   *
   */
  protected function extractHmac($headers) {
    $reg = array();
    if (is_array($headers)) {
      foreach ($headers as $name => $value) {
        if (strtolower($name) == 'pragma' && preg_match("/hmac_digest=([^;]+);/i", $value, $reg)) {
          return trim($reg[1]);
        }
      }
    }
    return '';
  }

  /**
   * Validate the authenticity of returned data using a nonce and HMAC-SHA1.
   *
   * @return
   *  TRUE or FALSE.
   */
  protected function validResponse($hmac, $nonce, $string, $derived_key = NULL) {
    if (empty($derived_key)) {
      $derived_key = $this->derivedKey();
    }
    return $hmac == hash_hmac('sha1', $nonce . $string, $derived_key);
  }

  /**
   * Helper method for making an HTTP request.
   */
  protected function performHttpRequest($method, $url, $timeout, $rawPost = NULL, $contentType = NULL) {
    $options = array(
      'method' => $method,
      'timeout' => $timeout && $timeout > 0 ? $timeout : $this->getDefaultTimeout(),
      'headers' => array(),
    );

    if ($this->http_auth) {
      $options['headers']['Authorization'] = $this->http_auth;
    }
    if ($timeout) {
      $options['timeout'] = $timeout;
    }
    if ($rawPost) {
      $options['data'] = $rawPost;
    }
    if ($contentType) {
      $options['headers']['Content-Type'] = $contentType;
    }

    $this->prepareRequest($url, $options);
    $response = drupal_http_request($url, $options);
    $type = isset($response->headers['content-type']) ? $response->headers['content-type'] : 'text/xml';
    $body = isset($response->data) ? $response->data : NULL;
    return new Apache_Solr_HttpTransport_Response($response->code, $type, $body);
  }
}
