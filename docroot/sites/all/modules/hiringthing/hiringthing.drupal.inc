<?php

/**
 * A sub-class of HiringThingHttpRequester that uses drupal_http_request().
 */
class HiringThingHttpRequesterDrupal extends HiringThingHttpRequester {
  protected function make_request($url, $data = NULL, $method = 'GET', $headers = array()) {
    $response = drupal_http_request($url, array(
      'data' => $data,
      'method' => $method,
      'headers' => $headers,
    ));

    // TODO: handle failure somehow!

    return drupal_json_decode($response->data);
  }
}

