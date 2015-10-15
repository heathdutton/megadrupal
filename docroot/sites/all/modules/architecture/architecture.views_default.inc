<?php

/**
 * @file
 * Provide default views using the Views API.
 */

/**
 * Implements hook_views_default_views().
 */
function architecture_views_default_views() {
  $views = array();

  // Find all the files in the directory with the correct extension.
  $include_path = drupal_get_path('module', 'architecture') . '/views';
  $files = file_scan_directory($include_path, "/\.view\.inc$/");

  // Load each file and dig out its view.
  foreach ($files as $filepath => $file) {
    require $filepath;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }

  return $views;
}
