<?php
/**
 * @file
 * openacademy_people.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function openacademy_people_defaultconfig_features() {
  return array(
    'openacademy_people' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function openacademy_people_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create openacademy_person content'.
  $permissions['create openacademy_person content'] = array(
    'name' => 'create openacademy_person content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any openacademy_person content'.
  $permissions['delete any openacademy_person content'] = array(
    'name' => 'delete any openacademy_person content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own openacademy_person content'.
  $permissions['delete own openacademy_person content'] = array(
    'name' => 'delete own openacademy_person content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any openacademy_person content'.
  $permissions['edit any openacademy_person content'] = array(
    'name' => 'edit any openacademy_person content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own openacademy_person content'.
  $permissions['edit own openacademy_person content'] = array(
    'name' => 'edit own openacademy_person content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
