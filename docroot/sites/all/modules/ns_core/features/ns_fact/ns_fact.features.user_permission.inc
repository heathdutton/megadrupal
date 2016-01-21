<?php
/**
 * @file
 * ns_fact.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_fact_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: create ns_fact content
  $permissions['create ns_fact content'] = array(
    'name' => 'create ns_fact content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any ns_fact content
  $permissions['delete any ns_fact content'] = array(
    'name' => 'delete any ns_fact content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_fact content
  $permissions['delete own ns_fact content'] = array(
    'name' => 'delete own ns_fact content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any ns_fact content
  $permissions['edit any ns_fact content'] = array(
    'name' => 'edit any ns_fact content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_fact content
  $permissions['edit own ns_fact content'] = array(
    'name' => 'edit own ns_fact content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer',
    ),
    'module' => 'node',
  );

  return $permissions;
}
