<?php
/**
 * @file
 * Entity recur hooks for the node module.
 */

/**
 * Implements hook_entity_recur_access_alter().
 */
function node_entity_recur_access_alter(&$access, $entity_type, $entity) {
  if ($entity_type == 'node') {
    // Only give access to published nodes, unless the user has the
    // 'administer nodes' permission.
    if ($access) {
      $access = $entity->status || user_access('administer nodes');
    }
  }
}

/**
 * Implements ENTITY_TYPE_entity_recur_recur_menu_path()
 */
function node_entity_recur_recur_menu_path() {
  return 'node/%node/recur';
}