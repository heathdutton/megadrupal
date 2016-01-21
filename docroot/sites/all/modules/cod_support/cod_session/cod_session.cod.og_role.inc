<?php
/**
 * @file
 * cod_session.features.og_features_role.inc
 */

/**
 * Implements hook_og_initial_roles().
 */
function cod_session_og_initial_roles() {
  $roles = array();

  // Exported OG Role: 'node:event:session organizer'.
  $roles['node:event:session organizer'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'event',
    'name' => 'session organizer',
  );

  return $roles;
}
