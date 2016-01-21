<?php
/**
 * @file
 * sprout_users.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function sprout_users_user_default_roles() {
  $roles = array();

  // Exported role: content editor.
  $roles['content editor'] = array(
    'name' => 'content editor',
    'weight' => 2,
  );

  // Exported role: site manager.
  $roles['site manager'] = array(
    'name' => 'site manager',
    'weight' => 3,
  );

  return $roles;
}
