<?php

/**
 * @file
 * Creates the appropriate network classes for usage in SocialMediaBar.
 */

namespace SocialMediaBar\Network;

class NetworkFactory {
  /**
   * Returns a social media network to work with.
   *
   * @param string $network
   *   Network we want to build an object for.
   *
   * @return NetworkInterface
   *   Network object to use
   *
   * @throws \Exception
   *   Throws an exception if a network is passed that we do not have code for.
   */
  static public function getNetwork($network) {
    $class = __NAMESPACE__ . '\\' . ucfirst($network);
    $filename = __DIR__ . '/' . ucfirst($network) . '.php';
    if (is_file($filename)) {
      require_once $filename;
      if (class_exists(ucfirst($class))) {
        return new $class();
      }
    }

    throw new \Exception('Network ' . $network . ' does not exist for SocialMediaBar');
  }
}
