<?php
/**
 * @file
 * ideation.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function ideation_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer special idea fields'.
  $permissions['administer special idea fields'] = array(
    'name' => 'administer special idea fields',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ideation',
  );

  // Exported permission: 'create idea_site content'.
  $permissions['create idea_site content'] = array(
    'name' => 'create idea_site content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'editor' => 'editor',
      'staff' => 'staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any idea_site content'.
  $permissions['delete any idea_site content'] = array(
    'name' => 'delete any idea_site content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own idea_site content'.
  $permissions['delete own idea_site content'] = array(
    'name' => 'delete own idea_site content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any idea_site content'.
  $permissions['edit any idea_site content'] = array(
    'name' => 'edit any idea_site content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own idea_site content'.
  $permissions['edit own idea_site content'] = array(
    'name' => 'edit own idea_site content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'staff' => 'staff',
    ),
    'module' => 'node',
  );

  return $permissions;
}
