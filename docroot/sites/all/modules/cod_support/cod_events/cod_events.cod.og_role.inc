<?php
/**
 * @file
 * cod_session.features.og_features_role.inc
 */

/**
 * Implements hook_og_initial_roles().
 */
function cod_events_og_initial_roles() {
  $roles = array();

  // Exported OG Role: 'node:event:content editor'.
  $roles['node:event:content editor'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'event',
    'name' => 'content editor',
  );

  return $roles;
}
