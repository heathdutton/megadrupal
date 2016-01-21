<?php
/**
 * @file
 * ns_page.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_page_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_page content
  $permissions['create ns_page content'] = array(
    'name' => 'create ns_page content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer'
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_page content
  $permissions['delete any ns_page content'] = array(
    'name' => 'delete any ns_page content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_page content
  $permissions['delete own ns_page content'] = array(
    'name' => 'delete own ns_page content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_page content
  $permissions['edit any ns_page content'] = array(
    'name' => 'edit any ns_page content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_page content
  $permissions['edit own ns_page content'] = array(
    'name' => 'edit own ns_page content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer'
    ),
    'module' => 'node',
  );

  return $permissions;
}
