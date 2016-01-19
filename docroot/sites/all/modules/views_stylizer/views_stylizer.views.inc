<?php

/**
 * @file
 * Views include file with views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function views_stylizer_views_plugins() {
  $plugins = array(
      'style' => array(
          'table_stylizer' => array(
              'title' => t('Dynamic Style'),
              'help' => t('Displays content in chosen style'),
              'handler' => 'views_plugin_style_views_stylizer',
              'theme' => 'views_view_stylizer',
              'uses row plugin' => TRUE,
              'uses row class' => TRUE,
              'uses fields' => TRUE,
              'uses options' => TRUE,
              'type' => 'normal',
              'help topic' => 'style-table',
          ),
      ),
  );
  return $plugins;
}