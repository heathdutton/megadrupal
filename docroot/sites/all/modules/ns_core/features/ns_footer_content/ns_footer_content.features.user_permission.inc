<?php
/**
 * @file
 * ns_footer_content.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_footer_content_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_footer_content content
  $permissions['create ns_footer_content content'] = array(
    'name' => 'create ns_footer_content content',
    'roles' => array(
      0 => 'chief editor',
      1 => 'super user',
      2 => 'administrator'
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_footer_content content
  $permissions['delete any ns_footer_content content'] = array(
    'name' => 'delete any ns_footer_content content',
    'roles' => array(
      0 => 'chief editor',
      1 => 'super user',
      2 => 'administrator'
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_footer_content content
  $permissions['delete own ns_footer_content content'] = array(
    'name' => 'delete own ns_footer_content content',
    'roles' => array(
      0 => 'chief editor',
      1 => 'super user',
      2 => 'administrator'
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_footer_content content
  $permissions['edit any ns_footer_content content'] = array(
    'name' => 'edit any ns_footer_content content',
    'roles' => array(
      0 => 'chief editor',
      1 => 'administrator',
      2 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_footer_content content
  $permissions['edit own ns_footer_content content'] = array(
    'name' => 'edit own ns_footer_content content',
    'roles' => array(
      0 => 'chief editor',
      1 => 'administrator',
      2 => 'super user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
