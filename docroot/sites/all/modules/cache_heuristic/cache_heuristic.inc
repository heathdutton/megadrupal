<?php
/**
 * @file
 * Cache class for gathering heuristics.
 */

/**
 * Helper function for getting bin specific variables with fallback.
 *
 * @param string $prefix
 *   The variable prefix, e.g. 'heuristic_cache'.
 * @param string $bin
 *   The name of the bin, e.g. 'cache'.
 * @param string $name
 *   The name of the variable, e.g. 'class'.
 * @param string $default
 *   The default value if none found, e.g. 'DrupalDatabaseCache'.
 *
 * @return mixed
 *   The value of the variable.
 */
function cache_heuristic_variable_get($prefix, $bin, $name, $default = NULL) {
  $data = variable_get($prefix . '_' . $name . '_' . $bin);
  if ($bin && !isset($data)) {
    $data = variable_get($prefix . '_default_' . $name, $default);
  }
  return isset($data) ? $data : $default;
}

/**
 * Class using destructor hack as an event for drupal_static_reset().
 */
class DrupalStaticResetWatcher {
  protected $callback;

  /**
   * Constructor.
   *
   * @param mixed $callback
   *   The callback to invoke during desctruction.
   */
  public function __construct($callback) {
    $this->callback = $callback;
  }

  /**
   * Destructor.
   *
   * Invoke callback.
   */
  public function __destruct() {
    call_user_func($this->callback);
  }

}

/**
 * Base class for HeuristicCache.
 */
class HeuristicCacheBase implements DrupalCacheInterface {
  /**
   * Global error log.
   *
   * @var array
   */
  static protected $errors = array();

  /**
   * Global prefetch store.
   *
   * @var array
   */
  static protected $prefetchData = NULL;

  /**
   * Global prepared store.
   *
   * @var array
   */
  static protected $preparedData = NULL;

  /**
   * Global set of requested cache IDs.
   *
   * @var array
   */
  static protected $set = array();

  /**
   * Pointer to a bin in the global prefetch store for easy access.
   *
   * @var array
   */
  protected $prefetch = array();

  /**
   * Static cache for fetched items.
   *
   * @var array
   */
  protected $prepared = array();

  /**
   * The Drupal cache object to use for cache requests.
   *
   * @var DrupalCacheInterface
   */
  protected $backend = NULL;

  /**
   * Static cache.
   *
   * @var boolean
   */
  protected $static = NULL;

  /**
   * Buffer size.
   *
   * @var integer
   */
  protected $bufferSize = NULL;

  /**
   * The bin for this cache object.
   *
   * @var string
   */
  protected $bin;

  /**
   * Whether or not the deferred items have been fetched yet.
   *
   * @var boolean
   */
  protected $prefetchCids = NULL;

  /**
   * Constructor.
   *
   * Initialize bin and prefetch data.
   */
  public function __construct($bin) {
    $this->bin = $bin;

    // Load configuration.
    $class = cache_heuristic_variable_get('heuristic_cache', $this->bin, 'class', 'DrupalDatabaseCacheDeferred');
    $this->backend = new $class($bin);
    $this->static = cache_heuristic_variable_get('heuristic_cache', $this->bin, 'static', FALSE);
    $this->bufferSize = cache_heuristic_variable_get('heuristic_cache', $this->bin, 'buffer_size', -1);

    self::prefetchBins($this);

    // Initialize prefetch for this bin.
    if (!array_key_exists($bin, self::$prefetchData)) {
      self::$prefetchData[$bin] = NULL;
      self::$preparedData[$bin] = NULL;
    }
    $this->prefetch = &self::$prefetchData[$bin];
    $this->prepared = &self::$preparedData[$bin];

    // Setup static reset watcher.
    $watcher = &drupal_static('cache_heuristic_static_watcher', NULL);
    if (!isset($watcher)) {
      $watcher = new DrupalStaticResetWatcher(array(get_class($this), 'staticReset'));
    }
  }

