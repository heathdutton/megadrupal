<?php

/**
 * @file
 * Abstract class to group together common code for Networks.
 */

namespace SocialMediaBar\Network;

abstract class NetworkAbstract {
  /**
   * Returns the count from the online service we're querying.
   *
   * @param string $url
   *   URL to get the count for.
   *
   * @return int
   *   Count from the service
   */
  protected abstract function getCountFromService($url);

  /**
   * Returns the share count of the URL.
   *
   * @param string $url
   *   URL to get the count for.
   *
   * @return int
   *   Share count for the URL
   */
  public function getCount($url) {
    $cachekey = $url . get_class($this);
    $cache_ttl = variable_get('socialmediabar_cache_time', 30);

    if ($cache_ttl > 0) {
      $data = cache_get($cachekey);
      if ($data === FALSE) {
        $count = $this->getCountFromService($url);
        cache_set($cachekey, $count, 'cache', strtotime('+' . $cache_ttl . ' minutes'));
        $data = $count;
      }
      else {
        $data = $data->data;
      }
    }
    else {
      $data = $this->getCountFromService($url);
    }

    return $data;
  }
}
