<?php
/**
 * @file
 * Implementation of cache.inc with memcache logic included.
 */

/**
 * Memcache class extended to support Cache Heuristic.
 */
class MemCacheDrupalDeferred extends MemCacheDrupal implements DrupalCacheDeferredInterface {
  /**
   * Serialize the data.
   *
   * @var boolean
   */
  protected $serialize = TRUE;

  public $wildcard_flushes;
  public $invalidate;
  public $cache_lifetime;
  public $cache_flush;
  public $cache_content_flush;
  public $cache_temporary_flush;
  public $flushed;

  /**
   * Constructor.
   */
  public function __construct($bin) {
    $this->serialize = cache_heuristic_variable_get('memcache_deferred', $bin, 'serialize', TRUE);
    $this->memcache = dmemcache_object($bin);
    $this->bin = $bin;

    // If page_cache_without_database is enabled, we have to manually load the
    // $conf array out of cache_bootstrap.
    static $variables_loaded = FALSE;
    // NOTE: We don't call drupal_get_bootstrap_phase() because this would
    // break all 7.x Drupal installations prior to 7.33. For more information
    // see https://www.drupal.org/node/667098.
    if (drupal_bootstrap(NULL, FALSE) <= DRUPAL_BOOTSTRAP_VARIABLES && !$variables_loaded) {
      drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES, FALSE);
      if (!variable_get('memcache_variables_loaded')) {
        global $conf;
        if ($bin == 'cache_bootstrap') {
          $variables_loaded = TRUE;
          $conf = $this->initializeVariables(isset($conf) ? $conf : array());
        }
        else {
          _cache_get_object('cache_bootstrap');
        }
        if (!variable_get('memcache_variables_loaded')) {
          variable_set('memcache_variables_loaded', TRUE);
        }
      }
    }

    $this->reloadVariables();
  }

  /**
   * Loads the persistent variable table.
   *
   * The variable table is composed of values that have been saved in the table
   * with variable_set() as well as those explicitly specified in the
   * configuration file.
   */
  public function initializeVariables($conf = array()) {
    // NOTE: caching the variables improves performance by 20% when serving
    // cached pages.
    if ($cached = $this->get('variables')) {
      $variables = $cached->data;
    }
    else {
      // Cache miss. Avoid a stampede.
      $name = 'variable_init';
      if (!lock_acquire($name, 1)) {
        // Another request is building the variable cache.
        // Wait, then re-run this function.
        lock_wait($name);
        return $this->initializeVariables($conf);
      }
      else {
        // Proceed with variable rebuild.
        $variables = array_map('unserialize', db_query('SELECT name, value FROM {variable}')->fetchAllKeyed());
        $this->set('variables', $variables);
        lock_release($name);
      }
    }

    foreach ($conf as $name => $value) {
      $variables[$name] = $value;
    }

    return $variables;
  }

  /**
   * Implements DrupalCacheInterface::get().
   */
  public function get($cid) {
    $cache = dmemcache_get($cid, $this->bin, $this->memcache);
    if ($this->valid($cid, $cache)) {
      $cache = $this->prepareItem($cache);
      return $cache;
    }
    return FALSE;
  }

  /**
   * Implements DrupalCacheInterface::getMultiple().
   */
  public function getMultiple(&$cids) {
    $results = dmemcache_get_multi($cids, $this->bin, $this->memcache);
    foreach ($results as $cid => &$result) {
      if (!$this->valid($cid, $result)) {
        // This object has expired, so don't return it.
        unset($results[$cid]);
      }
      else {
        $result = $this->prepareItem($result);
      }
    }
    // Remove items from the referenced $cids array that we are returning,
    // per the comment in cache_get_multiple() in includes/cache.inc.
    $cids = array_diff($cids, array_keys($results));
    return $results;
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  public function set($cid, $data, $expire = CACHE_PERMANENT) {
    $created_microtime = round(microtime(TRUE), 3);

    // Create new cache object.
    $cache = new stdClass();
    $cache->cid = $cid;
    $cache->serialized = $this->serialize;
    $cache->data = is_object($data) ? clone $data : $data;
    $cache->data = $cache->serialized ? serialize($cache->data) : $cache->data;
    $cache->created = REQUEST_TIME;
    $cache->created_microtime = $created_microtime;
    // Record the previous number of wildcard flushes affecting our cid.
    $cache->flushes = $this->wildcardFlushes($cid);
    if ($expire == CACHE_TEMPORARY) {
      // Convert CACHE_TEMPORARY (-1) into something that will live in memcache
      // until the next flush.
      $cache->expire = REQUEST_TIME + 2591999;
      // This is a temporary cache item.
      $cache->temporary = TRUE;
    }
    // Expire time is in seconds if less than 30 days, otherwise is a timestamp.
    elseif ($expire != CACHE_PERMANENT && $expire < 2592000) {
      // Expire is expressed in seconds, convert to the proper future timestamp
      // as expected in dmemcache_get().
      $cache->expire = REQUEST_TIME + $expire;
      $cache->temporary = FALSE;
    }
    else {
      $cache->expire = $expire;
      $cache->temporary = FALSE;
    }

    // Manually track the expire time in $cache->expire.  When the object
    // expires, if stampede protection is enabled, it may be served while one
    // process rebuilds it. The ttl sent to memcache is set to the expire twice
    // as long into the future, this allows old items to be expired by memcache
    // rather than evicted along with a sufficient period for stampede
    // protection to continue to work.
    if ($cache->expire == CACHE_PERMANENT) {
      $memcache_expire = $cache->expire;
    }
    else {
      $memcache_expire = $cache->expire + (($cache->expire - REQUEST_TIME) * 2);
    }
    dmemcache_set($cid, $cache, $memcache_expire, $this->bin, $this->memcache);

    // Release lock if acquired in $this->valid().
    $lock = "memcache_$cid:$this->bin";
    if (variable_get('memcache_stampede_protection', FALSE) && isset($GLOBALS['locks'][$lock])) {
      lock_release("$lock");
    }
  }

  /**
   * Unserialize the data if applicable.
   */
  public function prepareItem($item) {
    // If the data is permanent or not subject to a minimum cache lifetime,
    // unserialize and return the cached data.
    if (!empty($item->serialized)) {
      $unserialized = @unserialize($item->data);
      $item->data = $unserialized === FALSE ? $item->data : $unserialized;
    }

    return $item;
  }

  /**
   * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
   */
  public function getMultipleDeferred($cids) {
    $results = dmemcache_get_multi($cids, $this->bin, $this->memcache);
    return $results;
  }

  /**
   * Implements DrupalCacheDeferredInterface::prepareDeferredItem().
   */
  public function prepareDeferredItem($cid, $item) {
    return $this->prepareItem($item);
  }

  /**
   * Implements DrupalCacheDeferredInterface::fetchDeferredItems().
   */
  public function fetchDeferredItems($cids, $results) {
    $results = $results ? $results : array();
    foreach ($results as $cid => &$result) {
      if (!$this->valid($cid, $result)) {
        // This object has expired, so don't return it.
        unset($results[$cid]);
      }
    }
    return $results;
  }

}
