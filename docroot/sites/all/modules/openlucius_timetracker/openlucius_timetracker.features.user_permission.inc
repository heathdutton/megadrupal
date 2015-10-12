<?php
/**
 * @file
 * openlucius_timetracker.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function openlucius_timetracker_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer timetracker'.
  $permissions['administer timetracker'] = array(
    'name' => 'administer timetracker',
    'roles' => array(
      'can create groups' => 'can create groups',
    ),
    'module' => 'openlucius_timetracker',
  );

  // Exported permission: 'create time entries'.
  $permissions['create time entries'] = array(
    'name' => 'create time entries',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'openlucius_timetracker',
  );

  // Exported permission: 'view reports page'.
  $permissions['view reports page'] = array(
    'name' => 'view reports page',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'openlucius_timetracker',
  );

  return $permissions;
}
