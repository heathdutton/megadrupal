<?php
/**
 * @file
 * restaurant_blog.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function restaurant_blog_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'delete any blog_post content'.
  $permissions['delete any blog_post content'] = array(
    'name' => 'delete any blog_post content',
    'roles' => array(),
    'module' => 'node',
  );

  return $permissions;
}
