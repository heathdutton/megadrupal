<?php
/**
 * @file
 * recruiter_common.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function recruiter_common_user_default_permissions() {
  $permissions = array();

  // Exported permission: access dashboard.
  $permissions['access dashboard'] = array(
    'name' => 'access dashboard',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'dashboard',
  );

  // Exported permission: show format selection for node.
  $permissions['show format selection for node'] = array(
    'name' => 'show format selection for node',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'better_formats',
  );

  // Exported permission: use text format full_wysiwyg_html.
  $permissions['use text format full_wysiwyg_html'] = array(
    'name' => 'use text format full_wysiwyg_html',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'filter',
  );

  return $permissions;
}
