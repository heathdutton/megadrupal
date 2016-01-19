<?php

/**
 * @file
 * Contains StepCache.
 */

/**
 * Manipulates CTools cache.
 */
class StepCache {

  /**
   * CTools cache namespace.
   *
   * @var string
   */
  const CTOOLS_NAMESPACE = 'step_data';

  /**
   * Gets an object from the cache.
   *
   * @param string $name
   *   The name of the cached object to retrieve.
   *
   * @return \stdClass
   *   The cache object.
   */
  public static  function get($name) {
    ctools_include('object-cache');

    $cache = ctools_object_cache_get(static::CTOOLS_NAMESPACE, $name);

    // Create an empty one if it doesn't exist.
    return $cache ?: array('locked' => ctools_object_cache_test(static::CTOOLS_NAMESPACE, $name));
  }

  /**
   * Creates or updates an object in the cache.
   *
   * @param string $name
   *  The name of the object to cache.
   *
   * @param object $data
   *  The object to be cached.
   */
  public static function set($name, $data) {
    ctools_include('object-cache');
    ctools_object_cache_set(static::CTOOLS_NAMESPACE, $name, $data);
  }

  /**
   * Removes an item from the object cache.
   *
   * @param string $name
   *  The name of the object to destroy.
   */
  public static function clear($name) {
    ctools_include('object-cache');
    ctools_object_cache_clear(static::CTOOLS_NAMESPACE, $name);
  }

}
