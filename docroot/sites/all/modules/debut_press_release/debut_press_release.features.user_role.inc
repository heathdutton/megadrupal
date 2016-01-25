<?php
/**
 * @file
 * debut_press_release.features.user_role.inc
 */

/**
 * Implementation of hook_user_default_roles().
 */
function debut_press_release_user_default_roles() {
  $roles = array();

  // Exported role: contributor
  $roles['contributor'] = array(
    'name' => 'contributor',
    'weight' => '3',
  );

  // Exported role: editor
  $roles['editor'] = array(
    'name' => 'editor',
    'weight' => '4',
  );

  return $roles;
}
