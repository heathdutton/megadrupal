<?php
/**
 * @file
 * recruiter_common.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function recruiter_common_user_default_roles() {
  $roles = array();

  // Exported role: administrator.
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => '2',
    'machine_name' => 'administrator',
  );

  // Exported role: content admin.
  $roles['content admin'] = array(
    'name' => 'content admin',
    'weight' => '6',
    'machine_name' => 'content_admin',
  );

  // Exported role: recruiter.
  $roles['recruiter'] = array(
    'name' => 'recruiter',
    'weight' => '3',
    'machine_name' => 'recruiter',
  );

  return $roles;
}
