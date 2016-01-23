<?php
/**
 * @file
 * bassets_server_user.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function bassets_server_user_user_default_roles() {
  $roles = array();

  // Exported role: Bassets Connection.
  $roles['Bassets Connection'] = array(
    'name' => 'Bassets Connection',
    'weight' => 3,
  );

  // Exported role: Bassets admin.
  $roles['Bassets admin'] = array(
    'name' => 'Bassets admin',
    'weight' => 4,
  );

  // Exported role: Bassets editor.
  $roles['Bassets editor'] = array(
    'name' => 'Bassets editor',
    'weight' => 5,
  );

  return $roles;
}
