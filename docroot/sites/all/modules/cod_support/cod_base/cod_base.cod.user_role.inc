<?php
/**
 * @file
 * cod_base.features.user_role.inc
 */

/**
 * Implements hook_user_initial_roles().
 */
function cod_base_user_initial_roles() {
  $roles = array();

  // Exported role: content editor.
  $roles['content editor'] = array(
    'name' => 'content editor',
    'weight' => 3,
  );

  return $roles;
}
