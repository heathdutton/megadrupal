<?php
/**
 * @file
 * openacademy_events.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function openacademy_events_defaultconfig_features() {
  return array(
    'openacademy_events' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function openacademy_events_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create openacademy_event content'.
  $permissions['create openacademy_event content'] = array(
    'name' => 'create openacademy_event content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any openacademy_event content'.
  $permissions['delete any openacademy_event content'] = array(
    'name' => 'delete any openacademy_event content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own openacademy_event content'.
  $permissions['delete own openacademy_event content'] = array(
    'name' => 'delete own openacademy_event content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any openacademy_event content'.
  $permissions['edit any openacademy_event content'] = array(
    'name' => 'edit any openacademy_event content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own openacademy_event content'.
  $permissions['edit own openacademy_event content'] = array(
    'name' => 'edit own openacademy_event content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
