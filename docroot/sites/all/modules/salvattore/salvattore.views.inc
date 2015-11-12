<?php
/**
 * @file
 * Provide views data and handlers for salvattore.module.
 */

/**
 * Implements hook_views_plugins().
 */
function salvattore_views_plugins() {
  return array(
    'style' => array(
      'salvattore' => array(
        'title' => t('Salvattore'),
        'handler' => 'views_plugin_style_salvattore',
        'help' => t("Display content in a masonry layout using Salvattore."),
        'theme' => 'views_view_salvattore',
        'type' => 'normal',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
      ),
    ),
  );
}

