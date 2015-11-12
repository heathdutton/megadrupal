<?php
/**
 * @file
 * clinks_party.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function party_dashboard_example_default_page_manager_pages() {
  $export = array();

  // Scan directory for any .panel files
  $files = file_scan_directory(dirname(__FILE__) . '/page_manager_pages', '/\.panel$/', array('key' => 'name'));
  foreach ($files as $file) {
    if ((include $file->uri) == 1) {
      $export[$page->name] = $page;
    }
  }

  return $export;
}
