<?php

/**
 * @file
 * Contains \Drupal\nimble_crm\NimbleClient.
 */

namespace Drupal\nimble_crm;

use OAuth2;

/**
 * Get the class OAuth2\Client.
 */
include_once drupal_get_path('module', 'oauth2_client') . '/oauth2_client.inc';

/**
 * Defines an OAuth2 Nimble Client.
 */
class NimbleClient extends OAuth2\Client {

  /**
   * Get and return an access token for the grant_type given in $params.
   */
  protected function getToken($data) {
    if (isset($data['scope']) and $data['scope'] == NULL) {
      unset($data['scope']);
    }

    $data['client_id'] = $this->params['client_id'];
    $data['client_secret'] = $this->params['client_secret'];

    $token_endpoint = $this->params['token_endpoint'];
    $options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($data),
      'headers' => array(
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
    );
    $result = drupal_http_request($token_endpoint, $options);

    if ($result->code != 200) {
      throw new \Exception(
        t("Failed to get an access token of grant_type @grant_type.\nError: @result_error",
          array(
            '@grant_type' => $data['grant_type'],
            '@result_error' => $result->error,
          ))
      );
    }

    return (Array) json_decode($result->data);
  }

}
