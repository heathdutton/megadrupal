<?php
/**
 * @file
 * Views plugin implementation.
 */

/**
 * Implements hook_views_plugins().
 */
function semanticviews_tree_views_plugins() {
  return array(
    'style' => array(
      'semanticviews_tree' => array(
        'title' => t('Semantic Views Tree'),
        'help' => t('Displays rows in a hierarchy.'),
        'handler' => 'semanticviews_tree_plugin_style_tree',
        'theme' => 'semanticviews_tree_view_tree',
        'theme file' => 'semanticviews_tree.theme.inc',
        'theme path' => drupal_get_path('module', 'semanticviews_tree'),
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
        //'help topic' => 'style-tree',
      ),
    ),
  );
}
