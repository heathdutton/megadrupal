<?php
/**
 * @file
 * cms_blog.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function cms_blog_user_default_roles() {
  $roles = array();

  // Exported role: Blogger.
  $roles['Blogger'] = array(
    'name' => 'Blogger',
    'weight' => 2,
  );

  return $roles;
}
