<?php
/**
 * @file
 * crm_core_event.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function crm_core_event_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'register for events'.
  $permissions['register for events'] = array(
    'name' => 'register for events',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'crm_core_event',
  );

  return $permissions;
}
