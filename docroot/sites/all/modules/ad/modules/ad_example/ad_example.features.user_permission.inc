<?php
/**
 * @file
 * ad_example.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ad_example_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create ad_example content'.
  $permissions['create ad_example content'] = array(
    'name' => 'create ad_example content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any ad_example content'.
  $permissions['delete any ad_example content'] = array(
    'name' => 'delete any ad_example content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own ad_example content'.
  $permissions['delete own ad_example content'] = array(
    'name' => 'delete own ad_example content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any ad_example content'.
  $permissions['edit any ad_example content'] = array(
    'name' => 'edit any ad_example content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own ad_example content'.
  $permissions['edit own ad_example content'] = array(
    'name' => 'edit own ad_example content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}
