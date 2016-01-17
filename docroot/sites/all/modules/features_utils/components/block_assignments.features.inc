<?php

/**
 * Implementation of hook_features_export()
 */
function block_assignments_features_export($data, &$export, $module_name) {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['features_utils'] = 'features_utils';
  $export['dependencies']['block'] = 'block';
  foreach ($data as $component) {
    $export['features']['block_assignments'][$component] = $component;
  }
  return array();
}

/**
 * Implementation of hook_features_export_options()
 */
function block_assignments_features_export_options() {
  return array('all_block_assignments' => t('All block assignments.'));
}

/**
 * Implementation of hook_features_export_render()
 */
function block_assignments_features_export_render($module, $data, $export = NULL) {
  $code = array();
  $code[] = '  $block_assignments = array();';

  foreach ($data as $name) {
    $code[] = "  \$block_assignments['{$name}'] = " . features_var_export(features_utils_block_assignments_load()) . ';';
  }

  $code[] = '  return $block_assignments;';
  $code = implode("\n", $code);
  return array('block_assignments_defaults' => $code);
}

/**
 * Implementation of hook_features_revert()
 */
function block_assignments_features_revert($module) {
  return block_assignments_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild()
 */
function block_assignments_features_rebuild($module) {
  $data = features_get_default('block_assignments', $module);

  // if we had a problem loading the data, bail
  if (!$data) return;

  // wipe out existing assignments
  db_delete('block')->condition('module', 'block', '!=')->execute();

  // insert feature-ized assignments
  $data = reset($data);
  foreach ($data as $assignment) {
    db_insert('block')->fields($assignment)->execute();
  }

  return TRUE;
}

/**
 * loads block assignments for all blocks except custom blocks provided by the core block module
 */
function features_utils_block_assignments_load() {
  $blocks = &drupal_static(__FUNCTION__, NULL);

  // only query once
  if (!$blocks) {
    $blocks = db_select('block', 'b')->fields('b')->condition('module', 'block', '!=')->orderBy('module')
        ->orderBy('delta')->execute()->fetchAll();
  }

  return $blocks;
}