<?php

/**
 * @file
 * Contains ContextioUserClient.
 */

namespace Drupal\fluxcontextio;

/**
 * Service client for the Contextio API.
 */
class ContextioClient extends Client {

  /**
   * {@inheritdoc}
   */
  public static function factory($config = array()) {
    $oauth = new Curl($config['client_id'], $config['client_secret'], $config['storage'], NULL);
    $client = new API($oauth);
    return $client;
  }
}
