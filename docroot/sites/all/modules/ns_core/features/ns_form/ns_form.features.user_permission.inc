<?php
/**
 * @file
 * ns_form.features.user_permission.inc
 */

/**
 * Implementation of hook_user_default_permissions().
 */
function ns_form_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: access all webform results
  $permissions['access all webform results'] = array(
    'name' => 'access all webform results',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
    ),
    'module' => 'webform',
  );

  // Exported permission: access own webform results
  $permissions['access own webform results'] = array(
    'name' => 'access own webform results',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
    ),
    'module' => 'webform',
  );

  // Exported permission: access own webform submissions
  $permissions['access own webform submissions'] = array(
    'name' => 'access own webform submissions',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
    ),
    'module' => 'webform',
  );

  // Exported permission: create ns_form content
  $permissions['create ns_form content'] = array(
    'name' => 'create ns_form content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer',
    ),
    'module' => 'node',
  );

  // Exported permission: delete all webform submissions
  $permissions['delete all webform submissions'] = array(
    'name' => 'delete all webform submissions',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
    ),
    'module' => 'webform',
  );

  // Exported permission: delete any ns_form content
  $permissions['delete any ns_form content'] = array(
    'name' => 'delete any ns_form content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own ns_form content
  $permissions['delete own ns_form content'] = array(
    'name' => 'delete own ns_form content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own webform submissions
  $permissions['delete own webform submissions'] = array(
    'name' => 'delete own webform submissions',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
      5 => 'writer',
    ),
    'module' => 'webform',
  );

  // Exported permission: edit all webform submissions
  $permissions['edit all webform submissions'] = array(
    'name' => 'edit all webform submissions',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
    ),
    'module' => 'webform',
  );

  // Exported permission: edit any ns_form content
  $permissions['edit any ns_form content'] = array(
    'name' => 'edit any ns_form content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own ns_form content
  $permissions['edit own ns_form content'] = array(
    'name' => 'edit own ns_form content',
    'roles' => array(
      0 => 'administrator',
      1 => 'chief editor',
      2 => 'editor',
      3 => 'super user',
      4 => 'writer',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own webform submissions
  $permissions['edit own webform submissions'] = array(
    'name' => 'edit own webform submissions',
    'roles' => array(
      0 => 'administrator',
      1 => 'blogger',
      2 => 'chief editor',
      3 => 'editor',
      4 => 'super user',
    ),
    'module' => 'webform',
  );

  return $permissions;
}
