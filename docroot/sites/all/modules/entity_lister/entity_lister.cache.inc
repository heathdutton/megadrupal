<?php

/**
 * @file
 * Entity Lister cache-related functions.
 */

/**
 * Clear the cache.
 *
 * @param object $entity
 *   The entity being saved or deleted. It's not clear we'll ever need this
 *   but we get it for free in the in presave and delete hooks.
 * @param string $type
 *   The entity type.
 */
function entity_lister_cache_clear($entity, $type) {
  /* When any entity is saved or deleted, we have to clear all lists featuring
   * that entity type. We can't be sure the bundle (for an item whose presave
   * clears the cache) will be the first one in the $cid, so we can't consider
   * the bundle when clearing the cache.
   */
  cache_clear_all('entity_lister:' . $type . ':', 'cache', TRUE);
}

/**
 * Implements hook_entity_presave().
 */
function entity_lister_entity_presave($entity, $type) {

  entity_lister_cache_clear($entity, $type);

}

/**
 * Implements hook_entity_delete().
 */
function entity_lister_entity_delete($entity, $type) {

  entity_lister_cache_clear($entity, $type);

}
