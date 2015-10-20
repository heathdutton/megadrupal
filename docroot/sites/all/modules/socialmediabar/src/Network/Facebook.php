<?php

/**
 * @file
 * Facebook network for SocialMediaBar.
 */

namespace SocialMediaBar\Network;

require_once 'NetworkInterface.php';
require_once 'NetworkAbstract.php';

/**
 * Sharer for Facebook.
 *
 * @package SocialMediaBar\Network
 */
class Facebook extends NetworkAbstract implements NetworkInterface {
  protected $baseShareURL = 'https://www.facebook.com/sharer/sharer.php';

  /**
   * Returns a URL that will forward the user to the appropriate share screen.
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
    $url = urlencode($url);
    $message = urlencode($message);

    return $this->baseShareURL . '?u=' . $url . '&t=' . $message;
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
    $url = strtolower($url);
    if ('http://' !== substr($url, 0, 7) && 'https://' !== substr($url, 0, 8)) {
      $url = 'http://' . $url;
    }
    $curl = curl_init();

    curl_setopt($curl, CURLOPT_URL, "http://graph.facebook.com/?id=" . $url);
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
    $curl_results = curl_exec($curl);
    curl_close($curl);

    $json = json_decode($curl_results, TRUE);
    return (int) $json['shares'];
  }
}
