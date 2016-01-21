<?php
/**
 * @file
 * recruiter_job.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function recruiter_job_user_default_permissions() {
  $permissions = array();

  // Exported permission: access rules link job_publish.
  $permissions['access rules link job_publish'] = array(
    'name' => 'access rules link job_publish',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: access rules link job_unpublish.
  $permissions['access rules link job_unpublish'] = array(
    'name' => 'access rules link job_unpublish',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'rules_link',
  );

  // Exported permission: create field_job_workflow_state.
  $permissions['create field_job_workflow_state'] = array(
    'name' => 'create field_job_workflow_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create job_per_file content.
  $permissions['create job_per_file content'] = array(
    'name' => 'create job_per_file content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: create job_per_link content.
  $permissions['create job_per_link content'] = array(
    'name' => 'create job_per_link content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: create job_per_template content.
  $permissions['create job_per_template content'] = array(
    'name' => 'create job_per_template content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any job_per_file content.
  $permissions['delete any job_per_file content'] = array(
    'name' => 'delete any job_per_file content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any job_per_link content.
  $permissions['delete any job_per_link content'] = array(
    'name' => 'delete any job_per_link content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any job_per_template content.
  $permissions['delete any job_per_template content'] = array(
    'name' => 'delete any job_per_template content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own job_per_file content.
  $permissions['delete own job_per_file content'] = array(
    'name' => 'delete own job_per_file content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own job_per_link content.
  $permissions['delete own job_per_link content'] = array(
    'name' => 'delete own job_per_link content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own job_per_template content.
  $permissions['delete own job_per_template content'] = array(
    'name' => 'delete own job_per_template content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any job_per_file content.
  $permissions['edit any job_per_file content'] = array(
    'name' => 'edit any job_per_file content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any job_per_link content.
  $permissions['edit any job_per_link content'] = array(
    'name' => 'edit any job_per_link content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any job_per_template content.
  $permissions['edit any job_per_template content'] = array(
    'name' => 'edit any job_per_template content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit field_job_workflow_state.
  $permissions['edit field_job_workflow_state'] = array(
    'name' => 'edit field_job_workflow_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own job_per_file content.
  $permissions['edit own job_per_file content'] = array(
    'name' => 'edit own job_per_file content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own job_per_link content.
  $permissions['edit own job_per_link content'] = array(
    'name' => 'edit own job_per_link content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own job_per_template content.
  $permissions['edit own job_per_template content'] = array(
    'name' => 'edit own job_per_template content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  // Exported permission: use Rules component rules_link_set_job_publish.
  $permissions['use Rules component rules_link_set_job_publish'] = array(
    'name' => 'use Rules component rules_link_set_job_publish',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'rules',
  );

  // Exported permission: use Rules component rules_link_set_job_unpublish.
  $permissions['use Rules component rules_link_set_job_unpublish'] = array(
    'name' => 'use Rules component rules_link_set_job_unpublish',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'rules',
  );

  // Exported permission: view field_job_workflow_state.
  $permissions['view field_job_workflow_state'] = array(
    'name' => 'view field_job_workflow_state',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own unpublished content.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      0 => 'administrator',
      1 => 'content admin',
      2 => 'job admin',
      3 => 'recruiter',
    ),
    'module' => 'node',
  );

  return $permissions;
}
