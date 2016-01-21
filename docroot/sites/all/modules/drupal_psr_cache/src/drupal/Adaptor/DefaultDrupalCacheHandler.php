<?php
/**
 * @file
 */

namespace Drupal\PSRCache\Adaptor;

class DefaultDrupalCacheHandler implements DrupalCacheHandler {

  const CACHE_PERMANENT = 0;

  const DEFAULT_CACHE_BIN = 'cache';

  public function cacheSet($key, $item, $bin = self::DEFAULT_CACHE_BIN, $ttl = self::CACHE_PERMANENT) {
    cache_set($key, $item, $bin, $ttl);
  }

  public function cacheClearAll($key = NULL, $bin = NULL, $useWildcard = FALSE) {
    cache_clear_all($key, $bin, $useWildcard);
  }

  public function cacheGet($key, $bin = self::DEFAULT_CACHE_BIN) {
    return cache_get($key, $bin);
  }

}
