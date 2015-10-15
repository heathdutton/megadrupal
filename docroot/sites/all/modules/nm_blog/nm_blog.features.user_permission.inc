<?php
/**
 * @file
 * nm_blog.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function nm_blog_user_default_permissions() {
  $permissions = array();

  // Exported permission: create nm_blog content.
  $permissions['create nm_blog content'] = array(
    'name' => 'create nm_blog content',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own nm_blog content.
  $permissions['edit own nm_blog content'] = array(
    'name' => 'edit own nm_blog content',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
    ),
    'module' => 'node',
  );

  return $permissions;
}
