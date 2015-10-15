<?php

require_once DRUPAL_ROOT . '/includes/cache.inc';

class SchrodiCacheDatabaseCache extends DrupalDatabaseCache {
  /**
   * Implements DrupalCacheInterface::set().
   */
  function set($cid, $data, $expire = CACHE_PERMANENT) {
    $fields = array(
      'serialized' => 0,
      // This is the whole reason for this class - use better than second
      // granularity, because it makes the hit rate way better.
      'created' => round(microtime(TRUE), 3),
      'expire' => $expire,
    );
    if (!is_string($data)) {
      $fields['data'] = serialize($data);
      $fields['serialized'] = 1;
    }
    else {
      $fields['data'] = $data;
      $fields['serialized'] = 0;
    }
    try {
      db_merge($this->bin)
        ->key(array('cid' => $cid))
        ->fields($fields)
        ->execute();
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
    }
  }
}

class SchrodiCacheApcCache implements DrupalCacheInterface {

  /**
   * The apc prefix.
   */
  protected $apcPrefix;

  /**
   * Constructs a DrupalDatabaseCache object.
   *
   * @param $bin
   *   The cache bin for which the object is created.
   */
  function __construct($bin) {
    global $conf;

    $this->apcPrefix = '';
    if (isset($conf['schrodicache']['apc_prefix'])) {
      $this->apcPrefix = $conf['schrodicache']['apc_prefix'];
    }
    $this->apcPrefix .= $bin;
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  function get($cid) {
    return apc_fetch($this->apcPrefix . $cid);
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  function set($cid, $data, $expire = CACHE_PERMANENT) {
    $fields = (object) array(
      'serialized' => 0,
      'created' => round(microtime(TRUE), 3),
      'expire' => $expire,
      'cid' => $cid,
      'data' => $data,
      'serialized' => 0,
    );
    apc_store($this->apcPrefix . $cid, $fields);
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  function isEmpty() {
    $apc = new ApcIterator('user', '/^' . preg_quote($this->apcPrefix) . '/');
    return $apc->getTotalCount > 0;
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  function clear($cid = NULL, $wildcard = FALSE) {
    if (!empty($cid)) {
      if ($wildcard) {
        if ($cid == '*') {
          apc_delete(new APCIterator('user', '/^' . preg_quote($this->apcPrefix) . '/'));
        }
        else {
          apc_delete(new APCIterator('user', '/^' . preg_quote($this->apcPrefix . $cid) . '/'));
        }
      }
      else {
        apc_delete($cid);
      }
    }
    else {
      apc_delete(new APCIterator('user', '/^' . preg_quote($this->apcPrefix) . '/'));
    }
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  function getMultiple(&$cids) {
    $cache = array();
    $prefixed_cids = array();
    foreach ($cids as $cid) {
      $prefixed_cids[$this->apcPrefix . $cid] = $cid;
    }
    $result = apc_fetch(array_keys($prefixed_cids));
    if (is_array($result)) {
      foreach ($result as $item) {
        $cache[$item->cid] = $item;
      }
    }
    $cids = array_diff($cids, array_keys($cache));
    return $cache;
  }
}

class SchrodiCacheCache implements DrupalCacheInterface {

  /**
   * Cache key prefix for the bin-specific entry to track the last write.
   */
  const LAST_WRITE_TIMESTAMP_PREFIX = 'last_write_timestamp_';

  /**
   * @var string
   */
  protected $bin;

  /**
   * The consistent cache backend.
   */
  protected $consistentBackend;

  /**
   * The fast cache backend.
   */
  protected $fastBackend;

  /**
   * The time at which the last write to this cache bin happened.
   *
   * @var float
   */
  protected $lastWriteTimestamp;

  /**
   * Constructs a ChainedFastBackend object.
   *
   * @param $consistent_backend
   *   The consistent cache backend.
   * @param $fast_backend
   *   The fast cache backend.
   * @param string $bin
   *   The cache bin for which the object is created.
   */
  public function __construct($bin) {
    global $conf;

    $consistent_class = 'SchrodiCacheDatabaseCache';
    if (!empty($conf['schrodicache']['consistent_backend'])) {
      $consistent_class = $conf['schrodicache']['consistent_backend'];
    }
    $this->consistentBackend = new $consistent_class($bin);

    $fast_class = 'SchrodiCacheApcCache';
    if (!empty($conf['schrodicache']['inconsistent_backend'])) {
      $fast_class = $conf['schrodicache']['inconsistent_backend'];
    }
    $this->fastBackend = new $fast_class($bin);

    $shutdown_function = array('SchrodiCacheCache', 'shutdownUpdateLastWriteTimes');
    $not_found = TRUE;
    $callbacks = drupal_register_shutdown_function();
    foreach ($callbacks as $info) {
      if ($info['callback'] == $shutdown_function) {
        $not_found = FALSE;
        break;
      }
    }
    if ($not_found) {
      drupal_register_shutdown_function($shutdown_function);
    }

    $this->bin = $bin;
    $this->lastWriteTimestamp = NULL;
  }

  /**
   * {@inheritdoc}
   */
  public function get($cid, $allow_invalid = FALSE) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids, $allow_invalid);
    return reset($cache);
  }

  /**
   * {@inheritdoc}
   */
  public function getMultiple(&$cids, $allow_invalid = FALSE) {
    $cids_copy = $cids;
    $cache = array();

    // If we can determine the time at which the last write to the consistent
    // backend occurred (we might not be able to if it has been recently
    // flushed/restarted), then we can use that to validate items from the fast
    // backend, so try to get those first. Otherwise, we can't assume that
    // anything in the fast backend is valid, so don't even bother fetching
    // from there.
    $last_write_timestamp = $this->getLastWriteTimestamp();
    if ($last_write_timestamp) {
      // Items in the fast backend might be invalid based on their timestamp,
      // but we can't check the timestamp prior to getting the item, which
      // includes unserializing it. However, unserializing an invalid item can
      // throw an exception. For example, a __wakeup() implementation that
      // receives object properties containing references to code or data that
      // no longer exists in the application's current state.
      //
      // Unserializing invalid data, whether it throws an exception or not, is
      // a waste of time, but we only incur it while a cache invalidation has
      // not yet finished propagating to all the fast backend instances.
      //
      // Most cache backend implementations should not wrap their internal
      // get() implementations with a try/catch, because they have no reason to
      // assume that their data is invalid, and doing so would mask
      // unserialization errors of valid data. We do so here, only because the
      // fast backend is non-authoritative, and after discarding its
      // exceptions, we proceed to check the consistent (authoritative) backend
      // and allow exceptions from that to bubble up.
      try {
        $items = $this->fastBackend->getMultiple($cids, $allow_invalid);
      }
      catch (\Exception $e) {
        $cids = $cids_copy;
        $items = array();
      }

      // Even if items were successfully fetched from the fast backend, they
      // are potentially invalid if older than the last time the bin was
      // written to in the consistent backend, so only keep ones that aren't.
      foreach ($items as $item) {
        if ($item->created < $last_write_timestamp) {
          $cids[array_search($item->cid, $cids_copy)] = $item->cid;
        }
        else {
          $cache[$item->cid] = $item;
        }
      }
    }

    // If there were any cache entries that were not available in the fast
    // backend, retrieve them from the consistent backend and store them in the
    // fast one.
    if ($cids) {
      foreach ($this->consistentBackend->getMultiple($cids, $allow_invalid) as $item) {
        $cache[$item->cid] = $item;
        $this->fastBackend->set($item->cid, $item->data);
      }
    }

    return $cache;
  }

  /**
   * {@inheritdoc}
   */
  public function set($cid, $data, $expire = Cache::PERMANENT, array $tags = array()) {
    $this->markAsOutdated();
    $this->consistentBackend->set($cid, $data, $expire, $tags);
    $this->fastBackend->set($cid, $data, $expire, $tags);
  }

  /**
   * {@inheritdoc}
   */
  public function setMultiple(array $items) {
    $this->markAsOutdated();
    $this->consistentBackend->setMultiple($items);
    $this->fastBackend->setMultiple($items);
  }

  /**
   * {@inheritdoc}
   */
  public function delete($cid) {
    $this->markAsOutdated();
    $this->consistentBackend->deleteMultiple(array($cid));
  }

  /**
   * {@inheritdoc}
   */
  public function deleteMultiple(array $cids) {
    $this->markAsOutdated();
    $this->consistentBackend->deleteMultiple($cids);
  }

  /**
   * {@inheritdoc}
   */
  public function deleteTags(array $tags) {
    $this->markAsOutdated();
    $this->consistentBackend->deleteTags($tags);
  }

  /**
   * {@inheritdoc}
   */
  public function deleteAll() {
    $this->markAsOutdated();
    $this->consistentBackend->deleteAll();
  }

  /**
   * {@inheritdoc}
   */
  public function invalidate($cid) {
    $this->invalidateMultiple(array($cid));
  }

  /**
   * {@inheritdoc}
   */
  public function invalidateMultiple(array $cids) {
    $this->markAsOutdated();
    $this->consistentBackend->invalidateMultiple($cids);
  }

  /**
   * {@inheritdoc}
   */
  public function invalidateTags(array $tags) {
    $this->markAsOutdated();
    $this->consistentBackend->invalidateTags($tags);
  }

  /**
   * {@inheritdoc}
   */
  public function invalidateAll() {
    $this->markAsOutdated();
    $this->consistentBackend->invalidateAll();
  }

  /**
   * {@inheritdoc}
   */
  public function garbageCollection() {
    $this->consistentBackend->garbageCollection();
    $this->fastBackend->garbageCollection();
  }

  /**
   * {@inheritdoc}
   */
  public function removeBin() {
    $this->consistentBackend->removeBin();
    $this->fastBackend->removeBin();
  }

  /**
   * @todo Document in https://www.drupal.org/node/2311945.
   */
  public function reset() {
    $this->lastWriteTimestamp = NULL;
  }

  /**
   * Gets the last write timestamp.
   */
  protected function getLastWriteTimestamp() {
    if ($this->lastWriteTimestamp === NULL) {
      $cache = $this->consistentBackend->get(self::LAST_WRITE_TIMESTAMP_PREFIX . $this->bin);
      $this->lastWriteTimestamp = $cache ? $cache->data : 0;
    }
    return $this->lastWriteTimestamp;
  }

  /**
   * Marks the fast cache bin as outdated because of a write.
   */
  protected function markAsOutdated() {
    // Clocks on a single server can drift. Multiple servers may have slightly
    // differing opinions about the current time. Given that, do not assume
    // 'now' on this server is always later than our stored timestamp.
    $now = microtime(TRUE);
    if ($now > $this->getLastWriteTimestamp()) {
      $this->lastWriteTimestamp = $now;
      $this->consistentBackend->set(self::LAST_WRITE_TIMESTAMP_PREFIX . $this->bin, $this->lastWriteTimestamp);
    }
  }

  /**
   * {@inheritdoc}
   */
  function isEmpty() {
    return $this->consistentBackend->isEmpty();
  }

  function clear($cid = NULL, $wildcard = FALSE) {
    $this->fastBackend->clear($cid, $wildcard);
    $this->consistentBackend->clear($cid, $wildcard);
  }

  public static function shutdownUpdateLastWriteTimes() {
    if (!empty(static::$writeTimes)) {
      static::updateLastWriteTimes(static::$writeTimes);
    }
  }

}

