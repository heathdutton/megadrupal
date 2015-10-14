<?php
/**
 * @file
 * openacademy_courses.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function openacademy_courses_defaultconfig_features() {
  return array(
    'openacademy_courses' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function openacademy_courses_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'create openacademy_course content'.
  $permissions['create openacademy_course content'] = array(
    'name' => 'create openacademy_course content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any openacademy_course content'.
  $permissions['delete any openacademy_course content'] = array(
    'name' => 'delete any openacademy_course content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own openacademy_course content'.
  $permissions['delete own openacademy_course content'] = array(
    'name' => 'delete own openacademy_course content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any openacademy_course content'.
  $permissions['edit any openacademy_course content'] = array(
    'name' => 'edit any openacademy_course content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own openacademy_course content'.
  $permissions['edit own openacademy_course content'] = array(
    'name' => 'edit own openacademy_course content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
    ),
    'module' => 'node',
  );

  return $permissions;
}
