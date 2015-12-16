<?php

/**
 * @file
 * Default views for the WP blog module.
 */

/**
 * Implements hook_views_default_views().
 */
function wp_blog_views_default_views() {
  $views = array();

  // Parse each .view file in the views folder.
  $path = dirname(__FILE__) . '/views';
  foreach (glob("{$path}/*.view") as $file) {
    include($file);
    $views[$view->name] = $view;
  }
  return $views;
}
