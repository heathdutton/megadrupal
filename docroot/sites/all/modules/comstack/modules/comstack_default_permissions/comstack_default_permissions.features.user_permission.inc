<?php
/**
 * @file
 * comstack_default_permissions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function comstack_default_permissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer comstack content'.
  $permissions['administer comstack content'] = array(
    'name' => 'administer comstack content',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'comstack',
  );

  // Exported permission: 'administer comstack settings'.
  $permissions['administer comstack settings'] = array(
    'name' => 'administer comstack settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'comstack',
  );

  // Exported permission: 'administer comstack_message_types'.
  $permissions['administer comstack_message_types'] = array(
    'name' => 'administer comstack_message_types',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'comstack',
  );

  // Exported permission: 'bypass comstack access control'.
  $permissions['bypass comstack access control'] = array(
    'name' => 'bypass comstack access control',
    'roles' => array(
      'administrator' => 'administrator',
      'moderator' => 'moderator',
    ),
    'module' => 'comstack',
  );

  // Exported permission: 'view comstack conversations'.
  $permissions['view comstack conversations'] = array(
    'name' => 'view comstack conversations',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comstack',
  );

  // Exported permission: 'view comstack messages'.
  $permissions['view comstack messages'] = array(
    'name' => 'view comstack messages',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'comstack',
  );

  return $permissions;
}
