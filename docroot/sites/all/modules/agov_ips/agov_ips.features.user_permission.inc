<?php
/**
 * @file
 * agov_ips.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function agov_ips_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create information_publication_scheme content'.
  $permissions['create information_publication_scheme content'] = array(
    'name' => 'create information_publication_scheme content',
    'roles' => array(
      'Content approver' => 'Content approver',
      'Content editor' => 'Content editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any information_publication_scheme content'.
  $permissions['edit any information_publication_scheme content'] = array(
    'name' => 'edit any information_publication_scheme content',
    'roles' => array(
      'Content approver' => 'Content approver',
      'Content editor' => 'Content editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
