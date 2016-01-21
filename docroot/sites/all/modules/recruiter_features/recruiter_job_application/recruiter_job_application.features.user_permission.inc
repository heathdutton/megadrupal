<?php
/**
 * @file
 * recruiter_job_application.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function recruiter_job_application_user_default_permissions() {
  $permissions = array();

  // Exported permission: access rules link job_application_reject.
  $permissions['access rules link job_application_reject'] = array(
    'name' => 'access rules link job_application_reject',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'recruiter',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: access rules link job_application_shortlist.
  $permissions['access rules link job_application_shortlist'] = array(
    'name' => 'access rules link job_application_shortlist',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'recruiter',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: create field_job_application_rej_msg.
  $permissions['create field_job_application_rej_msg'] = array(
    'name' => 'create field_job_application_rej_msg',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create field_job_application_rej_reason.
  $permissions['create field_job_application_rej_reason'] = array(
    'name' => 'create field_job_application_rej_reason',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create field_job_application_state.
  $permissions['create field_job_application_state'] = array(
    'name' => 'create field_job_application_state',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create job_application content.
  $permissions['create job_application content'] = array(
    'name' => 'create job_application content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: delete any job_application content.
  $permissions['delete any job_application content'] = array(
    'name' => 'delete any job_application content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own job_application content.
  $permissions['delete own job_application content'] = array(
    'name' => 'delete own job_application content',
    'roles' => array(
      0 => 'administrator',
      1 => 'applicant',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any job_application content.
  $permissions['edit any job_application content'] = array(
    'name' => 'edit any job_application content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: edit field_job_application_rej_msg.
  $permissions['edit field_job_application_rej_msg'] = array(
    'name' => 'edit field_job_application_rej_msg',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit field_job_application_rej_reason.
  $permissions['edit field_job_application_rej_reason'] = array(
    'name' => 'edit field_job_application_rej_reason',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit field_job_application_state.
  $permissions['edit field_job_application_state'] = array(
    'name' => 'edit field_job_application_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_job_application_rej_msg.
  $permissions['edit own field_job_application_rej_msg'] = array(
    'name' => 'edit own field_job_application_rej_msg',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_job_application_rej_reason.
  $permissions['edit own field_job_application_rej_reason'] = array(
    'name' => 'edit own field_job_application_rej_reason',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_job_application_state.
  $permissions['edit own field_job_application_state'] = array(
    'name' => 'edit own field_job_application_state',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own job_application content.
  $permissions['edit own job_application content'] = array(
    'name' => 'edit own job_application content',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: view field_job_application_rej_msg.
  $permissions['view field_job_application_rej_msg'] = array(
    'name' => 'view field_job_application_rej_msg',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
      2 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view field_job_application_rej_reason.
  $permissions['view field_job_application_rej_reason'] = array(
    'name' => 'view field_job_application_rej_reason',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
      2 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view field_job_application_state.
  $permissions['view field_job_application_state'] = array(
    'name' => 'view field_job_application_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
      2 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_job_application_rej_msg.
  $permissions['view own field_job_application_rej_msg'] = array(
    'name' => 'view own field_job_application_rej_msg',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_job_application_rej_reason.
  $permissions['view own field_job_application_rej_reason'] = array(
    'name' => 'view own field_job_application_rej_reason',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_job_application_state.
  $permissions['view own field_job_application_state'] = array(
    'name' => 'view own field_job_application_state',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
