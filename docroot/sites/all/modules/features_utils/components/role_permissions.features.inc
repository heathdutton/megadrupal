<?php

/**
 * @file
 * Role Permissions Features component hooks.
 */

/**
 * Implements hook_features_export().
 */
function role_permissions_features_export($data, &$export, $module_name) {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['features_utils'] = 'features_utils';
  foreach ($data as $component) {
    $export['features']['role_permissions'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 *
 * Provides a list of role_permissions components.
 */
function role_permissions_features_export_options() {
  return array_combine(user_roles(), user_roles());
}

/**
 * Implements hook_features_export_render().
 */
function role_permissions_features_export_render($module, $data, $export = NULL) {
  $code = array();

  $code[] = '  $role_permissions = array();';

  foreach ($data as $role) {
    //$code[] = "  \$role_permissions['{$role->name}'] = " . features_var_export(site_permissions_load()) . ';';
    $code[] = "  \$role_permissions['{$role}'] = " . features_var_export(role_permissions_load($role)) . ';';
  }

  $code[] = '  return $role_permissions;';
  $code = implode("\n", $code);
  return array('role_permissions_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function role_permissions_features_revert($module) {
  return role_permissions_features_rebuild($module);
}

/**
 * Implements hook_features_rebuild().
 */
function role_permissions_features_rebuild($module) {

  $data = features_get_default('role_permissions', $module);

  // If we had a problem loading the data, bail.
  if (!$data) {
    return;
  }

  // @todo: Conduct this business in a transaction.

  foreach ($data as $role_name => $perms) {
    if ($role = user_role_load_by_name($role_name)) {
      // Delete existing permission assignments.
      db_query('DELETE FROM {role_permission} WHERE rid = :rid', array(':rid' => $role->rid));
      // Insert data from module.
      $query = 'INSERT INTO {role_permission} (rid, permission, module) VALUES (:rid, :permission, :module)';
      $args = array(':rid' => $role->rid);
      foreach ($perms as $perm) {
        $args[':permission'] = $perm['permission'];
        $args[':module'] = $perm['module'];
        db_query($query, $args);
      }
    }
  }

  return TRUE;
}

/**
 * Loads permissions assigned to the role.
 */
function role_permissions_load($role_name) {
  $data = &drupal_static(__FUNCTION__.$role_name);

  // Only load from db first time through the page request.
  if (!$data) {
    if ($role = user_role_load_by_name($role_name)) {
      $query = 'SELECT permission, module FROM {role_permission} WHERE rid = :rid ORDER BY module, permission';
      $data =  db_query($query, array(':rid' => $role->rid))->fetchAll();
    } else {
      $data = array();
    }
  }

  return $data;
}
