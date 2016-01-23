<?php
/**
 * @file
 * cms_event_registrations.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function cms_event_registrations_user_default_roles() {
  $roles = array();

  // Exported role: Event Registrations Manager.
  $roles['Event Registrations Manager'] = array(
    'name' => 'Event Registrations Manager',
    'weight' => 4,
  );

  return $roles;
}
