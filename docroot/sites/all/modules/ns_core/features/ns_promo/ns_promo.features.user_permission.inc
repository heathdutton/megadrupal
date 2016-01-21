<?php
/**
 * @file
 * ns_promo.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_promo_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_promo content
  $permissions['create ns_promo content'] = array(
    'name' => 'create ns_promo content',
    'roles' => array(
      0 => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_promo content
  $permissions['delete any ns_promo content'] = array(
    'name' => 'delete any ns_promo content',
    'roles' => array(
      0 => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_promo content
  $permissions['delete own ns_promo content'] = array(
    'name' => 'delete own ns_promo content',
    'roles' => array(
      0 => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_promo content
  $permissions['edit any ns_promo content'] = array(
    'name' => 'edit any ns_promo content',
    'roles' => array(
      0 => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_promo content
  $permissions['edit own ns_promo content'] = array(
    'name' => 'edit own ns_promo content',
    'roles' => array(
      0 => 'editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
