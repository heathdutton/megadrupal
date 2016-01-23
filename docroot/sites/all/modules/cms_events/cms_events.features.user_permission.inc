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

  // Exported permission: 'administer cms_events_registration registration'.
  $permissions['administer cms_events_registration registration'] = array(
    'name' => 'administer cms_events_registration registration',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'administer own cms_events_registration registration'.
  $permissions['administer own cms_events_registration registration'] = array(
    'name' => 'administer own cms_events_registration registration',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create cms_events_registration registration'.
  $permissions['create cms_events_registration registration'] = array(
    'name' => 'create cms_events_registration registration',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create cms_events_registration registration other anonymous'.
  $permissions['create cms_events_registration registration other anonymous'] = array(
    'name' => 'create cms_events_registration registration other anonymous',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create cms_events_registration registration other users'.
  $permissions['create cms_events_registration registration other users'] = array(
    'name' => 'create cms_events_registration registration other users',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'create event content'.
  $permissions['create event content'] = array(
    'name' => 'create event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any cms_events_registration registration'.
  $permissions['delete any cms_events_registration registration'] = array(
    'name' => 'delete any cms_events_registration registration',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'delete own cms_events_registration registration'.
  $permissions['delete own cms_events_registration registration'] = array(
    'name' => 'delete own cms_events_registration registration',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'delete own event content'.
  $permissions['delete own event content'] = array(
    'name' => 'delete own event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit cms_events_registration registration state'.
  $permissions['edit cms_events_registration registration state'] = array(
    'name' => 'edit cms_events_registration registration state',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'edit own event content'.
  $permissions['edit own event content'] = array(
    'name' => 'edit own event content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'update any cms_events_registration registration'.
  $permissions['update any cms_events_registration registration'] = array(
    'name' => 'update any cms_events_registration registration',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'update own cms_events_registration registration'.
  $permissions['update own cms_events_registration registration'] = array(
    'name' => 'update own cms_events_registration registration',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'view cms_events_registration registration'.
  $permissions['view cms_events_registration registration'] = array(
    'name' => 'view cms_events_registration registration',
    'roles' => array(
      'Event Manager' => 'Event Manager',
    ),
    'module' => 'registration',
  );

  // Exported permission: 'view own cms_events_registration registration'.
  $permissions['view own cms_events_registration registration'] = array(
    'name' => 'view own cms_events_registration registration',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'registration',
  );

  return $permissions;
}
