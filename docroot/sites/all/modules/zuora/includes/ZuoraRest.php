<?php

/**
 * Class Zuora.
 */
class ZuoraRest extends Zuora {
  /**
   *
   */
  const ZUORA_PRODUCTION = 'https://api.zuora.com/rest';

  /**
   *
   */
  const ZUORA_SANDBOX = 'https://apisandbox-api.zuora.com/rest';

  /**
   *
   */
  const ZUORA_API_VERSION = 'v1';

  /**
   * Singleton class instance.
   *
   * @var ZuoraRest
   */
  protected static $instance;

  /**
   * Singleton constructor for Zuora API.
   *
   * @return \ZuoraRest
   */
  public static function instance() {
    if (self::$instance === NULL) {
      self::$instance = new self();
    }

    return self::$instance;
  }

  /**
   * Perform GET request.
   *
   * @param $uri
   * @param array|NULL $body
   *
   * @return ZuoraResponse
   */
  public function get($uri) {
     return $this->httpRequest('GET', $uri);
  }

  /**
   * Perform POST request.
   *
   * @param $uri
   * @param array $body
   *
   * @return ZuoraResponse
   */
  public function post($uri, array $body) {
    return $this->httpRequest('POST', $uri, $body);
  }

  /**
   * Perform PUT request.
   *
   * @param $uri
   * @param array $body
   *
   * @return ZuoraResponse
   */
  public function put($uri, array $body) {
    return $this->httpRequest('PUT', $uri, $body);
  }

  /**
   * Builds a drupal_http_request call.
   *
   * @param $type
   * @param $uri
   * @param array|NULL $body
   *
   * @throws ZuoraException
   *
   * @return ZuoraResponse
   */
  public function httpRequest($type, $uri, array $body = null) {
    $response = new ZuoraResponse(drupal_http_request($this->getEndpointUrl($uri), array(
      'headers' => $this->httpHeaders(),
      'method' => $type,
      'data' => drupal_json_encode($body),
    )));

    if (!$response->successful()) {
      throw new ZuoraException(t('Zuora API error: @error', @array('@error' => $response->error())));
    }

    return $response;
  }

  /**
   * Returns HTTP request headers.
   *
   * @return array
   */
  protected function httpHeaders() {
    return array(
      'Accept' => 'application/json',
      'Content-Type' => 'application/json',
      'apiAccessKeyId' => $this->tenant_user_id,
      'apiSecretAccessKey' => $this->tenant_password,
    );
  }

  /**
   * Returns endpoint URL for REST API.
   *
   * @param $uri
   *    The API method to invoke.
   *
   * @return string
   */
  public function getEndpointUrl($uri) {
    return $this->getEndpointBaseUrl() . '/' . self::ZUORA_API_VERSION . $uri;
  }

  /**
   * Returns base URL for REST Endpoint.
   *
   * @return string
   */
  public function getEndpointBaseUrl() {
    if ($this->isSandboxed()) {
      return self::ZUORA_SANDBOX;
    }
    else {
      return self::ZUORA_PRODUCTION;
    }
  }

}
