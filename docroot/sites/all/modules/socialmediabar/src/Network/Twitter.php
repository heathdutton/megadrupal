<?php

/**
 * @file
 * Twitter class for SocialMediaBar.
 */

namespace SocialMediaBar\Network;

require_once 'NetworkAbstract.php';
require_once 'NetworkInterface.php';

/**
 * Sharer for Twitter.
 *
 * @package SocialMediaBar\Network
 */
class Twitter extends NetworkAbstract implements NetworkInterface {
  protected $baseCountURL = 'http://urls.api.twitter.com/1/urls/count.json';
  protected $baseShareURL = 'https://twitter.com/intent/tweet';

  /**
   * Builds the URL to Twitter's share interface.
   *
   * @see NetworkInterface::buildShareURL()
   */
  public function buildShareURL($url, $message) {
    $url = urlencode($url);
    $message = urlencode($message);

    return $this->baseShareURL . '?text=' . $message . '&url=' . $url;
  }

  /**
   * Runs a basic GET query to get a count for a network.
   *
   * @param string $url
   *   URL that we need to get the count for.
   *
   * @return int
   *   Count from the service
   */
  protected function getCountFromService($url) {
    $counter_url = $this->baseCountURL . '?url=' . $url;
    $ch = curl_init($counter_url);
    curl_setopt_array($ch, array(
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_RETURNTRANSFER => TRUE,
    ));
    $response = curl_exec($ch);
    $json = json_decode($response, TRUE);
    return $json['count'];
  }
}
