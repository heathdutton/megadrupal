<?php

/**
 * @file
 * This interface relates to Object caching.
 */

namespace Drupal\maps_import\Cache\Object;

/**
 * Maps Import Cache class interface.
 */
interface CacheInterface {

  /**
   * Load all records from database.
   *
   * @return array
   *   The fetched data if any.
   */
  public function loadAll();

  /**
   * Get a single datum from database.
   *
   * @param mixed $criterium
   *   The value used to filter the data.
   * @param string $column
   *   The column where to search for the given criteria. If NULL, the default
   *   column defined by child class is used.
   * @param array $conditions
   *   See the method load() below.
   *
   * @return mixed
   *   An instance of the requested data or FALSE if no data was found.
   */
  public function loadSingle($criterium, $column = NULL, array $conditions = array());

  /**
   * Get a set of objects from database.
   *
   * @param mixed $critera
   *   An array of values to filter by.
   * @param string $column
   *   The column where to search for the given criteria. If NULL,
   *   the default column defined by child class is used.
   * @param array $conditions
   *   An array of conditions to filter the results with.
   *
   * @return array
   *   A list of the retrieved data if any.
   */
  public function load(array $criteria, $column = NULL, array $conditions = array());

  /**
   * Clear the cached data if wrapper supports caching.
   */
  public function clearBinCache();

}
