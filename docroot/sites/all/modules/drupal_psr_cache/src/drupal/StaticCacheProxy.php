<?php
/**
 * @file
 *
 * Static cache proxy.
 */

namespace Drupal\PSRCache;

use Psr\Cache\CacheItemInterface;
use Psr\Cache\CacheItemPoolInterface;

/**
 * Class StaticCacheProxy
 * @package Drupal\PSRCache
 *
 * Static cache proxy is an additional caching layer which using static cache as
 * a first level cache, and accept another cache for second level.
 * This way cache pools can be stacked.
 */
class StaticCacheProxy implements CacheItemPoolInterface {

  /**
   * The second level cache item pool.
   *
   * @var \Psr\Cache\CacheItemPoolInterface
   */
  private $cacheItemPool;

  /**
   * Static cache.
   *
   * @var CacheItemInterface[]
   */
  private static $staticCache = array();

  public function __construct(CacheItemPoolInterface $cacheItemPool) {
    $this->cacheItemPool = $cacheItemPool;
  }

  /**
   * Returns a Cache Item representing the specified key.
   *
   * This method must always return an ItemInterface object, even in case of
   * a cache miss. It MUST NOT return null.
   *
   * @param string $key
   *   The key for which to return the corresponding Cache Item.
   * @return \Psr\Cache\CacheItemInterface
   *   The corresponding Cache Item.
   * @throws \Psr\Cache\InvalidArgumentException
   *   If the $key string is not a legal value a \Psr\Cache\InvalidArgumentException
   *   MUST be thrown.
   */
  public function getItem($key) {
    if (!isset(self::$staticCache[$key])) {
      $item = $this->cacheItemPool->getItem($key);
      self::$staticCache[$key] = $item;
    }

    return self::$staticCache[$key];
  }

  /**
   * Returns a traversable set of cache items.
   *
   * @param array $keys
   * An indexed array of keys of items to retrieve.
   * @return array|\Traversable
   * A traversable collection of Cache Items keyed by the cache keys of
   * each item. A Cache item will be returned for each key, even if that
   * key is not found. However, if no keys are specified then an empty
   * traversable MUST be returned instead.
   */
  public function getItems(array $keys = array()) {
    $items = array();
    foreach ($keys as $key) {
      $items[] = $this->getItem($key);
    }
    return $items;
  }

  /**
   * Deletes all items in the pool.
   *
   * @return boolean
   *   True if the pool was successfully cleared. False if there was an error.
   */
  public function clear() {
    $this->cacheItemPool->clear();
    self::$staticCache = array();
  }

  /**
   * Removes multiple items from the pool.
   *
   * @param array $keys
   * An array of keys that should be removed from the pool.
   * @return static
   * The invoked object.
   */
  public function deleteItems(array $keys) {
    foreach ($keys as $key) {
      unset(self::$staticCache[$key]);
    }
    $this->cacheItemPool->deleteItems($keys);
  }

  /**
   * Persists a cache item immediately.
   *
   * @param CacheItemInterface $item
   *   The cache item to save.
   *
   * @return static
   *   The invoked object.
   */
  public function save(CacheItemInterface $item) {
    $this->cacheItemPool->save($item);
  }

  /**
   * Sets a cache item to be persisted later.
   *
   * @param CacheItemInterface $item
   *   The cache item to save.
   * @return static
   *   The invoked object.
   */
  public function saveDeferred(CacheItemInterface $item) {
    $this->cacheItemPool->saveDeferred($item);
  }

  /**
   * Persists any deferred cache items.
   *
   * @return bool
   *   TRUE if all not-yet-saved items were successfully saved. FALSE otherwise.
   */
  public function commit() {
    $this->cacheItemPool->commit();
  }

}
