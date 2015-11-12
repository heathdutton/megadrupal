<?php
/**
 * @file
 * nm_testimonials.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function nm_testimonials_user_default_permissions() {
  $permissions = array();

  // Exported permission: create nm_testimonial content.
  $permissions['create nm_testimonial content'] = array(
    'name' => 'create nm_testimonial content',
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
