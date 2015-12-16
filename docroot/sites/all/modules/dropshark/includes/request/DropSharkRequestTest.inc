<?php

/**
 * Class DropSharkRequestTest.
 */
class DropSharkRequestTest implements DropSharkRequest {

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
    $result = new stdClass();
    $result->data = drupal_http_build_query($data);
    return $result;
  }

}
