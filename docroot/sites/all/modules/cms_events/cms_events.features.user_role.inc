<?php
/**
 * @file
 * cms_events.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function cms_events_user_default_roles() {
  $roles = array();

  // Exported role: Event Manager.
  $roles['Event Manager'] = array(
    'name' => 'Event Manager',
    'weight' => 3,
  );

  return $roles;
}
