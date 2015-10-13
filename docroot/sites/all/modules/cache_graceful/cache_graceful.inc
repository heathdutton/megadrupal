<?php
/**
 * @file
 *
 * Cache class for gathering heuristics.
 */

include_once 'cache_graceful.defines.inc';

class GracefulCache implements DrupalCacheInterface {
  private $backend = NULL;
  protected $bin;
  private $locks = array();

  function clear($cid = NULL, $wildcard = FALSE) {
    return $this->backend->clear($cid, $wildcard);
  }

  function isEmpty() {
    return $this->backend->isEmpty();
  }

  function set($cid, $data, $expire = CACHE_PERMANENT) {
    if (empty($cid)) return;
    // Add stale period, if not temporary or permanent
    if ($expire > 0) {
      $expire += variable_get('cache_graceful_stale', CACHE_GRACEFUL_STALE);
    }
    $this->backend->set($cid, $data, $expire);
    if (!empty($this->locks[$cid])) {
      unset($this->locks[$cid]);
      try {
        lock_release("cache:$this->bin:$cid");
      }
      catch (Exception $e) {
        // Error releasing lock
      }
    }
  }

  function get($cid) {
    $cids = array($cid);
    $cache = $this->getMultiple($cids);
    return reset($cache);
  }

  function getMultiple(&$cids) {
    $all = $cids;
    $result = $this->backend->getMultiple($cids);
    $cids = array_flip($cids);
    $dirty = FALSE;
    foreach ($all as $cid) {
      if ($dirty) {
        if ($cache = $this->backend->get($cid)) {
          $result[$cid] = $cache;
          unset($cids[$cid]);
        }
      }
      if (!isset($result[$cid])) {
        try {
          $key = "cache:$this->bin:$cid";
          if  (lock_acquire($key)) {
            // Got lock, assuming caller will fill cache.
            $this->locks[$cid] = TRUE;;
          }
          else {
            // No cache, but couldn't lock. Someone else must be recaching
            $lock_wait = variable_get('cache_graceful_wait_for_cache', CACHE_GRACEFUL_WAIT_FOR_CACHE);
            $dirty = TRUE;
            if ($lock_wait && !lock_wait($key, $lock_wait)) {
              if ($cache = $this->backend->get($cid)) {
                $result[$cid] = $cache;
                unset($cids[$cid]);
              }
              else {
              }
            }
            else {
            }
          }
        }
        catch (Exception $e) {
          // Couldn't lock, just ignore
        }
      }
      elseif ($this->hasExpired($result[$cid])) {
        unset($result[$cid]);
        $cids[$cid] = end($cids) + 1;
      }
    }
    $cids = array_flip($cids);
    return $result;
  }

  /**
   * Constructor.
   * Initialize bin and prefetch data
   */
  function __construct($bin) {
    $this->bin = $bin;

    // Let our parent do his work
    $class = variable_get('graceful_cache_class_' . $bin);
    if (!isset($class)) {
      $class = variable_get('graceful_cache_default_class', 'DrupalDatabaseCache');
    }
    $this->backend = new $class($bin);
  }

  /**
   * Check if object has expired
   * @fixme Smelly code: Also sets a lock. Doesn't quite fit the name hasExpired().
   */
  function hasExpired(&$cache) {
    if (empty($cache->cid)) return TRUE;

    if ($cache->expire > 0 && $cache->expire - variable_get('cache_graceful_stale', CACHE_GRACEFUL_STALE) < time()) {
      try {
        $key = "cache:$this->bin:$cache->cid";
        // Expired
        if  (lock_acquire($key)) {
          // Got lock, assuming caller will fill cache.
          $this->locks[$cache->cid] = TRUE;;
          return TRUE;
        }
        else {
          // Cache is present, but we couldn't get a lock. Stale cache returned
          return FALSE;
        }
      }
      catch (Exception $e) {
        // Couldn't lock
        return TRUE;
      }
    }

    // Cache is present and hasn't expired
    return FALSE;
  }
}

