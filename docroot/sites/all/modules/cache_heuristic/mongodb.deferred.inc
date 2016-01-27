<?php
/**
 * @file
 * MongoDB cache backend deferred.
 */

/**
 * MonogoDB cache implementation.
 *
 * This is Drupal's default cache implementation. It uses the MongoDB to store
 * cached data. Each cache bin corresponds to a collection by the same name.
 */
class DrupalMongoDBCacheDeferred extends DrupalMongoDBCache {
  /**
   * Get one item, without unserializing it, etc.
   */
  function getUnprepared($cid) {
    $collection = mongodb_collection($this->bin);
    // Garbage collection necessary when enforcing a minimum cache lifetime.
    $this->garbageCollection($this->bin);
    $cache = $collection->findOne(array('_id' => (string)$cid));
    return $cache;
  }

  /**
   * Get multiple items, without unserializing them, etc.
   */
  function getMultipleUnprepared(&$cids) {
    $collection = mongodb_collection($this->bin);
    // Garbage collection necessary when enforcing a minimum cache lifetime.
    $this->garbageCollection($this->bin);
    $find = array();
    $find['_id']['$in'] = array_map('strval', $cids);
    $result = $collection->find($find);
    $cache = array();
    foreach ($result as $item) {
      if ($item) {
        $cache[$item->cid] = $item;
      }
    }
    $cids = array_diff($cids, array_keys($cache));
    return $cache;
  }

  /**
   * Implements DrupalCacheDeferredInterface::getMultipleDeferred().
   */
  public function getMultipleDeferred($cids) {
    if (count($cids) == 1) {
      $cid = reset($cids);
      $result = $this->getUnprepared($cid);
      return $result ? array($cid => $result) : FALSE;
    }
    return $this->getMultipleUnprepared($cids);
  }

  /**
   * Implements DrupalCacheDeferredInterface::prepareDeferredItem().
   */
  public function prepareDeferredItem($cid, $item) {
    if ($item->serialized) {
      $item->data = unserialize($item->data);
    }
    return $item;
  }

  /**
   * Implements DrupalCacheDeferredInterface::fetchDeferredItems().
   */
  public function fetchDeferredItems($cids, $results) {
    $results = $results ? $results : array();
    foreach ($results as $cid => &$result) {
      if (empty($result)) {
        unset($results[$cid]);
      }
    }
    return $results;
  }

}
