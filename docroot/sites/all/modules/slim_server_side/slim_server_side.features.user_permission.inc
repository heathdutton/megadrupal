<?php
/**
 * @file
 * slim_server_side.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function slim_server_side_user_default_permissions() {
  $permissions = array();

  // Exported permission: create things content.
  $permissions['create things content'] = array(
    'name' => 'create things content',
    'roles' => array(
      0 => 'mobile user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own things content.
  $permissions['delete own things content'] = array(
    'name' => 'delete own things content',
    'roles' => array(
      0 => 'mobile user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own things content.
  $permissions['edit own things content'] = array(
    'name' => 'edit own things content',
    'roles' => array(
      0 => 'mobile user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
