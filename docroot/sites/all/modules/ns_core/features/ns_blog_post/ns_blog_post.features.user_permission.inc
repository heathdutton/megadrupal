<?php
/**
 * @file
 * ns_blog.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_blog_post_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_blog_post content
  $permissions['create ns_blog_post content'] = array(
    'name' => 'create ns_blog_post content',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_blog_post content
  $permissions['delete any ns_blog_post content'] = array(
    'name' => 'delete any ns_blog_post content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_blog_post content
  $permissions['delete own ns_blog_post content'] = array(
    'name' => 'delete own ns_blog_post content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_blog_post content
  $permissions['edit any ns_blog_post content'] = array(
    'name' => 'edit any ns_blog_post content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_blog_post content
  $permissions['edit own ns_blog_post content'] = array(
    'name' => 'edit own ns_blog_post content',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
