<?php
/**
 * @file
 *
 * Drupal cache pool.
 */

namespace Drupal\PSRCache;

use Drupal\PSRCache\Adaptor\DefaultDrupalCacheHandler;
use Drupal\PSRCache\Adaptor\DrupalCacheHandler;
use Psr\Cache\CacheItemInterface;
use Psr\Cache\CacheItemPoolInterface;

/**
 * Class CachePool
 * @package Drupal\PSRCache
 *
 * Drupal's PSR cache pool implementation using Drupal's Cache API.
 */
class CachePool implements CacheItemPoolInterface {

  // Default Drupal cache bin.
  const DEFAULT_BIN = 'cache';

  /**
   * @var CacheItemInterface[]
   */
  protected $deferred;

  /**
   * @var string
   */
  protected $bin = self::DEFAULT_BIN;

  /**
   * @var DrupalCacheHandler
   */
  protected $cacheHandler;

  /**
   * @param $bin
   */
  public function setBin($bin) {
    $this->bin = $bin;
  }

  /**
   * @return string
   */
  public function getBin() {
    return $this->bin;
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
    $cacheData = $this->getCacheHandler()->cacheGet($key, $this->getBin());
    $item = new CacheItem(
      $key,
      isset($cacheData->data) ? $cacheData->data : NULL,
      $this->getBin(),
      isset($cacheData->expire) ? $cacheData->expire : DefaultDrupalCacheHandler::CACHE_PERMANENT
    );
    return $item;
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
    $this->getCacheHandler()->cacheClearAll('*', $this->getBin(), TRUE);
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
      $this->getCacheHandler()->cacheClearAll($key, $this->getBin(), TRUE);
    }
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
    $this->getCacheHandler()->cacheSet($item->getKey(), $item->get(), $this->getBin(), $item->getExpiration());
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
    $hash = spl_object_hash($item);
    $this->deferred[$hash] = $item;
  }

  /**
   * Persists any deferred cache items.
   *
   * @return bool
   *   TRUE if all not-yet-saved items were successfully saved. FALSE otherwise.
   */
  public function commit() {
    foreach ($this->deferred as $deferred) {
      $this->save($deferred);
    }
  }

  /**
   * @return DrupalCacheHandler
   */
  public function getCacheHandler() {
    if (empty($this->cacheHandler)) {
      $this->setCacheHandler(new DefaultDrupalCacheHandler());
    }
    return $this->cacheHandler;
  }

  /**
   * Opportunity to set the default Drupal cache handler. (Eg. mocking for tests.)
   *
   * @param DrupalCacheHandler $cacheHandler
   */
  public function setCacheHandler($cacheHandler) {
    $this->cacheHandler = $cacheHandler;
  }

}
