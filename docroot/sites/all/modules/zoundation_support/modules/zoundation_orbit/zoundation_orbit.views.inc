<?php

/**
 * @file
 * Views include file for zoundation_orbit.
 */

/**
 * Implements hook_views_plugins().
 */
function zoundation_orbit_views_plugins() {
  $plugins = array();

  $plugins['style']['zoundation_orbit_slideshow'] = array(
    'title' => t('Zoundation Orbit Slideshow'),
    'help' => t('Displays rows in an orbit slideshow'),
    'handler' => 'views_plugin_style_zoundation_orbit_slideshow',
    'theme' => 'zoundation_orbit_slideshow',
    'uses row plugin' => TRUE,
    'uses row class' => FALSE,
    'uses options' => TRUE,
    'type' => 'normal',
    'path' => drupal_get_path('module', 'zoundation_orbit') . '/views',
  );

  return $plugins;
}
