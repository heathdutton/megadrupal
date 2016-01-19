<?php

/**
 * @file
 * Provide views plugin for views_arg_parent_term.module.
 */

/**
 * Implements hook_views_plugins().
 */
function views_arg_parent_term_views_plugins() {
  return array(
    'module' => 'views_arg_parent_term',
    'argument default' => array(
      'taxonomy_parent_tid' => array(
        'title' => t('Taxonomy parent term ID from URL'),
        'handler' => 'views_arg_parent_term_plugin_argument_default_parent_taxonomy_tid',
        'path' => drupal_get_path('module', 'views_arg_parent_term') . '/includes',
        'parent' => 'fixed',
      ),
    ),
  );
}
