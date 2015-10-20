<?php

/**
 * @file
 * The Maps Import Cache system is used to ensure that there is only one
 * instance of a given object.
 * It also takes care of caching collection of object or data inside the
 * database cache tables.
 */

namespace Drupal\maps_import\Cache;

/**
 * Maps Import Cache class interface.
 */
interface CacheInterface {

  /**
   * Get the current cache instance.
   *
   * @return CacheInterface
   */
  public static function getInstance();

}
