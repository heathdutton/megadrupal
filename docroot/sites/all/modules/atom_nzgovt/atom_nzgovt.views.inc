<?php

/**
 * @file
 * Views plugins definition for atom_nzgovt
 */

/**
 * Implements hook_views_plugins()
 */
function atom_nzgovt_views_plugins() {
  $theme_path = drupal_get_path('module', 'atom_nzgovt');
  return array(
    'module' => 'atom_nzgovt',
    'style' => array(
      'atom' => array(
        'title' => t('ATOM Feed'),
        'handler' => 'views_plugin_style_atom',
        'theme' => 'views_view_atom',
        'theme path' => $theme_path,
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'help' => t('Generates an ATOM feed from a view.'),
      ),
    ),
    'row' => array(
      'atom_node' => array(
        'title' => t('ATOM nodes'),
        'handler' => 'views_plugin_row_node_atom',
        'path' => "$theme_path",
        'theme' => 'views_view_row_atom',
        'theme path' => "$theme_path",
        'base' => array('node'),
        'uses options' => TRUE,
        'uses fields' => FALSE,
        'type' => 'feed',
        'help' => t('Displays the nodes with an optional template.'),
      ),
    ),
  );
}
