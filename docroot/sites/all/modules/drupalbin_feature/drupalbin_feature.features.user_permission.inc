<?php
/**
 * @file
 * drupalbin_feature.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function drupalbin_feature_user_default_permissions() {
  $permissions = array();

  // Exported permission: access content
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: clone node
  $permissions['clone node'] = array(
    'name' => 'clone node',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'clone',
  );

  // Exported permission: clone own nodes
  $permissions['clone own nodes'] = array(
    'name' => 'clone own nodes',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'clone',
  );

  // Exported permission: create code content
  $permissions['create code content'] = array(
    'name' => 'create code content',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any code content
  $permissions['delete any code content'] = array(
    'name' => 'delete any code content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own code content
  $permissions['delete own code content'] = array(
    'name' => 'delete own code content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any code content
  $permissions['edit any code content'] = array(
    'name' => 'edit any code content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own code content
  $permissions['edit own code content'] = array(
    'name' => 'edit own code content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
