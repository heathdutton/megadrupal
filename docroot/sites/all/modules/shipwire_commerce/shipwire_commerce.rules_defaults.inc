<?php

/**
 * @file
 * Default rules configurations for Shipwire Commerce.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function shipwire_commerce_default_rules_configuration() {
  $configs = array();
  // Load *.rule files from default_rules directory.
  $rules_path = drupal_get_path('module', 'shipwire_commerce') . '/default_rules';
  $files = file_scan_directory($rules_path, '/\.rule$/');
  foreach ($files as $filepath => $file) {
    require $filepath;
    if (isset($rule_export)) {
      $rule = rules_import($rule_export);
      $configs[$rule->name] = $rule;
    }
  }

  return $configs;
}
