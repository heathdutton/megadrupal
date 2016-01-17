<?php

/**
 * @file
 * Implementation of default Views.
 */

/**
 * Implement hook_views_default_views().
 */
function search_log_views_default_views() {
  $views = array();
  $api = search_log_views_api();
  $path = $api['path'];

  require_once $path . '/search_log_default.inc';
  $views[$view->name] = $view;

  return $views;
}
