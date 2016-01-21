<?php

/**
 * @file
 * Simplyscroll views plugin definition
 */


/**
 * Implements of hook_views_plugins
 */
function simplyscroll_views_plugins() {
  return array(
    'module' => 'simplyscroll',
    'style' => array(
      'simplyscroll' => array(
        'title' => t('Simply Scroll'),
        'theme' => 'views_view_simplyscroll',
        'help' => t('Display one node at a time while rotating through them.'),
        'handler' => 'simplyscroll_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
