<?php

/**
 * @file
 * SwipeView views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function views_swipeview_views_plugins() {
  $module_path = drupal_get_path('module', 'views_swipeview');
  return array(
    'style' => array(
      'swipeview' => array(
        'title' => t('SwipeView'),
        'help' => t('Display the view rows in a SwipeView slideshow.'),
        'handler' => 'views_swipeview_plugin_style_swipeview',
        'theme' => 'views_swipeview_slideshow',
        'theme path' => $module_path . '/theme',
        'theme file' => 'views_swipeview.theme.inc',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
