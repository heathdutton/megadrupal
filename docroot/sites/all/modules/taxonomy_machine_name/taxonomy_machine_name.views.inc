<?php

/**
 * @file
 * Taxonomy Machine Name Views Module File.
 */

/**
 * Implements hook_views_data_alter().
 */
function taxonomy_machine_name_views_data_alter(&$data) {
  $data['taxonomy_term_data']['machine_name'] = array(
    'title'    => t('Machine name'),
    'help'     => t('The taxonomy term machine name.'),
    'field'    => array(
      'handler'        => 'views_plugin_field_taxonomy_term_machine_name',
      'click sortable' => TRUE,
    ),
    'sort'     => array(
      'handler' => 'views_handler_sort',
    ),
    'filter'   => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
}

/**
 * Implements hook_views_plugins_alter().
 */
function taxonomy_machine_name_views_plugins_alter(&$plugins) {
  if (isset($plugins['argument validator']['taxonomy_term']['handler'])) {
    $handler = 'views_plugin_argument_validate_taxonomy_term_machine_name';
    $path = drupal_get_path('module', 'taxonomy_machine_name') . '/views/modules/taxonomy';
    $file = 'views_plugin_argument_validate_taxonomy_term_machine_name.inc';

    $plugins['argument validator']['taxonomy_term']['handler'] = $handler;
    $plugins['argument validator']['taxonomy_term']['path'] = $path;
    $plugins['argument validator']['taxonomy_term']['file'] = $file;
    $plugins['argument validator']['taxonomy_term']['module'] = 'taxonomy_machine_name';
  }
}
