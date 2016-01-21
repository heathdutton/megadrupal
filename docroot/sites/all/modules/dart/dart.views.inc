<?php

/**
 * Implementats hook_views_plugins
 * Based off the same in views/modules/node.views.inc
 */
function dart_views_plugins() {

  return array(
    'module' => 'dart', // This just tells our themes are elsewhere.
    'row' => array(
      'dart_node' => array(
        'title' => t('Content with DART Ads'),
        'help' => t('Display the node with standard node view, but with a dash of DART.'),
        'handler' => 'views_plugin_row_dart_node_view',
        'path' => drupal_get_path('module', 'dart') . '/views',
        'theme' => 'dart_views_view_row_node',
        'base' => array('node'), // only works with 'node' as base.
        'type' => 'normal',
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-node',
      ),
    ),
  );
}
