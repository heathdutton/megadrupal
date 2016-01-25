<?php

/**
 * Class DropSharkRequestDefault.
 */
class DropSharkRequestDefault implements DropSharkRequest {

  /**
   * {@inheritdoc}
   */
  public function getHost() {
    return variable_get('dropshark_host', DROPSHARK_HOST_DEFAULT);
  }

  /**
   * {@inheritdoc}
   */
  public function get($endpoint, $data = array()) {
    $url = $this->getHost() . '/' . $endpoint;

    if ($data) {
      $url .= '&' . drupal_http_build_query($data);
    }

    return $this->execute($url);
  }

  /**
   * {@inheritdoc}
   */
  public function post($endpoint, $data = array()) {
    $url = $this->getHost() . '/' . $endpoint;
    $options['method'] = 'POST';
    $options['headers']['Content-Type'] = 'application/x-www-form-urlencoded';

    if ($data) {
      $options['data'] = drupal_http_build_query($data);
    }

    return $this->execute($url, $options);
  }

  /**
   * Execute the HTTP request.
   *
   * @param $url
   *   The URL which is being requested.
   * @param array $options
   *   An array of options to send to drupal_http_request().
   *
   * @return object
   *   The result from drupal_http_request().
   */
  protected function execute($url, $options = array()) {
    $options = $this->setAuth($options);
    $result = drupal_http_request($url, $options);
    if (isset($result->data)) {
      $result->data = json_decode($result->data);
    }
    return $result;
  }

  /**
   * Sets the site token into the request header.
   *
   * @param $options
   *   The drupal_http_request() $options array.
   *
   * @return mixed
   *   The resulting drupal_http_request() $options array.
   */
  protected function setAuth($options) {
    if ($token = variable_get('dropshark_site_token')) {
      $options['headers']['Authorization'] = $token;
    }

    return $options;
  }

}
