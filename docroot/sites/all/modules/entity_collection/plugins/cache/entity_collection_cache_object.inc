<?php
/**
 * @file
 * An entity_collection cache indirection mechanism
 * that just uses the basic object cache.
 */

$plugin = array(
  // Cache plugins are the rare plugin types that have no real UI but
  // we're providing a title just in case.
  'title' => t('Region object cache'),
  'cache get' => 'entity_collection_cache_object_get',
  'cache set' => 'entity_collection_cache_object_set',
  'cache clear' => 'entity_collection_cache_object_clear',
  'cache finalize' => 'entity_collection_cache_object_set',
);

/**
 * Get a cache object.
 */
function entity_collection_cache_object_get($data, $key) {
  // Ensure that if there is somehow no data, we at least don't stomp on other
  // people's caches.
  if (empty($data)) {
    $data = 'simple_cache_plugin';
  }
  ctools_include('object-cache');
  return ctools_object_cache_get($data, $key);
}

/**
 * Set the cache object.
 */
function entity_collection_cache_object_set($data, $key, $object) {
  // Ensure that if there is somehow no data, we at least don't stomp on other
  // people's caches.
  if (empty($data)) {
    $data = 'simple_cache_plugin';
  }
  ctools_include('object-cache');
  return ctools_object_cache_set($data, $key, $object);
}

/**
 * Clear the cache for an object.
 */
function entity_collection_cache_object_clear($data, $key) {
  // Ensure that if there is somehow no data, we at least don't stomp on other
  // people's caches.
  if (empty($data)) {
    $data = 'simple_cache_plugin';
  }
  ctools_include('object-cache');
  return ctools_object_cache_clear($data, $key);
}
