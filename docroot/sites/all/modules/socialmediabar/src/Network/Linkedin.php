<?php

/**
 * @file
 * Code for LinkedIn.
 */

namespace SocialMediaBar\Network;

require_once 'NetworkInterface.php';
require_once 'NetworkAbstract.php';

/**
 * Sharer for LinkedIn.
 *
 * @package SocialMediaBar\Network
 */
class Linkedin extends NetworkAbstract implements NetworkInterface {
  protected $baseCountURL = 'http://www.linkedin.com/countserv/count/share';
  protected $baseShareURL = 'http://www.linkedin.com/shareArticle';

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

    return $this->baseShareURL . '?mini=true&url=' . $url . '&title=' . $message;
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
    $counter_url = $this->baseCountURL . '?format=json&url=' . $url;
    $ch = curl_init($counter_url);
    curl_setopt_array($ch, array(
      CURLOPT_SSL_VERIFYPEER => FALSE,
      CURLOPT_RETURNTRANSFER => TRUE,
    ));
    $response = curl_exec($ch);

    $data = json_decode($response, TRUE);
    return $data['count'];
  }
}
