<?php

/**
 * @file
 * Provide some default rules that work with the barcode scanner module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_barcode_scanner_default_rules_configuration() {

  $rules_path = drupal_get_path('module', 'commerce_barcode_scanner') . '/rules';
  $files = file_scan_directory($rules_path, '#(.*)\.rule\.inc#');

  $rules = array();
  foreach ($files as $filepath => $file) {
    $rule_name = 'commerce_barcode_scanner_' . str_replace('.rule.inc', '', $file->filename);
    $loaded_rule = require $filepath;
    $rules[$rule_name] = rules_import($loaded_rule);
  }

  return $rules;
}
