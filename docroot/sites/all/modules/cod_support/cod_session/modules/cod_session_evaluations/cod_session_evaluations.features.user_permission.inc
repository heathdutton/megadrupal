<?php
/**
 * @file
 * cod_session_evaluations.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function cod_session_evaluations_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create field_eval_field_1'.
  $permissions['create field_eval_field_1'] = array(
    'name' => 'create field_eval_field_1',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_eval_field_2'.
  $permissions['create field_eval_field_2'] = array(
    'name' => 'create field_eval_field_2',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_eval_field_3'.
  $permissions['create field_eval_field_3'] = array(
    'name' => 'create field_eval_field_3',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_eval_field_4'.
  $permissions['create field_eval_field_4'] = array(
    'name' => 'create field_eval_field_4',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_eval_field_5'.
  $permissions['create field_eval_field_5'] = array(
    'name' => 'create field_eval_field_5',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_eval_text_field'.
  $permissions['create field_eval_text_field'] = array(
    'name' => 'create field_eval_text_field',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create session_evaluation content'.
  $permissions['create session_evaluation content'] = array(
    'name' => 'create session_evaluation content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit field_eval_field_1'.
  $permissions['edit field_eval_field_1'] = array(
    'name' => 'edit field_eval_field_1',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_eval_field_2'.
  $permissions['edit field_eval_field_2'] = array(
    'name' => 'edit field_eval_field_2',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_eval_field_3'.
  $permissions['edit field_eval_field_3'] = array(
    'name' => 'edit field_eval_field_3',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_eval_field_4'.
  $permissions['edit field_eval_field_4'] = array(
    'name' => 'edit field_eval_field_4',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_eval_field_5'.
  $permissions['edit field_eval_field_5'] = array(
    'name' => 'edit field_eval_field_5',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_eval_text_field'.
  $permissions['edit field_eval_text_field'] = array(
    'name' => 'edit field_eval_text_field',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_eval_field_1'.
  $permissions['edit own field_eval_field_1'] = array(
    'name' => 'edit own field_eval_field_1',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_eval_field_2'.
  $permissions['edit own field_eval_field_2'] = array(
    'name' => 'edit own field_eval_field_2',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_eval_field_3'.
  $permissions['edit own field_eval_field_3'] = array(
    'name' => 'edit own field_eval_field_3',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_eval_field_4'.
  $permissions['edit own field_eval_field_4'] = array(
    'name' => 'edit own field_eval_field_4',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_eval_field_5'.
  $permissions['edit own field_eval_field_5'] = array(
    'name' => 'edit own field_eval_field_5',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_eval_text_field'.
  $permissions['edit own field_eval_text_field'] = array(
    'name' => 'edit own field_eval_text_field',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_eval_field_1'.
  $permissions['view field_eval_field_1'] = array(
    'name' => 'view field_eval_field_1',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_eval_field_2'.
  $permissions['view field_eval_field_2'] = array(
    'name' => 'view field_eval_field_2',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_eval_field_3'.
  $permissions['view field_eval_field_3'] = array(
    'name' => 'view field_eval_field_3',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_eval_field_4'.
  $permissions['view field_eval_field_4'] = array(
    'name' => 'view field_eval_field_4',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_eval_field_5'.
  $permissions['view field_eval_field_5'] = array(
    'name' => 'view field_eval_field_5',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_eval_text_field'.
  $permissions['view field_eval_text_field'] = array(
    'name' => 'view field_eval_text_field',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_eval_field_1'.
  $permissions['view own field_eval_field_1'] = array(
    'name' => 'view own field_eval_field_1',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_eval_field_2'.
  $permissions['view own field_eval_field_2'] = array(
    'name' => 'view own field_eval_field_2',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_eval_field_3'.
  $permissions['view own field_eval_field_3'] = array(
    'name' => 'view own field_eval_field_3',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_eval_field_4'.
  $permissions['view own field_eval_field_4'] = array(
    'name' => 'view own field_eval_field_4',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_eval_field_5'.
  $permissions['view own field_eval_field_5'] = array(
    'name' => 'view own field_eval_field_5',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_eval_text_field'.
  $permissions['view own field_eval_text_field'] = array(
    'name' => 'view own field_eval_text_field',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
