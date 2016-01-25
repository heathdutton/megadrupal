<?php

/**
 * @file
 *   Extends Drupal's default database cache.
 */

/**
 * Alternative implementation of Drupal's default cache backend.
 *
 * This cache backend extends Drupal's default database cache implementation by
 * altering how cached items are cleared. The primary goal is to increaser cache
 * hits by ensuring cron doesn't truncate complete bins.
 *
 * Also note that all comments have been stripped except those pertaining to
 * modifications here. See the original cache.inc file for more details.
 *
 * @see http://drupal.org/node/891600
 */
class AlternativeDrupalDatabaseCache extends DrupalDatabaseCache { 
  /**
   * Implements DrupalCacheInterface::getMultiple().
   *
   * We handle garbage collection differently; this method is reimplemented only
   * to remove a call to the garbageCollection() method.
   */
  function getMultiple(&$cids) {
    try {
      $result = db_query('SELECT cid, data, created, expire, serialized FROM {' . db_escape_table($this->bin) . '} WHERE cid IN (:cids)', array(':cids' => $cids));
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
      return array();
    }
  }

  /**
   * Generic garbage collection method.
   *
   * Rather than respecting minimum cache lifetime at the bin-level, we respect
   * it on an entry-by-entry basis.
   *
   * Additionally, we've moved session cache handling to a separate method.
   */
  protected function garbageCollection() {
    if ($cache_lifetime = variable_get('cache_lifetime', 0)) {
      // Clear expired items from the cache.
      db_delete($this->bin)
        ->condition('expire', CACHE_PERMANENT, '>')
        ->condition('expire', REQUEST_TIME, '<')
        ->execute();

      // Clear CACHE_TEMPORARY items that are older than cache_lifetime.
      db_delete($this->bin)
        ->condition('expire', CACHE_TEMPORARY)
        ->condition('created', REQUEST_TIME - $cache_lifetime, '<')
        ->execute();
    }
    else {
      // No minimum lifetime. Flush all expired and temporary cache entries.
      db_delete($this->bin)
        ->condition('expire', CACHE_PERMANENT, '<>')
        ->condition('expire', REQUEST_TIME, '<')
        ->execute();
    }
  }

  /**
   * Cleans up the per-session cache expiration data.
   */
  protected function cleanSession() {
    $cache_lifetime = variable_get('cache_lifetime', 0);
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
  }

  /**
   * Implements DrupalCacheInterface::set().
   */
  function set($cid, $data, $expire = CACHE_PERMANENT) {
    // When setting cache items, clean up old session data in case it is stale.
    $this->cleanSession();

    return parent::set($cid, $data, $expire);
  }

  /**
   * Implements DrupalCacheInterface::clear().
   *
   * Here, we ensure that cron only runs cache garbage collection at a
   * configurable frequency, defaulting to 24 hours.
   */
  function clear($cid = NULL, $wildcard = FALSE) {
    if (empty($cid)) {
      // Only way to it's from cron is to check the backtrace.
      $backtrace = debug_backtrace();
      if ($backtrace[2]['function'] == 'system_cron') {
        $cache_lifetime = variable_get('cache_lifetime', 0);
        $cache_garbage_collection_frequency = variable_get('cache_garbage_collection_frequency', 86400);
        $name = 'cache_garbage_collect_' . $this->bin;
        $window = max($cache_lifetime, $cache_garbage_collection_frequency);
        if (flood_is_allowed($name, 1, $window, 'cron')) {
          $this->garbageCollection();
          flood_register_event($name, $window, 'cron');
        }
      }
      // Otherwise, clear cache as usual.
      else {
        $this->garbageCollection();
      }
    }
    else {
      // Normal cache clears use default core logic.
      return parent::clear($cid, $wildcard);
    }
  }
}
