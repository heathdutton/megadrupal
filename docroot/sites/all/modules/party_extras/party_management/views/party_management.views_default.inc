<?php
/**
 * @file
 * party_management.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function party_management_views_default_views() {
  $views = array();

  // Scan this directory for any .view files
  $files = file_scan_directory(dirname(__FILE__), '/\.view$/', array('key' => 'name'));
  foreach ($files as $file) {
    if ((include $file->uri) == 1) {
      $views[$view->name] = $view;
    }
  }

  return $views;
}
