<?php
/**
 * @file
 * comstack_friends_default_permissions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function comstack_friends_default_permissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'can have comstack_blocked relationships'.
  $permissions['can have comstack_blocked relationships'] = array(
    'name' => 'can have comstack_blocked relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'can have comstack_friends relationships'.
  $permissions['can have comstack_friends relationships'] = array(
    'name' => 'can have comstack_friends relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'can request comstack_blocked relationships'.
  $permissions['can request comstack_blocked relationships'] = array(
    'name' => 'can request comstack_blocked relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'can request comstack_friends relationships'.
  $permissions['can request comstack_friends relationships'] = array(
    'name' => 'can request comstack_friends relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'delete comstack_blocked relationships'.
  $permissions['delete comstack_blocked relationships'] = array(
    'name' => 'delete comstack_blocked relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'delete comstack_friends relationships'.
  $permissions['delete comstack_friends relationships'] = array(
    'name' => 'delete comstack_friends relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'maintain comstack_blocked relationships'.
  $permissions['maintain comstack_blocked relationships'] = array(
    'name' => 'maintain comstack_blocked relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  // Exported permission: 'maintain comstack_friends relationships'.
  $permissions['maintain comstack_friends relationships'] = array(
    'name' => 'maintain comstack_friends relationships',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'user_relationships',
  );

  return $permissions;
}
