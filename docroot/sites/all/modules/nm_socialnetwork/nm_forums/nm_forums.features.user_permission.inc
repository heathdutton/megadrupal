<?php
/**
 * @file
 * nm_forums.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function nm_forums_user_default_permissions() {
  $permissions = array();

  // Exported permission: create forum content.
  $permissions['create forum content'] = array(
    'name' => 'create forum content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
