<?php
/**
 * @file
 * openpublic_workflow.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function openpublic_workflow_user_default_roles() {
  $roles = array();

  // Exported role: publisher.
  $roles['publisher'] = array(
    'name' => 'publisher',
    'weight' => 4,
  );

  return $roles;
}
