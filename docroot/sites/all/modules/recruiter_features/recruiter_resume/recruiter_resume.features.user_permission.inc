<?php
/**
 * @file
 * recruiter_resume.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function recruiter_resume_user_default_permissions() {
  $permissions = array();

  // Exported permission: access rules link resume_init_visible.
  $permissions['access rules link resume_init_visible'] = array(
    'name' => 'access rules link resume_init_visible',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: access rules link resume_set_hidden.
  $permissions['access rules link resume_set_hidden'] = array(
    'name' => 'access rules link resume_set_hidden',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
      2 => 'content admin',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: access rules link resume_set_visible.
  $permissions['access rules link resume_set_visible'] = array(
    'name' => 'access rules link resume_set_visible',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
      2 => 'content admin',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: edit any resume profile.
  $permissions['edit any resume profile'] = array(
    'name' => 'edit any resume profile',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'profile2',
  );

  // Exported permission: edit field_resume_state.
  $permissions['edit field_resume_state'] = array(
    'name' => 'edit field_resume_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_resume_state.
  $permissions['edit own field_resume_state'] = array(
    'name' => 'edit own field_resume_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own resume profile.
  $permissions['edit own resume profile'] = array(
    'name' => 'edit own resume profile',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
      2 => 'content admin',
    ),
    'module' => 'profile2',
  );

  // Exported permission: flag resume_bookmarks.
  $permissions['flag resume_bookmarks'] = array(
    'name' => 'flag resume_bookmarks',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'recruiter',
    ),
    'module' => 'flag',
  );

  // Exported permission: unflag resume_bookmarks.
  $permissions['unflag resume_bookmarks'] = array(
    'name' => 'unflag resume_bookmarks',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'recruiter',
    ),
    'module' => 'flag',
  );

  // Exported permission: view any resume profile.
  $permissions['view any resume profile'] = array(
    'name' => 'view any resume profile',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'recruiter',
    ),
    'module' => 'profile2',
  );

  // Exported permission: view field_resume_state.
  $permissions['view field_resume_state'] = array(
    'name' => 'view field_resume_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_resume_state.
  $permissions['view own field_resume_state'] = array(
    'name' => 'view own field_resume_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own resume profile.
  $permissions['view own resume profile'] = array(
    'name' => 'view own resume profile',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
      2 => 'content admin',
    ),
    'module' => 'profile2',
  );

  return $permissions;
}
