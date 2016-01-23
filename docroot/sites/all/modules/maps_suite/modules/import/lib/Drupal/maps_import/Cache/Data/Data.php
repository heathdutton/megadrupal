<?php

/**
 * @file
 * Define the Maps Import Cache class that relates to data.
 */

namespace Drupal\maps_import\Cache\Data;

use Drupal\maps_import\Cache\Cache;
use Drupal\maps_suite\Exception\Exception;

/**
 * The Maps Import Cache class.
 */
abstract class Data extends Cache {

  /**
   * Whether to use drupal_static() or not.
   *
   * Using drupal_static allow contributed module to interact with
   * the static data.
   *
   * @var bool
   */
  protected $static = FALSE;

  /**
   * @inheritdoc
   *
   * Cached data always use cache bin.
   */
  final protected function useCacheBin() {
    return TRUE;
  }

  /**
   * Build the data.
   *
   * @param array $options
   *   An optional array, whose keys and values depend on the cached
   *   data. See specialized child classes for more information.
   *
   * @return array
   *   The built data, ready for storage in cache.
   */
  abstract protected function buildData(array $options = array());

  /**
   * Get the cache ID for clearing all related cache entries.
   *
   * @return string
   */
  abstract protected function getWildcard();

  /**
   * @inheritdoc
   */
  public function load(array $options = array()) {
    try {
      $cid = $this->getWildcard() . $this->getCacheKey($options);

      if (!isset($this->cache[$cid])) {
        if ($this->static) {
          $this->cache[$cid] = &drupal_static($cid);
        }

        // Initialize the cache so even if some error occurs, the buildData()
        // method will not be called twice.
        $this->cache[$cid] = array();

        if ($cached = cache_get($cid, 'cache_maps_suite')) {
          $this->cache[$cid] = $cached->data;
        }

        if (!isset($cached) || FALSE === $cached) {

          $this->cache[$cid] = $this->buildData($options);
          cache_set($cid, $this->cache[$cid], 'cache_maps_suite');
        }
      }

      return $this->cache[$cid];
    }
    catch (Exception $e) {
      $e->watchdog();
    }
    catch (\Exception $e) {
      watchdog_exception('maps_suite', $e);
    }

    return array();
  }

  /**
   * @inheritdoc
   */
  public function loadSingle($key, array $options = array()) {
    if ($data = $this->load($options)) {
      return isset($data[$key]) ? $data[$key] : NULL;
    }

    return NULL;
  }

  /**
   * @inheritdoc
   */
  public function clearBinCache($all = TRUE, array $options = array()) {
    if ($all) {
      cache_clear_all($this->getWildcard(), 'cache_maps_suite', TRUE);
      $this->cache = array();
    }
    else {
      cache_clear_all($this->getCacheKey($options), 'cache_maps_suite');
      $this->cache[$this->getCacheKey()] = array();
    }
  }

}
