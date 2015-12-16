<?php

/**
 * Class DropSharkRequestDefault.
 */
class DropSharkRequestDefault implements DropSharkRequest {

  /**
   * {@inheritdoc}
   */
  public function getHost() {
    return DROPSHARK_HOST;
  }

  /**
   * {@inheritdoc}
   */
  public function post($endpoint, $data) {
    $url = $this->getHost() . '/' . $endpoint;
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
    );
    return drupal_http_request($url, $options);
  }

}
