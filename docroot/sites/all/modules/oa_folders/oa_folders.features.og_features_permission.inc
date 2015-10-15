<?php
/**
 * @file
 * oa_folders.features.og_features_permission.inc
 */

/**
 * Implements hook_og_features_default_permissions().
 */
function oa_folders_og_features_default_permissions() {
  $permissions = array();

  // Exported og permission: 'node:oa_group:create oa_folder content'
  $permissions['node:oa_group:create oa_folder content'] = array(
    'roles' => array(
      'administrator member' => 'administrator member',
    ),
  );

  // Exported og permission: 'node:oa_space:create oa_folder content'
  $permissions['node:oa_space:create oa_folder content'] = array(
    'roles' => array(
      'administrator member' => 'administrator member',
      'member' => 'member',
    ),
  );

  return $permissions;
}
