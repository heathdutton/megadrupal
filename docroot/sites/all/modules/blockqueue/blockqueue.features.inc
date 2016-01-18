<?php

/**
 * @file
 * Features file.
 */

/**
 * Extends the CTools implementation of hook_features_export().
 *
 * @param array  $data
 *   This is the machine name for the component in question.
 * @param array  $export
 *   Array of all components to be exported.
 * @param string $module_name
 *   The name of the feature module to be generated.
 *
 * @return array
 *   The pipe array of further processors that should be called.
 */
function blockqueue_features_export($data, &$export, $module_name) {
  // Default dependencies.
  $export['dependencies']['block'] = 'block';

  // Load blockqueues.
  foreach ($data as $component) {
    $blockqueue = blockqueue_load($component);

    // Add blocks' module dependencies.
    foreach ($blockqueue->blocks as $token) {
      list($module, $delta) = explode('|', $token);

      $export['dependencies'][$module] = $module;
    }

    // Improve dependencies if features extra block is enabled.
    if (module_exists('fe_block')) {
      $export['dependencies']['features_extra'] = 'features_extra';
      $export['dependencies']['fe_block']       = 'fe_block';

      foreach ($blockqueue->blocks as $token) {
        $component = str_replace('|', '-', $token);

        $export['features']['fe_block_settings'][$component] = $component;
      }
    }
  }

  // Fallback on default ctools features export.
  return ctools_component_features_export('blockqueue', $data, $export, $module_name);
}
