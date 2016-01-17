<?php
/**
 * @file
 * recruiter_resume.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function recruiter_resume_user_default_roles() {
  $roles = array();

  // Exported role: applicant.
  $roles['applicant'] = array(
    'name' => 'applicant',
    'weight' => '4',
    'machine_name' => 'applicant',
  );

  return $roles;
}
