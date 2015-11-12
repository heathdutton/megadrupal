<?php

/**
 * Implements hook_default_rules_configuration().
 */
function rules_onceperpay_default_rules_configuration() {
  $config = array();
  $rules_path = drupal_get_path('module', 'rules_onceperday') . '/default_rules';
  $files = file_scan_directory($rules_path, '/\.inc$/');
  foreach ($files as $filepath => $file) {
    require $filepath;
    if (isset($rule)) {
      $config['mymodule_' . $file->name] = rules_import($rule);
    }
  }
  return $config;
}