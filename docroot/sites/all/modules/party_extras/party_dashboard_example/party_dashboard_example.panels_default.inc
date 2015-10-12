<?php
/**
 * @file
 * party_dashboard_example.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function party_dashboard_example_default_panels_mini() {
  $export = array();

  // Scan directory for any .panel files
  $files = file_scan_directory(dirname(__FILE__) . '/panels_mini', '/\.panel$/', array('key' => 'name'));
  foreach ($files as $file) {
    if ((include $file->uri) == 1) {
      $export[$mini->name] = $mini;
    }
  }

  return $export;
}
