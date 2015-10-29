<?php
/**
 * @file
 * Implementation of default Views.
 *
 * @copyright (c) 2005-2013 Ouest Systemes Informatiques (OSInet)
 *
 * @license Licensed under the General Public License version 2 or later.
 */

/**
 * Implements hook_views_default_views().
 */
function zeitgeist_views_default_views() {
  $views = array();
  $api = zeitgeist_views_api();
  $path = $api['path'];

  require_once "$path/zeitgeist_default.view.inc";
  $views[$view->name] = $view;

  return $views;
}
