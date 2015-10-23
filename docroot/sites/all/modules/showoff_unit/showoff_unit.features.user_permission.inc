<?php
/**
 * @file
 * showoff_unit.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function showoff_unit_user_default_permissions() {
  $permissions = array();

  // Exported permission: create unit content.
  $permissions['create unit content'] = array(
    'name' => 'create unit content',
    'roles' => array(
      'Content Administrator' => 'Content Administrator',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any unit content.
  $permissions['delete any unit content'] = array(
    'name' => 'delete any unit content',
    'roles' => array(
      'Content Administrator' => 'Content Administrator',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own unit content.
  $permissions['delete own unit content'] = array(
    'name' => 'delete own unit content',
    'roles' => array(
      'Content Administrator' => 'Content Administrator',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any unit content.
  $permissions['edit any unit content'] = array(
    'name' => 'edit any unit content',
    'roles' => array(
      'Content Administrator' => 'Content Administrator',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own unit content.
  $permissions['edit own unit content'] = array(
    'name' => 'edit own unit content',
    'roles' => array(
      'Content Administrator' => 'Content Administrator',
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  return $permissions;
}
