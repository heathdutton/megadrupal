<?php

/**
 * @file
 * Interface for different networks used to pull counts from and share with.
 */

namespace SocialMediaBar\Network;

/**
 * Interface for social media networks to work with the Social Media BAr.
 *
 * @package SocialMediaBar\Network
 */
interface NetworkInterface {
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
  public function buildShareURL($url, $message);

  /**
   * Returns a share count from a network for a URL.
   *
   * @param string $url
   *   URL we want to get a count for.
   *
   * @return int
   *   Share count from the service
   */
  public function getCount($url);
}
