<?php

/**
 * @file
 * Features integration for Mosaik module.
 */

/**
 * Implements hook_features_export_options().
 */
function mosaik_features_export_options() {
  $options = array();

  foreach (mosaik_get_all_from_db() as $mosaik) {
    $options[$mosaik['name']] = $mosaik['title'];
  }

  return $options;
}

/**
 * Implements hook_features_export().
 */
function mosaik_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['features'] = 'features';
  $export['dependencies']['mosaik'] = 'mosaik';
  $export['dependencies']['pieces'] = 'pieces';

  foreach ($data as $component) {
    $export['features']['mosaik'][$component] = $component;
  }

  return $export;
}

/**
 * Implements hook_features_export_render().
 */
function mosaik_features_export_render($module, $data) {
  $code = array();
  $code[] = '$mosaiks = array();';

  foreach ($data as $mosaik_name) {
    $mosaik = mosaik_load($mosaik_name);
    $code[] = "\n\$mosaiks['{$mosaik['name']}'] = " . features_var_export($mosaik) . ";";
  }

  $code[] = "return \$mosaiks;";
  $code = implode("\n", $code);

  return array('mosaik_defaults' => $code);
}

/**
 * Implements hook_features_revert().
 */
function mosaik_features_revert($module) {
  mosaik_features_rebuild($module, TRUE);
}

/**
 * Implements hook_features_rebuild().
 */
function mosaik_features_rebuild($module, $revert = FALSE) {
  $saved_mosaiks = module_invoke($module, 'mosaik_defaults');
  if (!empty($saved_mosaiks)) {
    foreach ($saved_mosaiks as $mosaik) {
      mosaik_save($mosaik);

      if ($revert) {
        pieces_clear_associations($mosaik['name']);
      }

      foreach ($mosaik['pieces'] as $piece) {
        if (empty($piece['module']) && empty($piece['delta'])) {
          pieces_save($piece);
        }
      }

      pieces_save_associations($mosaik, $mosaik['pieces']);
    }
  }
}
