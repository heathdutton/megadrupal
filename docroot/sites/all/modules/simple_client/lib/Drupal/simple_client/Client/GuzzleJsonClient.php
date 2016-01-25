<?php
/**
 * @file
 * Contains a Simple Client specifically tailored for JSON requests.
 *
 * @copyright Copyright(c) 2012 Previous Next Pty Ltd
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at previousnext dot com dot au
 */

namespace Drupal\simple_client\Client;

/**
 * Class SimpleJsonClient
 *
 * @package Drupal\simple_client\Client
 */
class GuzzleJsonClient
  extends GuzzleClient {

  /**
   * Overrides SimpleClient::getHeaders().
   */
  protected function getHeaders() {

    $headers = parent::getHeaders();
    $headers['Accept'] = 'application/json';

    return $headers;
  }

  /**
   * Overrides SimpleClient::getResponse().
   */
  protected function decodeResponse($result) {

    return json_decode($result);
  }
}
