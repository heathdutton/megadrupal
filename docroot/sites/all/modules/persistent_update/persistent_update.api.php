<?php

/**
 * @file
 * API documentation for Persistent Update API.
 */

/**
 * Allows modules to execute persistent update hooks; think hook_update_N() that
 * always runs.
 */
function hook_persistent_update() {
  $module = basename(__FILE__, '.install');
  $info   = system_get_info('module', $module);

  // Enable all dependencies.
  module_enable($info['dependencies']);

  // Forcefully clear Features caches.
  module_load_include('inc', 'features', 'features.export');
  foreach (array_keys($info['features']) as $component) {
    if ($component == 'features_api') {
      continue;
    }

    features_get_components($component, NULL, TRUE);
    features_include_defaults($component, TRUE);
    features_get_default($component, $module, TRUE, TRUE);
  }

  // Build components list for feature revert.
  $revert[$module] = array_keys($info['features']);

  // Flush all standard Drupal caches.
  drupal_flush_all_caches();

  // Revert all feature component.
  features_revert($revert);
}
