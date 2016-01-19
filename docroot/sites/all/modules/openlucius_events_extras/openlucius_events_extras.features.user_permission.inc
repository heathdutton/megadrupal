<?php
/**
 * @file
 * openlucius_events.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function openlucius_events_extras_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer openlucius event configuration'.
  $permissions['administer openlucius event configuration'] = array(
    'name' => 'administer openlucius event configuration',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
    ),
    'module' => 'openlucius_events_extras',
  );

  // Exported permission: 'flag attending_event'.
  $permissions['flag attending_event'] = array(
    'name' => 'flag attending_event',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'client' => 'client',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag not_attending_event'.
  $permissions['flag not_attending_event'] = array(
    'name' => 'flag not_attending_event',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'send openlucius event reminders'.
  $permissions['send openlucius event reminders'] = array(
    'name' => 'send openlucius event reminders',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
    ),
    'module' => 'openlucius_events_extras',
  );

  // Exported permission: 'unflag attending'.
  $permissions['unflag attending'] = array(
    'name' => 'unflag attending',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
    ),
    'module' => 'openlucius_events_extras',
  );

  // Exported permission: 'unflag attending_event'.
  $permissions['unflag attending_event'] = array(
    'name' => 'unflag attending_event',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'client' => 'client',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag not attending'.
  $permissions['unflag not attending'] = array(
    'name' => 'unflag not attending',
    'roles' => array(
      'admin' => 'admin',
      'can create groups' => 'can create groups',
    ),
    'module' => 'openlucius_events_extras',
  );

  // Exported permission: 'unflag not_attending_event'.
  $permissions['unflag not_attending_event'] = array(
    'name' => 'unflag not_attending_event',
    'roles' => array(
      'authenticated user' => 'authenticated user',
      'openlucius authenticated user' => 'openlucius authenticated user',
    ),
    'module' => 'flag',
  );

  return $permissions;
}
