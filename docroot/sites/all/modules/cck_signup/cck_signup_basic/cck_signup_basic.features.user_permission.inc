<?php
/**
 * @file
 * cck_signup_basic.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function cck_signup_basic_user_default_permissions() {
  $permissions = array();

  // Exported permission: create event content
  $permissions['create event content'] = array(
    'name' => 'create event content',
    'roles' => array(),
  );

  // Exported permission: create signup content
  $permissions['create signup content'] = array(
    'name' => 'create signup content',
    'roles' => array(),
  );

  // Exported permission: delete any event content
  $permissions['delete any event content'] = array(
    'name' => 'delete any event content',
    'roles' => array(),
  );

  // Exported permission: delete any signup content
  $permissions['delete any signup content'] = array(
    'name' => 'delete any signup content',
    'roles' => array(),
  );

  // Exported permission: delete own event content
  $permissions['delete own event content'] = array(
    'name' => 'delete own event content',
    'roles' => array(),
  );

  // Exported permission: delete own signup content
  $permissions['delete own signup content'] = array(
    'name' => 'delete own signup content',
    'roles' => array(),
  );

  // Exported permission: edit any event content
  $permissions['edit any event content'] = array(
    'name' => 'edit any event content',
    'roles' => array(),
  );

  // Exported permission: edit any signup content
  $permissions['edit any signup content'] = array(
    'name' => 'edit any signup content',
    'roles' => array(),
  );

  // Exported permission: edit own event content
  $permissions['edit own event content'] = array(
    'name' => 'edit own event content',
    'roles' => array(),
  );

  // Exported permission: edit own signup content
  $permissions['edit own signup content'] = array(
    'name' => 'edit own signup content',
    'roles' => array(),
  );

  return $permissions;
}
