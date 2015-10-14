<?php
/**
 * @file
 * cod_registration.features.user_permission.inc
 */

/**
 * Implements hook_user_initial_permissions().
 */
function cod_registration_user_initial_permissions() {
  $permissions = array();

  // Exported permission: 'administer ticket'.
  $permissions['administer ticket'] = array(
    'name' => 'administer ticket',
    'roles' => array(
      'administrator' => 'administrator',
      'attendee manager' => 'attendee manager',
    ),
    'module' => 'ticket',
  );

  // Exported permission: 'administer ticket states'.
  $permissions['administer ticket states'] = array(
    'name' => 'administer ticket states',
    'roles' => array(
      'administrator' => 'administrator',
      'attendee manager' => 'attendee manager',
    ),
    'module' => 'ticket_state',
  );

  // Exported permission: 'edit all ticket registrations'.
  $permissions['edit all ticket registrations'] = array(
    'name' => 'edit all ticket registrations',
    'roles' => array(
      'administrator' => 'administrator',
      'attendee manager' => 'attendee manager',
    ),
    'module' => 'ticket',
  );

  // Exported permission: 'view all ticket registrations'.
  $permissions['view all ticket registrations'] = array(
    'name' => 'view all ticket registrations',
    'roles' => array(
      'administrator' => 'administrator',
      'attendee manager' => 'attendee manager',
    ),
    'module' => 'ticket',
  );

  return $permissions;
}
