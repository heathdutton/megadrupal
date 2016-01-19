<?php

/**
 * @file
 * Default views implementations
 */

/**
 * Implements hook_views_default_views().
 */
function mailmon_views_default_views() {
  $views = array();
  $files = file_scan_directory(drupal_get_path('module', 'mailmon') . '/views', '/\.view$/');
  foreach ($files as $path => $file) {
    require $path;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }
  return $views;
}
