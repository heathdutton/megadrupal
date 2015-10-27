<?php
/**
 * @file
 * cp_clone.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function cp_clone_user_default_permissions() {
  $permissions = array();

  // Exported permission: clone node.
  $permissions['clone node'] = array(
    'name' => 'clone node',
    'roles' => array(
      0 => 'administrator',
      1 => 'teacher',
    ),
    'module' => 'clone',
  );

  // Exported permission: clone own nodes.
  $permissions['clone own nodes'] = array(
    'name' => 'clone own nodes',
    'roles' => array(
      0 => 'administrator',
      1 => 'teacher',
    ),
    'module' => 'clone',
  );

  return $permissions;
}
