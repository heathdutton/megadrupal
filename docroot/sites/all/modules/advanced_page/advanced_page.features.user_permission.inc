<?php
/**
 * @file
 * advanced_page.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function advanced_page_user_default_permissions() {
  $permissions = array();

  // Exported permission: create url aliases
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'path',
  );

  return $permissions;
}
