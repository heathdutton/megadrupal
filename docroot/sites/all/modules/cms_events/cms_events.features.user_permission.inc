<?php
/**
 * @file
 * cms_events.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function cms_events_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create event content'.
  $permissions['create event content'] = array(
    'name' => 'create event content',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own event content'.
  $permissions['delete own event content'] = array(
    'name' => 'delete own event content',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own event content'.
  $permissions['edit own event content'] = array(
    'name' => 'edit own event content',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
