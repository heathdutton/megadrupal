<?php
/**
 * @file
 * codebook_wysiwyg.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function codebook_wysiwyg_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'use text format codebook'.
  $permissions['use text format codebook'] = array(
    'name' => 'use text format codebook',
    'roles' => array(
      'Administrator' => 'Administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
