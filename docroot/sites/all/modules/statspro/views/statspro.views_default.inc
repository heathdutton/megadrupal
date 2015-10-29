<?php
// $Id: statspro.views_default.inc,v 1.2.4.3 2010/06/10 20:06:57 rsevero Exp $

/**
 * @file
 *   Default views for Statistics Pro module.
 *
 */

/**
 * Declare all the .view files in the views subdir that end in .view.
 */
function statspro_views_default_views() {
  $files = file_scan_directory(drupal_get_path('module', 'statspro') . '/views/defaults', '/.view/');
  foreach ($files as $absolute => $file) {
    require DRUPAL_ROOT . '/' . $absolute;
    $views[$view->name] = $view;
  }

  return $views;
}
