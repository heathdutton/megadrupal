<?php
/**
 * @file
 * inspect_test.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function inspect_test_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create inspect_test content'.
  $permissions['create inspect_test content'] = array(
    'name' => 'create inspect_test content',
    'roles' => array(
      'inspect_test_editor' => 'inspect_test_editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own inspect_test content'.
  $permissions['delete own inspect_test content'] = array(
    'name' => 'delete own inspect_test content',
    'roles' => array(
      'inspect_test_editor' => 'inspect_test_editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own inspect_test content'.
  $permissions['edit own inspect_test content'] = array(
    'name' => 'edit own inspect_test content',
    'roles' => array(
      'inspect_test_editor' => 'inspect_test_editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
