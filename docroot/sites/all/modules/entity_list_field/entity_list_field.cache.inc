<?php

/**
 * @file
 * Entity List Field cache-related functions.
 */

/**
 * Clear the cache.
 *
 * @param object $entity
 *   The entity being saved or deleted.
 * @param string $type
 *   The entity type.
 */
function entity_list_field_cache_clear($entity, $type) {
  /* When any entity is saved or deleted, we have to clear all lists featuring
   * that entity type. We can't be sure the bundle for this particular entity
   * will be the first one in the $cid, so we can't consider the bundle when
   * clearing the cache.
   */
  cache_clear_all('entity_list_field:' . $type . ':', 'cache', TRUE);
}

/**
 * Implements hook_entity_presave().
 */
function entity_list_field_entity_presave($entity, $type) {

  entity_list_field_cache_clear($entity, $type);

}

/**
 * Implements hook_entity_delete().
 */
function entity_list_field_entity_delete($entity, $type) {

  entity_list_field_cache_clear($entity, $type);

}
