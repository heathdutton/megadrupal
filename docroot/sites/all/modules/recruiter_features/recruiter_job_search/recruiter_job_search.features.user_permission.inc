<?php
/**
 * @file
 * recruiter_job_search.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function recruiter_job_search_user_default_permissions() {
  $permissions = array();

  // Exported permission: flag job_bookmarks.
  $permissions['flag job_bookmarks'] = array(
    'name' => 'flag job_bookmarks',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
      2 => 'content admin',
    ),
    'module' => 'flag',
  );

  // Exported permission: unflag job_bookmarks.
  $permissions['unflag job_bookmarks'] = array(
    'name' => 'unflag job_bookmarks',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
      2 => 'content admin',
    ),
    'module' => 'flag',
  );

  return $permissions;
}
