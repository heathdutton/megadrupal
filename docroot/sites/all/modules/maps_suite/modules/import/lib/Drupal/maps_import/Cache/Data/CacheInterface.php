<?php

/**
 * @file
 * This interface relates to Data caching.
 */

namespace Drupal\maps_import\Cache\Data;

/**
 * Maps Import Cache class interface.
 */
interface CacheInterface {

  /**
   * Get a single datum from database.
   *
   * @param string $key
   *   The datum identifier.
   * @param array $options
   *   An optional array, whose keys and values depend on the cached
   *   data. See specialized child classes for more information.
   *
   * @return mixed
   *   An instance of the requested data or FALSE if no data was found.
   */
  public function loadSingle($key, array $options = array());

  /**
   * Load all data from cache.
   *
   * @param array $options
   *   An optional array, whose keys and values depend on the cached
   *   data. See specialized child classes for more information.
   *
   * @return array
   *   A list of the retrieved data if any.
   */
  public function load(array $options = array());

  /**
   * Clear the cached data.
   *
   * @param bool $all
   *   Flag indicating whether add related entries in the cache table
   *   should be deleted, or only a single row. If TRUE, the options
   *   parameter may be optional.
   * @param array $options
   *   An optional array, whose keys and values depend on the cached
   *   data. See specialized child classes for more information.
   */
  public function clearBinCache($all = TRUE, array $options = array());

}
