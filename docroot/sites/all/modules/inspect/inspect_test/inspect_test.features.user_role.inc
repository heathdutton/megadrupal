<?php
/**
 * @file
 * inspect_test.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function inspect_test_user_default_roles() {
  $roles = array();

  // Exported role: inspect_test_editor.
  $roles['inspect_test_editor'] = array(
    'name' => 'inspect_test_editor',
    'weight' => 0,
  );

  return $roles;
}
