<?php
/**
 * @file
 * cod_wysiwyg.features.user_permission.inc
 */

/**
 * Implements hook_user_initial_permissions().
 */
function cod_wysiwyg_user_initial_permissions() {
  $permissions = array();

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
