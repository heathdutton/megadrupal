<?php

/**
 * @file
 * Contains cache class with fast cache layer.
 */

/**
 * Defines constant with a name of cache bin for the module.
 */
define ('FAST_CACHE_BIN', 'cache_fast_cache');

/**
 * Class performs cache request and mostly works
 * with static cache, except "get" requests when
 * cache was not found in the static cache.
 */
class FastCache implements DrupalCacheInterface {

  /**
   * Cache bin name.
   */
  protected $bin;

  /**
   * Variable which stores prefetched cache objects.
   */
  protected $prefetched = array();

  /**
   * Stores cache items which we didn't find them in the cache storage
   * during cache prefetch.
   */
  protected $prefetch_failed = array();

  /**
   * Defines if "set" and "delete" cache requests should be performed
   * immediately or at the end of the page request.
   */
  protected $lazy_cache = FALSE;

  /**
   * @param $bin
   *   Cache bin name.
   */
  function __construct($bin) {
    $fast_cache = &drupal_static('fast_cache', array());

    // Keep a name of the current cache bin.
    $this->bin = $bin;

    // Get list of cache ids which have to be prefetched. Don't prefetch cache
    // for cache_fast_cache bin to avoid endless loop here.
    if (!isset($fast_cache['prefetched_cids']) && $bin != FAST_CACHE_BIN) {

      // Get URL of the current page. Later we will log information about used
      // cache IDs where this URL will be a cache key.
      $fast_cache['url'] = empty($_GET['q']) ? '<front>' : $_GET['q'];

      // Load list of cache bins and cache ids which were used on this page.
      $cache = cache_get($fast_cache['url'], FAST_CACHE_BIN);

      // Save info into static array about list of prefetched cache ids for the
      // current page.
      $fast_cache['prefetched_cids'] = !empty($cache->data) ? $cache->data : array();
    }

    // If current cache bin contains cache ids which could be prefetched,
    // then make multiple request directly from backend cache, because no sense
    // to invoke getMultiple() function of the current class - we need data
    // from backend!
    if (!empty($fast_cache['prefetched_cids'][$this->bin])) {
      $cids = array_keys($fast_cache['prefetched_cids'][$this->bin]);
      $this->prefetched = fast_cache_bin_backend($this->bin)->getMultiple($cids);

      // $cids contain array of cache ids which were not fetched from the
      // storage. We mark here that prefetch for that cache happened, but no
      // cache data exists in the cache storage. It helps to avoid duplicated
      // calls to cache backend for items which doesn't exist even in the real
      // cache storage.
      foreach ($cids as $cid) {
        $this->prefetch_failed[$cid] = FALSE;
      }
    }

    // Read the setting for lazy cache.
    $this->lazy_cache = variable_get('fast_cache_lazy_cache', FALSE);

    // Initialize empty arrays for fast cache queries. It actually is not very
    // necessary, but helps to avoid multiple isset() and empty() checks
    // in the code below and makes it easier to read and understand.
    $fast_cache['get'][$this->bin] = array();
    $fast_cache['set'][$this->bin] = array();
    $fast_cache['delete'][$this->bin] = array();
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    return !empty($cache) ? reset($cache) : FALSE;
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  function getMultiple(&$cids) {
    $cache = array();

    // Get a static cache with possible "sets" which still not commited
    // to the cache backend.
    $fast_cache = &drupal_static('fast_cache', array());

    // Log information about cache ids which we needed for the current page
    // request.
    foreach ($cids as $cid) {
      $fast_cache['get'][$this->bin][$cid] = TRUE;
    }

    // Static cache magic happens here.
    $cache_empty = array();
    foreach ($cids as $cid) {

      // First try to search a cache object in the static cache. Probably,
      // we already had "set cache" request for this object withing the current
      // php request, and we will avoid request to the cache backend.
      if (isset($fast_cache['set'][$this->bin][$cid])) {
        $cache[$cid] = $fast_cache['set'][$this->bin][$cid];
      }
      // If not found in first static cache, then probably cache already was
      // prefetched.
      elseif (isset($this->prefetched[$cid])) {
        $cache[$cid] = $this->prefetched[$cid];
      }
      // If items is not prefetched, then probably we tried to prefetch it,
      // but it just doesn't exist in the cache storage. It means that no need
      // to make another request to the cache backend to fetch this item.
      elseif (isset($this->prefetch_failed[$cid])) {
        $cache_empty[] = $cid;
      }
    }

    // Leave cache ids for which we were unable to get data from static cache
    // or prefetch.
    $cids = array_diff($cids, array_keys($cache));

    // This array contains cache ids that we need to request from the cache
    // backend.
    $unchecked_cids = array_diff($cids, $cache_empty);

    // If we still have cache IDs which needs to get cache, then search for
    // the cache items in the cache backend.
    if (!empty($unchecked_cids)) {
      $backend_cache = fast_cache_bin_backend($this->bin)->getMultiple($unchecked_cids);

      // Mix together data from the static and backend caches.
      $cache += $backend_cache;
    }

    // Merge cache ids for which we couldn't fetch cache from cache storage.
    $cids = array_merge($cache_empty, $unchecked_cids);

    return $cache;
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  function set($cid, $data, $expire = CACHE_PERMANENT) {

    // Get the array with static cache for "set" and "delete" requests.
    $fast_cache = &drupal_static('fast_cache', array());

    // If cache "set" request happens after static cache has been commited to
    // the cache backend, then we have to execute "set" method directly. See
    // fast_cache_commit_cache() for detailed info.
    if (!empty($fast_cache['commited'])) {
      fast_cache_bin_backend($this->bin)->set($cid, $data, $expire);
      return;
    }

    // We have to unset references from $data array. Otherwise if
    // $data is an array with objects, and then objects will be changed, they
    // will be changed in static cache as well, and we have to prevent that.
    if (is_array($data)) {
      $cache_data = unserialize(serialize($data));
    }
    elseif (is_object($data)) {
      $cache_data = clone $data;
    }
    else {
      $cache_data = $data;
    }

    // Store object in the static cache in the same way like object looks
    // directly after fetching from the cache backend. We need this because
    // cache get requests may find some object in the static cache, so to
    // avoid misleadings we standartize it right here.
    $fast_cache['set'][$this->bin][$cid] = (object) array(
      'cid' => $cid,
      'data' => $cache_data,
      'created' => REQUEST_TIME,
      'expire' => $expire
    );

    // If some "delete" request happened before this "set" request, then no
    // need to commit that "delete" request anymore - it will be overrided by
    // "set" request anyway.
    unset($fast_cache['delete'][$this->bin][$cid]);

    // If we don't use lazy cache, then set cache item immediately.
    if (empty($this->lazy_cache)) {
      fast_cache_bin_backend($this->bin)->set($cid, $data, $expire);
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  function clear($cid = NULL, $wildcard = FALSE) {

    // Get the array with static cache for "set" and "delete" requests.
    $fast_cache = &drupal_static('fast_cache', array());

    // If cache "delete" request happens after static cache has been commited
    // to the cache backend, then we have to execute "delete" method directly.
    // See fast_cache_commit_cache() for detailed info.
    if (!empty($fast_cache['commited'])) {
      fast_cache_bin_backend($this->bin)->clear($cid, $wildcard);
      return;
    }

    // Log information about this request to our static collection with cache
    // requests.
    $fast_cache['delete'][$this->bin][$cid] = $wildcard;

    // Now we have to remove cache items from static cache that were set to
    // the cache during this php request.

    // If $cid is empty, that means that was executed request to flush expired
    // and temporary cache.
    if (empty($cid)) {

      // Get minimum cache lifetime and timestamp when cache was flushed the
      // latest time.
      $cache_lifetime = variable_get('cache_lifetime', 0);
      $cache_flush = variable_get('cache_flush_' . $this->bin, 0);

      // If cache has minimum lifetime, but the current cache bin hasn't been
      // flushed for a long time, then we should run cache expiration for it.
      $cache_expired = !empty($cache_lifetime) && ($cache_flush + $cache_lifetime) < REQUEST_TIME;

      // If no minimum cache lifetime is set, or cache should be expired, remove
      // expired or temporary cache items from the static cache.
      if (empty($cache_lifetime) || $cache_expired) {
        foreach ($fast_cache['set'][$this->bin] as $cache_id => $cache) {
          if ($cache->expire != CACHE_PERMANENT && $cache->expire < REQUEST_TIME) {
            unset($fast_cache['set'][$this->bin][$cache_id]);
            unset($this->prefetched[$cache_id]);
          }
        }
      }
    }
    // If $cid exists and $wilcard also passed to this function, then we should
    // execute wildcard cache flushing.
    elseif ($wildcard) {

      // If $cid is "*", then the entire cache bin should be flushed. So all
      // cache from the current cache bin should not be commited to the cache
      // backend anymore.
      if ($cid == '*') {
        unset($fast_cache['set'][$this->bin]);
        $this->prefetched = array();
      }

      // If $cid is specified, then we should find each cache item with cache
      // id and check if wildcard matches it. It so, then this cache should
      // not be commited to the cache backend anymore.
      else {
        foreach ($fast_cache['set'][$this->bin] as $cache_id => $cache) {
          if (strpos($cache_id, $cid) === 0) {
            unset($fast_cache['set'][$this->bin][$cache_id]);
            unset($this->prefetched[$cache_id]);
          }
        }
      }
    }
    // If only $cid is specified, then simply delete this cache id (or ids)
    // from the static cache, so they won't be commited to the cache backend.
    elseif (is_array($cid)) {
      foreach ($cid as $cache_id) {
        unset($fast_cache['set'][$this->bin][$cache_id]);
        unset($this->prefetched[$cache_id]);
      }
    }
    else {
      unset($fast_cache['set'][$this->bin][$cid]);
      unset($this->prefetched[$cid]);
    }

    // If we don't use lazy cache, then expire cache item immediately.
    if (empty($this->lazy_cache)) {
      fast_cache_bin_backend($this->bin)->clear($cid, $wildcard);
    }
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  function isEmpty() {
    // We know nothing about the emptyness of cache bin, so let a cache backend
    // to respond.
    return fast_cache_bin_backend($this->bin)->isEmpty();
  }

}

/**
 * Returns initialized cache object which is responsible
 * for cache requests for the specified cache bin.
 *
 * @param $cache_bin
 *   Name of cache bin.
 *
 * @return DrupalCacheInterface
 */
function fast_cache_bin_backend($cache_bin) {

  // Prevent duplicate initialization for the same cache bin.
  $backends = &drupal_static('fast_cache_backends');
  if (empty($backends[$cache_bin])) {

    // First try to find a specific cache backend class for the current cache
    // bin.
    $class = variable_get('fast_cache_class_' . $cache_bin);

    // If specific cache backend for the cache bin is not specified, then use
    // default cache backend.
    if (!isset($class)) {
      $class = variable_get('fast_cache_default_class', 'DrupalDatabaseCache');
    }

    // Initialize the cache backend class for the current cache bin and keep it
    // in static cache to prevent from duplicated initialization.
    $backends[$cache_bin] = new $class($cache_bin);
  }

  return $backends[$cache_bin];
}
