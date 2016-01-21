<?php
/**
 * @file
 * ns_ad.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_ad_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_ad content
  $permissions['create ns_ad content'] = array(
    'name' => 'create ns_ad content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_ad content
  $permissions['delete any ns_ad content'] = array(
    'name' => 'delete any ns_ad content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_ad content
  $permissions['delete own ns_ad content'] = array(
    'name' => 'delete own ns_ad content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_ad content
  $permissions['edit any ns_ad content'] = array(
    'name' => 'edit any ns_ad content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_ad content
  $permissions['edit own ns_ad content'] = array(
    'name' => 'edit own ns_ad content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
