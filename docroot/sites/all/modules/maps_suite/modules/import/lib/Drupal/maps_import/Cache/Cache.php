<?php

/**
 * @file
 * Define the MaPS Import Cache class.
 */

namespace Drupal\maps_import\Cache;

use Drupal\maps_import\Exception\CacheException;

/**
 * MaPS Import Cache class.
 */
abstract class Cache implements CacheInterface {

  /**
   * A reference to the static cache.
   *
   * @var &array
   */
  protected $cache = array();

  /**
   * Do not allow class instanciation.
   *
   * Singleton design pattern.
   */
  protected function __construct() {
    static $drupal_static_fast;

    if (!isset($drupal_static_fast)) {
      $drupal_static_fast['cache'] = &drupal_static(get_class($this));
    }

    $this->cache = &$drupal_static_fast['cache'];

    if (!isset($this->cache)) {
      $this->cache = array();
    }
  }

  /**
   * @inheritdoc
   *
   * @return Drupal\maps_import\Cache\CacheInterface
   */
  public static function getInstance() {
    static $instance = array();

    $class = get_called_class();

    if (!isset($instance[$class])) {
      $instance[$class] = new $class();
    }

    return $instance[$class];
  }

  /**
   * @inheritdoc
   */
  public function clearBinCache() {
    if ($this->useCacheBin()) {
      cache_clear_all($this->getCacheKey(), 'cache_maps_suite');
    }

    $this->cache[$this->getCacheKey()] = array();
  }

  /**
   * Whether database caching is enabled or not.
   *
   * @return boolean
   */
  protected function useCacheBin() {
    return FALSE;
  }

  /**
   * Get the cache key that contains the cached data for a given column.
   *
   * @return string
   *   The cache key.
   */
  protected function getColumnCacheKey($column) {
    $cid = array($this->getCacheKey());

    if ($column !== $this->getDefaultColumn()) {
      $cid[] = $column;

      if ($this->isLocalisable()) {
        $cid[] = $this->getLanguageId();
      }
    }

    return implode(':', $cid);
  }

  /**
   * Get the cache key that contains the cached data.
   *
   * @return string
   *   The cache key.
   */
  abstract protected function getCacheKey();

  /**
   * @return boolean
   *   Whether the data may contain translated strings or not.
   */
  protected function isLocalisable() {
    return FALSE;
  }

  /**
   * Get the MaPS System® language ID that should be used for retrieving
   * the data.
   *
   * This only applies if the isLocalisable method returns TRUE.
   *
   * @return int
   *   The MaPS System® language ID.
   */
  protected function getLanguageId() {
    if (!$this->isLocalisable()) {
      throw new CacheException('The "getLanguageId" method should only be called if the cache class is localizable.');
    }

    return 0;
  }

}
