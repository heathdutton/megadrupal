<?php
/**
 * @file
 * Default views provided by party_service.
 */

/**
 * Implements hook_views_default_views().
 */
function party_service_views_default_views() {
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
