<?php

/**
 * Implementation of hook_features_export()
 */
function custom_menus_features_export($data, &$export, $module_name) {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['features_utils'] = 'features_utils';
  foreach ($data as $component) {
    $export['features']['custom_menus'][$component] = $component;
  }
  return array();
}

/**
 * Implementation of hook_features_export_options()
 */
function custom_menus_features_export_options() {
  return array('all_custom_menus' => t('All Custom Menu Items'));
}

/**
 * Implementation of hook_features_export_render()
 */
function custom_menus_features_export_render($module, $data, $export = NULL) {
  $code = array();
  $code[] = '  $custom_menus = array();';

  foreach ($data as $name) {
    $code[] = "  \$custom_menus['{$name}'] = " . features_var_export(features_utils_custom_menus_load()) . ';';
  }

  $code[] = '  return $custom_menus;';
  $code = implode("\n", $code);
  return array('custom_menus_defaults' => $code);
}

/**
 * Implementation of hook_features_revert()
 */
function custom_menus_features_revert($module) {
  return custom_menus_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild()
 */
function custom_menus_features_rebuild($module) {
  $data = features_get_default('custom_menus', $module);

  // if we had a problem loading the data, bail
  if (!$data) return;

  // wipe out the custom links
  db_delete('menu_links')->condition('customized', 1)->execute();

  // restore each og global perm from export
  $data = array_shift($data);
  foreach ($data as $menu_item) {
    db_insert('menu_links')->fields((array) $menu_item)->execute();
  }

  menu_cache_clear_all();

  return TRUE;
}

/**
 * load the custom menu links from the database
 */
function features_utils_custom_menus_load() {
  return db_select('menu_links', 'm')->fields('m')->condition('customized', 1)->execute()->fetchAll();
}