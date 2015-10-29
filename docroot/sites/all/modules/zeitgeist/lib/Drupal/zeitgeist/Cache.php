<?php
/**
 * @file
 * Zeitgeist cache handling class.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

namespace Drupal\zeitgeist;

/**
 * Zeitgeist cache-related constants and methods.
 *
 * Since some cache plugins are technically unable to perform wildcard
 * clearing, but simulate it (e.g., memcache), this system keeps track of its
 * cache ids to clear each of them manually.
 */
class Cache extends Enum {
  /**
   * @const The cache bin used by the module.
   */
  const BIN = 'cache';

  /**
   * @const The name of the module, uses as a prefix for cache ids.
   */
  const MODULE = 'zeitgeist';

  /**
   * @const The variable holding the list of known cache ids.
   */
  const VARCIDS = 'zeitgeist_cids';

  /**
   * Clear all Zeitgeist cache entries.
   */
  public static function clearAll() {
    $known_cids = array_keys(static::getCids());
    foreach ($known_cids as $cid) {
      $core_cid = static::prefixCid($cid);
      cache_clear_all($core_cid, Cache::BIN);
    }
    variable_del(static::VARCIDS);
  }

  /**
   * Return the data in cache for a given block view().
   *
   * @param string $cid
   *   The cache id.
   *
   * @return string
   *   HTML.
   */
  public static function get($cid) {
    $ret = cache_get(static::prefixCid($cid), static::BIN);
    return $ret;
  }

  /**
   * Store the results of a block view() in cache.
   *
   * @param string $cid
   *   The cache ID of the data to store.
   * @param mixed $data
   *   The data to store in the cache. Complex data types will be automatically
   *   serialized before insertion. Strings will be stored as plain text and are
   *   not serialized.
   * @param int $expire
   *   One of the following values:
   *   - CACHE_PERMANENT: Indicates that the item should never be removed unless
   *     explicitly told to using cache_clear_all() with a cache ID.
   *   - CACHE_TEMPORARY: Indicates that the item should be removed at the next
   *     general cache wipe.
   *   - A Unix timestamp: Indicates that the item should be kept at least until
   *     the given time, after which it behaves like CACHE_TEMPORARY.
   */
  public static function set($cid, $data, $expire) {
    $known_cids = static::getCids();
    if (!isset($known_cids[$cid])) {
      $known_cids[$cid] = TRUE;
      variable_set(static::VARCIDS, $known_cids);
    }
    cache_set(static::prefixCid($cid), $data, static::BIN, $expire);
  }

  /**
   * Get the list of cache ids known to the module.
   */
  public static function getCids() {
    $ret = variable_get('zeitgeist_cids', array());
    return $ret;
  }

  /**
   * Prefix a Block-level cid by the module name for the core cache system.
   *
   * @param string $cid
   *   The block-level cache id.
   *
   * @return string
   *   The cache id to handle to the core cache system.
   */
  public static function prefixCid($cid) {
    $ret = static::MODULE . ":$cid";
    return $ret;
  }
}
