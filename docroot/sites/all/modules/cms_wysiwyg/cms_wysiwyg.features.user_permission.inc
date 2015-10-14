<?php
/**
 * @file
 * cms_wysiwyg.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function cms_wysiwyg_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access media browser'.
  $permissions['access media browser'] = array(
    'name' => 'access media browser',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'media',
  );

  return $permissions;
}
