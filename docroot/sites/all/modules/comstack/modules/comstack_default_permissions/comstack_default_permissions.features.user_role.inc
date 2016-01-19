<?php
/**
 * @file
 * comstack_default_permissions.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function comstack_default_permissions_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 3,
  );

  // Exported role: moderator.
  $roles['moderator'] = array(
    'name' => 'moderator',
    'weight' => 2,
  );

  return $roles;
}
