<?php

/**
 * @file
 * Google+ network for SocialMediaBar.
 */

namespace SocialMediaBar\Network;

require_once 'NetworkInterface.php';
require_once 'NetworkAbstract.php';

/**
 * Sharer for Google+.
 *
 * @package SocialMediaBar\Network
 */
class Googleplus extends NetworkAbstract implements NetworkInterface {
  protected $baseShareURL = 'https://plus.google.com/share';

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

    return $this->baseShareURL . '?url=' . $url;
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
  protected function getCountFromService($url) {
    $curl = curl_init();
    curl_setopt($curl, CURLOPT_URL, "https://clients6.google.com/rpc");
    curl_setopt($curl, CURLOPT_POST, 1);
    curl_setopt($curl, CURLOPT_POSTFIELDS, '[{"method":"pos.plusones.get","id":"p","params":{"nolog":true,"id":"' . $url . '","source":"widget","userId":"@viewer","groupId":"@self"},"jsonrpc":"2.0","apiVersion":"v1"}]');
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($curl, CURLOPT_HTTPHEADER, array('Content-type: application/json'));
    $curl_results = curl_exec($curl);
    curl_close($curl);

    $json = json_decode($curl_results, TRUE);
    return (int) $json[0]['result']['metadata']['globalCounts']['count'];
  }
}
