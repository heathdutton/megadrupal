<?php
/**
 * @file
 * Provide views data and handlers for views_coscev.module
 */

/**
 * Implements hook_views_plugins().
 */
function views_coscev_views_plugins() {
  return array(
    'module' => 'views_coscev',
    'style' => array(
      'views_coscev' => array(
        'title' => t('Content scroll (COSCEV)'),
        'help' => t('COntent SCroll from EVerywhere'),
        'handler' => 'views_coscev_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'type' => 'normal',
        'path' => drupal_get_path('module', 'views_coscev'),
        'theme path' => drupal_get_path('module', 'views_coscev'),
        'theme' => 'views_coscev_view',
      ),
    ),
  );
}
