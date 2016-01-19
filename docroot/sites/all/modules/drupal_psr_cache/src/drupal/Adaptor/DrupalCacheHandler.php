<?php
/**
 * @file
 */

namespace Drupal\PSRCache\Adaptor;

interface DrupalCacheHandler {

  /**
   * @param string $key
   * @param mixed $item
   * @param string $bin
   * @param int $ttl
   */
  public function cacheSet($key, $item, $bin, $ttl);

  /**
   * @param string $key
   * @param string $bin
   * @param bool $useWildcard
   */
  public function cacheClearAll($key, $bin, $useWildcard);

  /**
   * @param string $key
   * @param string $bin
   * @return FALSE|object
   */
  public function cacheGet($key, $bin);

}
