<?php

/**
 * Interface DropSharkRequest.
 */
interface DropSharkRequest {

  /**
   * Get the hostname of the DropShark server.
   *
   * @return string
   *   The hostname of the DropShark server.
   */
  public function getHost();

  /**
   * Get data from DropShark.
   *
   * @param $endpoint
   *   The endpoint to get data.
   * @param array $data
   *   Data to be sent in the request.
   *
   * @return stdClass
   */
  public function get($endpoint, $data = array());

  /**
   * Post data to DropShark.
   *
   * @param $endpoint
   *   The endpoint to post data.
   * @param array $data
   *   Data to be sent in the request.
   *
   * @return stdClass
   */
  public function post($endpoint, $data = array());

}
