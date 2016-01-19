<?php

/**
 * Implementation of hook_features_export()
 */
function site_permissions_features_export($data, &$export, $module_name) {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['features_utils'] = 'features_utils';
  foreach ($data as $component) {
    $export['features']['site_permissions'][$component] = $component;
  }
  return array();
}

/**
 * Implementation of hook_features_export_options()
 */
function site_permissions_features_export_options() {
  return array('all_site_permissions' => t('All Site Permissions'));
}

/**
 * Implementation of hook_features_export_render()
 */
function site_permissions_features_export_render($module, $data, $export = NULL) {
  $code = array();
  $code[] = '  $site_permissions = array();';

  foreach ($data as $name) {
    $code[] = "  \$site_permissions['{$name}'] = " . features_var_export(site_permissions_load()) . ';';
  }

  $code[] = '  return $site_permissions;';
  $code = implode("\n", $code);
  return array('site_permissions_defaults' => $code);
}

/**
 * Implementation of hook_features_revert()
 */
function site_permissions_features_revert($module) {
  return site_permissions_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild()
 */
function site_permissions_features_rebuild($module) {
  $data = features_get_default('site_permissions', $module);
  
  // if we had a problem loading the data, bail
  if (!$data) return;

  // wipe out the table that holds the role/permission relations
  db_delete('role_permission')->execute();

  // restore each og global perm from export
  $data = array_shift($data);
  foreach ($data as $rp) {
    db_insert('role_permission')->fields((array) $rp)->execute();
  }

  return TRUE;
}

/**
 * Function to load the site's global permissions
 */
function site_permissions_load() {
  $data = &drupal_static(__FUNCTION__, NULL);

  // only run query first time through
  if (!$data) {
    $data =  db_select('role_permission', 'rp')->fields('rp')->orderBy('rid')
       ->orderBy('module')->orderBy('permission')->execute()->fetchAll();
  }

  return $data;
}