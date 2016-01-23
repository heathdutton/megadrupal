<?php
/**
 * @file
 * commonmark_feature.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function commonmark_feature_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'use text format commonmark'.
  $permissions['use text format commonmark'] = array(
    'name' => 'use text format commonmark',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
