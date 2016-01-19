<?php

/**
 * @file
 * CacheWarmerConnectFakeCache.
 */

class CacheWarmerConnectFakeCache implements DrupalCacheInterface {
  protected $bin;

  /**
   * Constructs a DrupalDatabaseCache object.
   *
   * @param $bin
   *   The cache bin for which the object is created.
   */
  function __construct($bin) {
    $this->bin = $bin;
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  function get($cid) {
  }

  function getMultiple(&$cids) {
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  function set($cid, $data, $expire = CACHE_PERMANENT) {
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  function clear($cid = NULL, $wildcard = FALSE) {
    // Bail out if cachewarming is not enabled.
    if (variable_get('cachewarmer_connect_status', FALSE) != TRUE) {
      return;
    }

    $cachewarmer_callback_url = variable_get('cachewarmer_connect_callback_url', 'https://www.cache-warmer.com/webhook/cachewarmer');
    $cachewarmer_project_id = variable_get('cachewarmer_connect_project_id');
    $cachewarmer_secret = variable_get('cachewarmer_connect_secret');

    if (!empty($cachewarmer_callback_url) && !empty($cachewarmer_project_id) && !empty($cachewarmer_secret)) {
      $data_array  = array('id' => $cachewarmer_project_id, 'secret' => $cachewarmer_secret);
      $data = json_encode($data_array);
      $response = drupal_http_request($cachewarmer_callback_url, array(
        'method' => 'POST',
        'data' => $data,
        'timeout' => 15,
        'headers' => array('Content-Type' => 'application/json'),
      ));

      if (!isset($response->code) || $response->code != "200") {
        $data = '';
        if (isset($response->data)) {
          $data = strip_tags($response->data);
        }
        $args = array('@error' => $data);
        watchdog('cachewarmer_connect', 'An error occured when trying to warm the cache:<br /> @error', $args, WATCHDOG_ERROR);
      }
      else {
        watchdog('cachewarmer_connect', 'Cachewarming triggered via Cachewarmer Connect.', array(), WATCHDOG_INFO);
      }
    }
  }

  /**
   * Implements DrupalCacheInterface::isValidBin().
   */
  function isValidBin() {
    return TRUE;
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  function isEmpty() {
  }

}
