<?php

/**
 * @file
 * E-mail sharer through ShareThis.
 */

namespace SocialMediaBar\Network;

require_once 'NetworkInterface.php';
require_once 'NetworkAbstract.php';

/**
 * Sharer for ShareThis E-mail.
 *
 * @package SocialMediaBar\Network
 */
class Email extends NetworkAbstract implements NetworkInterface {
  /**
   * Returns a URL for the share service.
   *
   * ShareThis doesn't exactly share this way but instead through their JS
   * interface, so this doesn't return anything
   *
   * @param string $url
   *   URL that we want to share.
   * @param string $message
   *   Message that we want to default to in the share message.
   *
   * @return string
   *   Built URL for sharing
   */
  public function buildShareURL($url, $message) {
    return NULL;
  }

  /**
   * Returns a share count from a network for a URL.
   *
   * @param string $url
   *   URL we want to get a count for.
   *
   * @return int
   *   Share count from the service
   */
  public function getCountFromService($url) {
    $provider = 'email';
    $api_key = variable_get('socialmediabar_apikey', '');

    $url = 'http://rest.sharethis.com/v1/count/urlinfo?'
      . http_build_query(compact('url', 'provider', 'api_key'));

    $ch = curl_init($url);
    curl_setopt_array($ch, array(
      CURLOPT_RETURNTRANSFER => TRUE,
    ));
    $response = curl_exec($ch);
    curl_close($ch);

    $data = json_decode($response);
    return (int) $data->email->outbound;
  }
}
