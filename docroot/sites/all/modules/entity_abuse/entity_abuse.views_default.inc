<?php

/**
 * @file
 * Contains process function for adding default views.
 */

/**
 * Implements hook_views_default_views().
 */
function entity_abuse_views_default_views() {

  $views = &drupal_static(__FUNCTION__);
  if (isset($views)) {
    return $views;
  }

  $dir = DRUPAL_ROOT . '/' . drupal_get_path('module', 'entity_abuse') . '/views_default';
  foreach (file_scan_directory($dir, '/\.inc$/') as $filepath => $file) {
    include $filepath;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }

  return $views;
}
