<?php
/**
 * @file
 * nm_members.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function nm_members_user_default_permissions() {
  $permissions = array();

  // Exported permission: access user profiles.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: edit own nm_member_profile profile.
  $permissions['edit own nm_member_profile profile'] = array(
    'name' => 'edit own nm_member_profile profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view any nm_member_profile profile.
  $permissions['view any nm_member_profile profile'] = array(
    'name' => 'view any nm_member_profile profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view own nm_member_profile profile.
  $permissions['view own nm_member_profile profile'] = array(
    'name' => 'view own nm_member_profile profile',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'profile2',
  );

  return $permissions;
}
