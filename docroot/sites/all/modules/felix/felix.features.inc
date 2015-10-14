<?php
/**
 * @file
 * Functions to allow exporting Felix configuration in Features.
 */

/**
 * Implements hook_features_export().
 */
function felix_region_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['felix_region'][$component] = $component;
  }
  // Add dependent block sets.
  $sets = db_select('felix_region', 'r')
    ->fields('r', array('block_set'))
    ->condition('r.name', array_keys($data), 'IN')
    ->distinct()
    ->execute()
    ->fetchAll();
  foreach ($sets as $set) {
    $export['features']['felix_block_set'][$set->block_set] = $set->block_set;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 */
function felix_region_features_export_options() {
  $options = array();
  $regions = db_select('felix_region', 'r')
    ->fields('r', array('name', 'title'))
    ->orderBy('r.title', 'asc')
    ->execute()
    ->fetchAll();
  foreach ($regions as $region) {
    $options[$region->name] = $region->title;
  }
  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function felix_region_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'felix', 'felix.export');
  $code = '';
  $code = "  \$regions = array();\n";
  foreach ($data as $name) {
    $object = felix_export_region($name);
    $code .= "  \$regions['{$name}'] = " . features_var_export($object, '  ') . ";\n";
  }
  $code .= "  return \$regions;";
  return array('felix_default_regions' => $code);
}

/**
 * Implements hook_features_export_revert().
 */
function felix_region_features_revert($module_name) {
  $regions = module_invoke($module_name, 'felix_default_regions');
  if (!empty($regions)) {
    foreach ($regions as $region) {
      felix_import_region($region);
    }
  }
}

/**
 * Implements hook_features_export_rebuild().
 */
function felix_region_features_rebuild($module_name) {
  $regions = module_invoke($module_name, 'felix_default_regions');
  if (!empty($regions)) {
    foreach ($regions as $region) {
      felix_import_region($region);
    }
  }
}

/**
 * Implements hook_features_export().
 */
function felix_block_set_features_export($data, &$export, $module_name) {
  foreach ($data as $component) {
    $export['features']['felix_block_set'][$component] = $component;
  }
  return array();
}

/**
 * Implements hook_features_export_options().
 */
function felix_block_set_features_export_options() {
  $options = array();
  $sets = db_select('felix_block_set', 'bs')
    ->fields('bs', array('name', 'title'))
    ->orderBy('bs.title', 'asc')
    ->execute()
    ->fetchAll();
  foreach ($sets as $set) {
    $set = (object) $set;
    $options[$set->name] = $set->title;
  }
  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function felix_block_set_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'felix', 'felix.export');
  $code = '';
  $code = "  \$sets = array();\n";
  foreach ($data as $name) {
    $object = felix_export_block_set($name);
    $code .= "  \$sets['{$name}'] = " . features_var_export($object, '  ') . ";\n";
  }
  $code .= "  return \$sets;";
  return array('felix_default_block_sets' => $code);
}

/**
 * Implements hook_features_export_revert().
 */
function felix_block_set_features_revert($module_name) {
  $sets = module_invoke($module_name, 'felix_default_block_sets');
  if (!empty($sets)) {
    foreach ($sets as $set) {
      felix_import_block_set($set);
    }
  }
}

/**
 * Implements hook_features_export_rebuild().
 */
function felix_block_set_features_rebuild($module_name) {
  $sets = module_invoke($module_name, 'felix_default_block_sets');
  if (!empty($sets)) {
    foreach ($sets as $set) {
      felix_import_block_set($set);
    }
  }
}
