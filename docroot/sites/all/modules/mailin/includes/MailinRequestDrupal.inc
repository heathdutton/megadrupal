<?php

/**
 * @file
 * Handle requests against Mailin server using Drupal HTTP request.
 */

use Mailin\Request as MR;
use Mailin\Response\Response;

/**
 * The Mailin Request Drupal class.
 *
 * This class uses Drupal HTTP request.
 */
class MailinRequestDrupal implements MR\RequestInterface {

  /**
   * @inheritdoc
   */
  public function request(array $data) {
    $options = array(
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
        'Expect:',
      ),
      'timeout' => 15.0,
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
    );

    $response = drupal_http_request(MR\Curl::MAILIN_URL, $options);
    return new Response($response->code == 200 ? $response->data : FALSE);
  }

}
