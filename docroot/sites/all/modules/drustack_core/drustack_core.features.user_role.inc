<?php
/**
 * @file
 * drustack_core.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function drustack_core_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 2,
  );

  // Exported role: power user.
  $roles['power user'] = array(
    'name' => 'power user',
    'weight' => 3,
  );

  return $roles;
}
