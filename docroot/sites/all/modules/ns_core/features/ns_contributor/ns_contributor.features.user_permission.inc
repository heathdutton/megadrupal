<?php
/**
 * @file
 * ns_contributor.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_contributor_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_contributor content
  $permissions['create ns_contributor content'] = array(
    'name' => 'create ns_contributor content',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_contributor content
  $permissions['delete any ns_contributor content'] = array(
    'name' => 'delete any ns_contributor content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_contributor content
  $permissions['delete own ns_contributor content'] = array(
    'name' => 'delete own ns_contributor content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_contributor content
  $permissions['edit any ns_contributor content'] = array(
    'name' => 'edit any ns_contributor content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_contributor content
  $permissions['edit own ns_contributor content'] = array(
    'name' => 'edit own ns_contributor content',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
    ),
    'module' => 'node',
  );

  return $permissions;
}
