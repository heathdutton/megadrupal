<?php
/**
 * @file
 * Provide views data and handlers.
 */

/**
 * Implements hook_views_plugins().
 */
function views_job_board_views_plugins() {
  $module_path = drupal_get_path('module', 'views_job_board');

  $plugins = array(
    'style' => array(
      'views_job_board_plugin_style' => array(
        'title' => t('Job Board'),
        'help' => t('Displays Job Board table.'),
        'path' =>  $module_path . '/plugins',
        'handler' => 'ViewsJobBoardPluginStyle',
        'parent' => 'table',
        'theme' => 'views_job_board',
        'theme path' => $module_path . '/templates',
        'theme file' => 'theme.inc',
        'uses row plugin' => FALSE,
        'uses row class' => FALSE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );

  return $plugins;
}
