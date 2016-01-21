<?php
/**
 * @file
 * Provide views data and handlers for savvior.module.
 */

/**
 * Implements hook_views_plugins().
 */
function savvior_views_plugins() {
  return array(
    'style' => array(
      'savvior' => array(
        'title' => t('Savvior'),
        'handler' => 'views_plugin_style_savvior',
        'help' => t("Display content in a masonry layout using Savvior."),
        'theme' => 'views_view_savvior',
        'type' => 'normal',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
      ),
    ),
  );
}