  /**
   * Reset static vars when drupal_static_reset() is called.
   */
  static public function staticReset() {
    foreach (array_keys(self::$prefetchData) as $bin) {
      self::$prefetchData[$bin] = array();
      self::$preparedData[$bin] = array();
    }

    $watcher = &drupal_static('cache_heuristic_static_watcher', NULL);
    if (!isset($watcher)) {
      $watcher = new DrupalStaticResetWatcher(array(__CLASS__, __FUNCTION__));
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    // Handle static caching.
    if ($this->static) {
      // Ensure items are fetched.
      $this->prefetchExecute();
      if ($wildcard) {
        if ($cid == '*') {
          // Flush all static cache.
          $this->prefetch = array();
          $this->prepared = array();
        }
        else {
          // Remove matching cache IDs from static cache.
          $cids = is_array($cid) ? $cid : array($cid);
          foreach (array_keys($this->prepared) as $key) {
            foreach ($cids as $c) {
              if ((string) $key == "" || strpos((string) $key, (string) $c) === 0) {
                unset($this->prefetch[$key]);
                unset($this->prepared[$key]);
              }
            }
          }
        }
      }
      else {
        // Remove cache ID from static cache.
        $cids = is_array($cid) ? $cid : array($cid);
        foreach ($cids as $c) {
          unset($this->prefetch[$c]);
          unset($this->prepared[$c]);
        }
      }
    }

    // Invoke backend.
    return $this->backend->clear($cid, $wildcard);
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    return $this->backend->isEmpty();
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    // Handle static caching.
    if ($this->static) {
      // Ensure items are fetched.
      $this->prefetchExecute();
      unset($this->prefetch[$cid]);
      unset($this->prepared[$cid]);
    }

    // Invoke backend.
    return $this->backend->set($cid, $data, $expire);
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  public function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    return reset($cache);
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   *
   * Aggregate cache data from prefetch and the requested cache IDs.
   */
  public function getMultiple(&$cids) {
    // Ensure items are fetched.
    $this->prefetchExecute();

    // Prepare fetch array for cache IDs missed by prefetch.
    $fetch = array_combine($cids, $cids);
    $result = array();

    // Update heuristics with the cache IDs requested.
    $this->addToSet($cids);

    // Use prefetch data where available.
    foreach ($cids as $cid) {
      if (isset($this->prefetch[$cid])) {
        // Prefetched cache ID found, prepare the cache item.
        $value = $this->backend->prepareDeferredItem($cid, $this->prefetch[$cid]);

        // Static cache it if applicable.
        if ($this->static) {
          $this->prepared[$cid] = $this->deepClone($value);
        }

        // Prefetch done, remove it from the prefetch array.
        $this->prefetch[$cid] = NULL;

        // Make sure we don't fetch we item, because we already have it.
        unset($fetch[$cid]);
        if ($value) {
          $result[$cid] = $value;
        }
      }
      elseif (isset($this->prepared[$cid])) {
        // Item is static cached. Use it, and make sure we don't fetch the item
        // again.
        unset($fetch[$cid]);
        $result[$cid] = $this->deepClone($this->prepared[$cid]);
      }
    }

    // Get all data that wasn't prefetched, from caching backend.
    if ($fetch) {
      $fetch = array_values($fetch);
      $result += $this->backend->getMultiple($fetch);
      // Handle static cache if applicable.
      if ($this->static) {
        foreach ($result as $cid => $data) {
          $this->prepared[$cid] = $this->deepClone($data);
        }
      }
    }

    // Calculate retrieved cache IDs.
    $cids = array_diff($cids, array_keys($result));
    return $result;
  }

  /**
   * Prefetch all relevant bins if applicable.
   */
  public static function prefetchBins($self = NULL) {
    // Run once only pattern.
    if (isset(self::$prefetchData)) {
      return;
    }

    // Only perform heuristic stuff on valid requests.
    if (drupal_is_cli() || !in_array($_SERVER['REQUEST_METHOD'], variable_get('cache_heuristic_valid_requests', array('GET')))) {
      self::$prefetchData = array();
      return;
    }

    // Support early-phase database query logging.
    if (method_exists('Database', 'getLog') && variable_get('cache_heuristic_devel_query_display', FALSE)) {
      @include_once DRUPAL_ROOT . '/includes/database/log.inc';
      Database::startLog('devel');
    }

    // Get cache IDs for this url.
    self::$prefetchData = array();
    $cid = 'hc:' . request_uri();

    // Don't double instantiate object, if the current cache object is also
    // used by the cache_heuristic bin.
    $hc_bin = variable_get('cache_heuristic_bin', 'cache_heuristic');
    $backend = $self && $hc_bin == $self->bin ? self : _cache_get_object($hc_bin);

    // We don't want to add the cache IDs to the set during prefetch,
    // so we use the backend directly if the bin is a HeuristicCache.
    $backend = $backend instanceof HeuristicCache ? $backend->backend : $backend;
    if (($result = $backend->get($cid)) && is_array($result->data)) {
      $set = $result->data;
    }
    else {
      $set = array();
    }

    // Prefetch data for each bin in set.
    foreach ($set as $bin => $cids) {
      // Again, make sure we don't double instantiate the cache object.
      $cache = $self && $bin == $self->bin ? $self : _cache_get_object($bin);

      $cids = array_keys($cids);

      // Only prefetch bins using the HeuristicCache backend.
      if (!$cache instanceof HeuristicCache) {
        continue;
      }

      // Buffer is disabled, skip it.
      if ($cache->bufferSize == 0) {
        continue;
      }

      $cache_backend = $cache->backend;

      // We only support deferred cache requests.
      if ($cache_backend instanceof DrupalCacheDeferredInterface) {
        $cache->prefetchCids = $cache->bufferSize > 0 ? array_slice($cids, 0, $cache->bufferSize) : $cids;
        self::$prefetchData[$bin] = $cache_backend->getMultipleDeferred($cache->prefetchCids);
      }
      else {
        // Using watchdog() may very well be way too early. Buffer the errors
        // until later, and let our hook_init() log them.
        self::$errors[] = array(
          'Cache bin: %bin is not compatible with class: %class. Cache Heuristic requires a cache backend implementing DrupalCacheDeferredInterface.',
          array(
            '%bin' => $bin,
            '%class' => get_class($cache_backend),
          ),
        );
      }
    }

    // Store the registered set of cache IDs at the end of the request.
    drupal_register_shutdown_function(array('HeuristicCache', 'storeSet'), $cid, $backend, $set);
  }

  /**
   * Execute a deferred cache query if applicable.
   */
  protected function prefetchExecute() {
    if (isset($this->prefetchCids)) {
      $cids = $this->prefetchCids;
      $this->prefetchCids = NULL;
      $this->prefetch = $this->backend->fetchDeferredItems($cids, $this->prefetch);
    }
  }

  /**
   * Add cache IDs to set.
   */
  protected function addToSet($cids) {
    foreach ($cids as $cid) {
      if ($this->bufferSize < 0 || ($this->bufferSize > (empty(self::$set[$this->bin]) ? 0 : count(self::$set[$this->bin])))) {
        self::$set[$this->bin][$cid] = TRUE;
      }
    }
  }

  /**
   * Store the set of cache IDs generated by this request.
   *
   * @param string $cid
   *   The cache ID of the set.
   * @param DrupalCacheInterface $cache
   *   The cache object to use for storing the set.
   * @param array $set
   *   The cids originally retrieved, keyed by bin.
   */
  public static function storeSet($cid, DrupalCacheInterface $cache, array $set) {
    $store = FALSE;
    $total_difference = 0;

    // Cleanup and check threshold for all bins.
    $bins = $set + self::$set;

    foreach ($bins as $bin => $cids) {
      // If difference exceeds threshold, then mark set to be stored.
      $new_cids = !empty(self::$set[$bin]) ? array_keys(self::$set[$bin]) : array();
      $org_cids = !empty($set[$bin]) ? array_keys($set[$bin]) : array();
      $difference = count(array_diff($org_cids, $new_cids)) + count(array_diff($new_cids, $org_cids));
      $threshold = cache_heuristic_variable_get('heuristic_cache', $bin, 'threshold', 5);
      $total_difference += $difference;

      // If any of the bins exceed their own threshold, make sure we store
      // the set at end.
      $store = $store ? $store : $difference > $threshold;

      // Remove the bin from the set entirely, if there are no cache IDs in it.
      if (empty($new_cids)) {
        unset(self::$set[$bin]);
      }
    }

    // Store the set if necessary.
    $total_threshold = variable_get('heuristic_cache_totalthreshold');
    if ($store || (isset($total_threshold) && $total_difference > $total_threshold)) {
      if (empty(self::$set)) {
        $cache->clear($cid);
      }
      else {
        $cache->set($cid, self::$set);
      }
    }
  }

  /**
   * Deep clone data structure.
   *
   * @param mixed $data
   *   The data structure to clone.
   *
   * @return mixed
   *   Cloned data structure.
   */
  protected function deepClone($data) {
    if (is_array($data)) {
      $result = array();
      foreach ($data as $key => $value) {
        $result[$key] = $this->deepClone($value);
      }
      return $result;
    }
    elseif (is_object($data)) {
      return clone $data;
    }
    else {
      return $data;
    }
  }

  /**
   * Get global error log.
   */
  static public function getErrors() {
    return self::$errors;
  }

}

if (interface_exists('DrupalTransactionalCacheInterface', FALSE)) {
  /**
   * HeuristicCache with transactional backend.
   */
  class HeuristicCache extends HeuristicCacheBase implements DrupalTransactionalCacheInterface {
    /**
     * Implements DrupalTransactionalCacheInterface::rollback().
     */
    public function rollback($depth) {
      if ($this->backend instanceof DrupalTransactionalCacheInterface) {
        return $this->backend->rollback($depth);
      }
    }

    /**
     * Implements DrupalTransactionalCacheInterface::commit().
     */
    public function commit($depth) {
      if ($this->backend instanceof DrupalTransactionalCacheInterface) {
        return $this->backend->commit($depth);
      }
    }

  }
}
else {
  /**
   * HeuristicCache vanilla style.
   */
  class HeuristicCache extends HeuristicCacheBase {}
}

/**
 * Deferred cache interface.
 */
interface DrupalCacheDeferredInterface {
  /**
   * Returns data from the persistent cache when given an array of cache IDs.
   *
   * @param mixed $cids
   *   An array of cache IDs for the data to retrieve.
   *
   * @return mixed
   *   A result object of the cache query.
   */
  public function getMultipleDeferred($cids);

  /**
   * Prepare a deferred item.
   *
   * @param string $cid
   *   The cache ID.
   * @param mixed $item
   *   The unprepared item.
   *
   * @return mixed
   *   The prepared item.
   */
  public function prepareDeferredItem($cid, $item);

  /**
   * Fetch deferred items.
   *
   * @param mixed $cids
   *   The cache IDs requested.
   * @param mixed $result
   *   The result from getMultipleDeferred.
   *
   * @return array
   *   Array of cache items, keyed by cache IDs.
   */
  public function fetchDeferredItems($cids, $result);

}

/**
 * Base classes.
 */

/**
 * DrupalDatabaseCache with support for specific database connection.
 */
class DrupalDatabaseCacheTarget extends DrupalDatabaseCache {
  /**
   * Database options to use for querying the bin.
   *
   * @var array
   */
  protected $options = array();

  /**
   * Use tombstone flush mechanism.
   *
   * @var boolean
   */
  protected $tombstoneFlush = FALSE;

  /**
   * Timestamp in microseconds of last flush.
   *
   * @var float
   */
  protected $flushed = 0;

  /**
   * Constructor.
   */
  public function __construct($bin) {
    parent::__construct($bin);

    // Configurable database target.
    $this->options['target'] = cache_heuristic_variable_get('cache', $this->bin, 'target', 'default');

    // Use tombstone flush.
    $this->tombstoneFlush = cache_heuristic_variable_get('cache', $this->bin, 'tombstone_flush', FALSE);

    if ($this->tombstoneFlush) {
      if (drupal_get_bootstrap_phase() > DRUPAL_BOOTSTRAP_VARIABLES) {
        $this->flushed = variable_get('cache_flushed_' . $bin);
      }
      else {
        $this->flushed = db_select('variable', 'v')
          ->fields('v', array('value'))
          ->condition('v.name', 'cache_flushed_' . $bin)
          ->execute()
          ->fetchField();
        $this->flushed = $this->flushed ? unserialize($this->flushed) : 0;
      }
    }
  }

  /**
   * Prepares a cached item.
   *
   * Checks that items are either permanent or did not expire, and unserializes
   * data as appropriate.
   *
   * @param mixed $cache
   *   An item loaded from cache_get() or cache_get_multiple().
   *
   * @return mixed
   *   The item with data unserialized as appropriate or FALSE if there is no
   *   valid item to load.
   */
  protected function prepareItem($cache) {
    global $user;

    if (!isset($cache->data)) {
      return FALSE;
    }

    if ($this->flushed && $this->flushed >= $cache->created) {
      return FALSE;
    }

    // If the cached data is temporary and subject to a per-user minimum
    // lifetime, compare the cache entry timestamp with the user session
    // cache_expiration timestamp. If the cache entry is too old, ignore it.
    if ($cache->expire != CACHE_PERMANENT && variable_get('cache_lifetime', 0) && isset($_SESSION['cache_expiration'][$this->bin]) && $_SESSION['cache_expiration'][$this->bin] > $cache->created) {
      // Ignore cache data that is too old and thus not valid for this user.
      return FALSE;
    }

    // If the data is permanent or not subject to a minimum cache lifetime,
    // unserialize and return the cached data.
    if ($cache->serialized) {
      $cache->data = unserialize($cache->data);
    }

    return $cache;
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  public function getMultiple(&$cids) {
    try {
      // Garbage collection necessary when enforcing a minimum cache lifetime.
      $this->garbageCollection($this->bin);

      // When serving cached pages, the overhead of using db_select() was found
      // to add around 30% overhead to the request. Since $this->bin is a
      // variable, this means the call to db_query() here uses a concatenated
      // string. This is highly discouraged under any other circumstances, and
      // is used here only due to the performance overhead we would incur
      // otherwise. When serving an uncached page, the overhead of using
      // db_select() is a much smaller proportion of the request.
      $conn = Database::getConnection($this->options['target']);
      $result = $conn->query('SELECT cid, data, created, expire, serialized FROM {' . $conn->escapeTable($this->bin) . '} WHERE cid IN (:cids)', array(':cids' => $cids), $this->options);
      $cache = array();
      foreach ($result as $item) {
        $item = $this->prepareItem($item);
        if ($item) {
          $cache[$item->cid] = $item;
        }
      }
      $cids = array_diff($cids, array_keys($cache));
      return $cache;
    }
    catch (Exception $e) {
      // If the database is never going to be available, cache requests should
      // return FALSE in order to allow exception handling to occur.
      return array();
    }
  }

  /**
   * Garbage collection for get() and getMultiple().
   */
  protected function garbageCollection() {
    $cache_lifetime = variable_get('cache_lifetime', 0);

    // Clean-up the per-user cache expiration session data, so that the session
    // handler can properly clean-up the session data for anonymous users.
    if (isset($_SESSION['cache_expiration'])) {
      $expire = REQUEST_TIME - $cache_lifetime;
      foreach ($_SESSION['cache_expiration'] as $bin => $timestamp) {
        if ($timestamp < $expire) {
          unset($_SESSION['cache_expiration'][$bin]);
        }
      }
      if (!$_SESSION['cache_expiration']) {
        unset($_SESSION['cache_expiration']);
      }
    }

    // Garbage collection of temporary items is only necessary when enforcing
    // a minimum cache lifetime.
    if (!$cache_lifetime) {
      return;
    }
    // When cache lifetime is in force, avoid running garbage collection too
    // often since this will remove temporary cache items indiscriminately.
    $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
    if ($cache_flush && ($cache_flush + $cache_lifetime <= REQUEST_TIME)) {
      // Reset the variable immediately to prevent a meltdown in heavy load
      // situations.
      variable_set('cache_flush_' . $this->bin, 0);
      // Time to flush old cache data.
      db_delete($this->bin, $this->options)
        ->condition('expire', CACHE_PERMANENT, '<>')
        ->condition('expire', $cache_flush, '<=')
        ->execute();
    }
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $fields = array(
      'serialized' => 0,
      'created' => REQUEST_TIME,
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
      db_merge($this->bin, $this->options)
        ->key(array('cid' => $cid))
        ->fields($fields)
        ->execute();
    }
    catch (Exception $e) {
      // The database may not be available, so we'll ignore cache_set requests.
    }
  }

  /**
   * Implements DrupalCacheInterface::clear().
   */
  public function clear($cid = NULL, $wildcard = FALSE) {
    global $user;

    if (empty($cid)) {
      if (variable_get('cache_lifetime', 0)) {
        // We store the time in the current user's session. We then simulate
        // that the cache was flushed for this user by not returning cached
        // data that was cached before the timestamp.
        $_SESSION['cache_expiration'][$this->bin] = REQUEST_TIME;

        $cache_flush = variable_get('cache_flush_' . $this->bin, 0);
        if ($cache_flush == 0) {
          // This is the first request to clear the cache, start a timer.
          variable_set('cache_flush_' . $this->bin, REQUEST_TIME);
        }
        elseif (REQUEST_TIME > ($cache_flush + variable_get('cache_lifetime', 0))) {
          // Clear the cache for everyone, cache_lifetime seconds have
          // passed since the first request to clear the cache.
          db_delete($this->bin, $this->options)
            ->condition('expire', CACHE_PERMANENT, '<>')
            ->condition('expire', REQUEST_TIME, '<')
            ->execute();
          variable_set('cache_flush_' . $this->bin, 0);
        }
      }
      else {
        // No minimum cache lifetime, flush all temporary cache entries now.
        db_delete($this->bin, $this->options)
          ->condition('expire', CACHE_PERMANENT, '<>')
          ->condition('expire', REQUEST_TIME, '<')
          ->execute();
      }
    }
    else {
      if ($wildcard) {
        if ($cid == '*') {
          // Check if $this->bin is a cache table before truncating. Other
          // cache_clear_all() operations throw a PDO error in this situation,
          // so we don't need to verify them first. This ensures that non-cache
          // tables cannot be truncated accidentally.
          if ($this->isValidBin()) {
            if ($this->tombstoneFlush) {
              $this->flushed = microtime(TRUE);
              variable_set('cache_flushed_' . $this->bin, $this->flushed);
            }
            else {
              db_truncate($this->bin, $this->options)->execute();
            }
          }
          else {
            throw new Exception(t('Invalid or missing cache bin specified: %bin', array('%bin' => $this->bin)));
          }
        }
        else {
          db_delete($this->bin, $this->options)
            ->condition('cid', db_like($cid) . '%', 'LIKE')
            ->execute();
        }
      }
      elseif (is_array($cid)) {
        // Delete in chunks when a large array is passed.
        do {
          db_delete($this->bin, $this->options)
            ->condition('cid', array_splice($cid, 0, 1000), 'IN')
            ->execute();
        } while (count($cid));
      }
      else {
        db_delete($this->bin, $this->options)
          ->condition('cid', $cid)
          ->execute();
      }
    }
  }

  /**
   * Implements DrupalCacheInterface::isEmpty().
   */
  public function isEmpty() {
    $this->garbageCollection();
    $query = db_select($this->bin, NULL, $this->options);
    $query->addExpression('1');
    $result = $query->range(0, 1)
      ->execute()
      ->fetchField();
    return empty($result);
  }

}

/**
 * Deferred cache implementation using asynchronous database queries.
 */
class DrupalDatabaseCacheDeferred extends DrupalDatabaseCacheTarget implements DrupalCacheDeferredInterface {
  /**
   * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
   */
  public function getMultipleDeferred($cids) {
    try {
      // Garbage collection necessary when enforcing a minimum cache lifetime.
      $this->garbageCollection($this->bin);

      // When serving cached pages, the overhead of using db_select() was found
      // to add around 30% overhead to the request. Since $this->bin is a
      // variable, this means the call to db_query() here uses a concatenated
      // string. This is highly discouraged under any other circumstances, and
      // is used here only due to the performance overhead we would incur
      // otherwise. When serving an uncached page, the overhead of using
      // db_select() is a much smaller proportion of the request.
      $conn = Database::getConnection($this->options['target']);
      $result = $conn->query('SELECT cid, data, created, expire, serialized FROM {' . $conn->escapeTable($this->bin) . '} WHERE cid IN (:cids)', array(':cids' => $cids), $this->options)->fetchAllAssoc('cid');
    }
    catch (Exception $e) {
      // If the database is never going to be available, cache requests should
      // return FALSE in order to allow exception handling to occur.
      $result = array();
    }
    return $result;
  }

  /**
   * Implements DrupalCacheDeferredInterface::prepareDeferredItem().
   */
  public function prepareDeferredItem($cid, $item) {
    return $this->prepareItem($item);
  }

  /**
   * Implements DrupalCachedDeferredInterface::fetchDeferredItems().
   */
  public function fetchDeferredItems($cids, $result) {
    return $result;
  }

}
