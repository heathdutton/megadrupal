<?php
/**
 * @file
 * recruiter_job.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function recruiter_job_user_default_roles() {
  $roles = array();

  // Exported role: job admin.
  $roles['job admin'] = array(
    'name' => 'job admin',
    'weight' => '5',
    'machine_name' => 'job_admin',
  );

  return $roles;
}
