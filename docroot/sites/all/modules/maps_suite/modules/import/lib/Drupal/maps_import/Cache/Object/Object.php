<?php

/**
 * @file
 * Define the Maps Import Cache class that relates to objects.
 */

namespace Drupal\maps_import\Cache\Object;

use Drupal\maps_import\Cache\Cache;
use Drupal\maps_import\Exception\CacheException;

/**
 * The Maps Import Cache class.
 */
abstract class Object extends Cache implements CacheInterface {

  /**
   * Whether all the records have been loaded or not.
   */
  protected $all = FALSE;

  /**
   * @return string
   *   The default column name for the load operations.
   */
  abstract protected function getDefaultColumn();

  /**
   * @return string
   *   The name of the interface that defines the column getter methods.
   */
  abstract protected function getInterfaceName();

  /**
   * Get the requested object instance from cache if they exist.
   *
   * @param $ids
   *   The data ids.
   *
   * @return array
   */
  protected function getCache(array $ids = NULL) {
    $cid = $this->getCacheKey();

    if (!isset($this->cache[$cid])) {
      $this->cache[$cid] = array();
    }

    if (isset($ids)) {
      return array_intersect_key($this->cache[$cid], array_flip($ids));
    }

    return $this->cache[$cid];
  }

  /**
   * Store data into cache.
   *
   * @param $id
   *   The data ID.
   * @param $data
   *   The fetched record.
   *
   * @return mixed
   *   The stored data.
   */
  protected function setCache($id, $record) {
    $cid = $this->getCacheKey();

    // Do not override existing data. Modifying any data should be done using
    // the referenced variable.
    if (!isset($this->cache[$cid][$id])) {
      $this->cache[$cid][$id] = $this->createObject($record);
    }

    return $this->cache[$cid][$id];
  }

  /**
   * @inheritdoc
   */
  public function load(array $criteria, $column = NULL, array $conditions = array()) {
    if (!isset($column)) {
      $column = $this->getDefaultColumn();
    }

    $cid = $this->getColumnCacheKey($column);

    if ($column !== $this->getDefaultColumn()) {
      $results = array();
      $getter = 'get' . maps_suite_drupal2camelcase($column);

      if (!method_exists($this->getInterfaceName(), $getter)) {
        throw new CacheException('The getter @method does not exist in the interface @interface.', 0, array('@method' => $getter, '@interface' => $this->getInterfaceName()));
      }

      // No need to perform any database query if all records have already
      // been loaded.
      if ($this->all) {
        foreach ($this->getCache() as $id => $data) {
          $criterium = $data->{$getter}();

          if (in_array($criterium, $criteria)) {
            $results[$id] = $data;

            if (!isset($this->cache[$cid][$criterium])) {
              $this->cache[$cid][$criterium] = $data;
            }
          }
        }

        return $this->filterResults($results, $conditions);
      }

      foreach ($criteria as $key => $criterium) {
        if (isset($this->cache[$cid][$criterium])) {
          if (!is_array($this->cache[$cid][$criterium])) {
          	$this->cache[$cid][$criterium] = array($this->cache[$cid][$criterium]);
          }
        	$results += $this->cache[$cid][$criterium];
          unset($criteria[$key]);
        }
      }
    }
    else {
      $results = $this->getCache($criteria);
      $criteria = array_diff($criteria, array_keys($results));
    }

    if ($criteria && !$this->all) {
      foreach (maps_suite_get_records($this->getTable(), $this->getDefaultColumn(), array($column => $criteria)) as $id => $record) {
      	$results[$id] = $this->setCache($id, $record);

      	if (isset($getter)) {
          $this->cache[$cid][$results[$id]->{$getter}()][$id] = $results[$id];
        }
      }
    }

    return $this->filterResults($results, $conditions);
  }

  /**
   * @inheritdoc
   */
  public function loadSingle($criterium, $column = NULL, array $conditions = array()) {
    if (!isset($column)) {
      $column = $this->getDefaultColumn();
    }

    if ($objects = $this->load(array($criterium), $column, $conditions)) {
      return reset($objects);
    }

    return FALSE;
  }

  /**
   * @inheritdoc
   */
  public function loadAll() {
    if (!$this->all) {
      $this->all = TRUE;
      $cid = $this->getCacheKey();

      if ($this->useCacheBin()) {
        if ($cached = cache_get($cid, 'cache_maps_suite')) {
          foreach ($cached->data as $id => $data) {
            if (!isset($this->cache[$cid][$id])) {
              $this->cache[$cid][$id] = $data;
            }
          }
        }
      }

      if (!isset($cached) || FALSE === $cached) {
        foreach (maps_suite_get_records($this->getTable(), $this->getDefaultColumn()) as $id => $record) {
          $this->setCache($id, $record);
        }

        if ($this->useCacheBin()) {
          cache_set($cid, $this->getCache(), 'cache_maps_suite');
        }
      }
    }

    return $this->getCache();
  }

  /**
   * Create a new object instance.
   */
  protected function createInstance(&$data) {
    $class = $this->getClassName($data);
    return new $class();
  }

  /**
   * Create an object from the given data.
   *
   * @param $data
   * @param $type
   *
   * @return object
   */
  protected function createObject($data) {
    $object = $this->createInstance($data);

    foreach ($data as $key => $value) {
      $suffix = maps_suite_drupal2camelcase($key);
      $setter = 'set' . $suffix;

      // Cast value to boolean if a isXyz or hasXYZ method exists.
      if (method_exists($object, 'is' . $suffix) || method_exists($object, 'has' . $suffix)) {
        $value = (bool) $value;
      }

      if (method_exists($object, $setter)) {
        call_user_func(array($object, $setter), $value);
      }
    }

    return $object;
  }

  /**
   * Check if the given data matches the specified condition.
   *
   * @param mixed $data
   * @param string $column
   * @param mixed $value
   * @param bool $negate
   *   A flag indicating whether the data should match or not.
   *
   * @return bool
   */
  protected function matchCondition($data, $column, $value, $negate = FALSE) {
    $datum = NULL;

    if (!is_object($data)) {
      return FALSE;
    }

    // @todo similar part of code is used somewhere else. This need
    // to be factorized.
    $method = maps_suite_drupal2camelcase($column);
    $methods = array(
      'get' . $method,
      'is' . $method,
      'has' . $method,
    );

    foreach ($methods as $method) {
      if (is_callable(array($data, $method))) {
        $datum = $data->{$method}();
        break;
      }
    }

    if (is_array($value) && !is_array($datum)) {
      $return = in_array($datum, $value);
    }
    else {
      $return = $value == $datum;
    }

    return !($return xor !$negate);
  }

  /**
   * Filter the loaded results with given conditions.
   *
   * @param array $results
   *   An array of results as returned by the load() method.
   * @param array $conditions
   *
   * @return array
   */
  protected function filterResults(array $results, array $conditions) {
    if (!$conditions) {
      return $results;
    }

    $return = array();

    foreach ($results as $id => $result) {
      foreach ($conditions as $column => $value) {
        if (!$this->matchCondition($data, $column, $value)) {
          continue 2;
        }
      }

      $return[$id] = $result;
    }

    return $return;
  }

  /**
   * @inheritdoc
   */
  public function clearBinCache() {
    parent::clearBinCache();
    $this->all = FALSE;
  }

  /**
   * @return string
   *   The table which holds the data.
   */
  abstract protected function getTable();

  /**
   * @return string
   *   The class name of the object to instanciate.
   */
  abstract protected function getClassName($data);

}
