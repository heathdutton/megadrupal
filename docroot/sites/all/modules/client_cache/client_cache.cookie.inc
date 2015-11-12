<?php

/**
 * @file
 * Contains implementation of DrupalCacheInterface with cookie logic included.
 */

class ClientCacheCookieDrupal implements DrupalCacheInterface {

  /**
   * {@inheritdoc}
   */
  public function __construct($bin) {
    $this->bin = $bin;
  }

  /**
   * {@inheritdoc}
   */
  public function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    return reset($cache);
  }

  /**
   * {@inheritdoc}
   */
  public function getMultiple(&$cids) {
    try {
      $cache = array();
      foreach ($cids as $cid) {
        $item = new stdClass();
        if (isset($_COOKIE[$cid]) && !empty($_COOKIE[$cid])) {
          // We use JSON as interchange format.
          // This is due to the fact that sometimes we want to
          // access data saved in cookies from JS.
          $item->data = drupal_json_decode($_COOKIE[$cid]);
        }
        else {
          $item->data = NULL;
        }
        $cache[] = $this->prepareItem($item);
      }

      return $cache;
    }
    catch (Exception $e) {
      // If the database is never going to be available, cache requests should
      // return FALSE in order to allow exception handling to occur.
      return array();
    }
  }

  /**
   * {@inheritdoc}
   */
  public function set($cid_raw, $data, $expire = CACHE_PERMANENT) {
    list($cid, $path) = self::getPathFromCid($cid_raw);
    $expire = $this->getExpirePeriod($expire);
    try {
      // We use JSON as interchance format.
      // This is due to the fact that sometimes we want to access
      // data saved in cookies from JS.
      $data = drupal_json_encode($data);
      setcookie($cid, $data, $expire, $path);
    }
    catch (Exception $e) {
      watchdog('client_cache.cookie', 'Serializing of @data failed', array('@data' => print_r($data, TRUE)));
    }
  }

  /**
   * {@inheritdoc}
   */
  public function clear($cid_raw = NULL, $wildcard = FALSE) {
    list($cid,) = self::getPathFromCid($cid_raw);
    if (self::get($cid)) {
      self::set($cid_raw, NULL, -1000);
    }
  }
  /**
   * {@inheritdoc}
   */
  public function isEmpty() {
  }

  /**
   * Prepare a cached item.
   *
   * Checks that items are either permanent or did not expire, and decodes
   * data as appropriate.
   *
   * @param mixed $cache
   *   An item loaded from cache_get() or cache_get_multiple().
   *
   * @return mixed
   *   The item with data decoded as appropriate or FALSE if there is no
   *   valid item to load.
   */
  protected function prepareItem($cache) {
    return $cache;
  }

  /**
   * Provides cache period.
   *
   * @param int $expire
   *   An integer of cache type or seconds to cache.
   *
   * @return int
   *   An integer of time to cache.
   */
  protected function getExpirePeriod($expire) {
    switch ($expire) {
      case CACHE_PERMANENT:
        // Cache it for long time for permanent.
        // @TODO make as configurable variable?
        $expire = time() + (60 * 60 * 24 * 500);
        break;

      case CACHE_TEMPORARY:
        // Set to remove cache on this session end.
        $expire = 0;
        break;

      default:
        // If integer, consider it as seconds.
        if (is_int($expire)) {
          $expire = time() + $expire;
        }
        else {
          $expire = 0;
        }
        break;

    }
    return $expire;
  }

  /**
   * Provides path from cache id.
   *
   * @param string $cid
   *   A string cache ID.
   *
   * @return array
   *   An array of cache id and path.
   */
  protected function getPathFromCid($cid) {
    $path = '/';
    if (is_array($cid) && isset($cid['path'])) {
      $path = $cid['path'];
      $cid = $cid['cid'];
    }
    return array($cid, $path);
  }

}
