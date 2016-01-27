<?php

/**
 * @file
 * Defines the View Style Plugins for Timeline module.
 */

/**
 * Implements hook_views_style_plugins().
 */
function timelinr_views_plugins() {
  return array(
    'module' => 'timelinr',
    'style' => array(
      'timelinr' => array(
        'title' => t('Views Timelinr'),
        'help' => t('Displays content on jQuery timelinr.'),
        'handler' => 'timelinr_views_plugin_style',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
        'theme' => 'views_view_timelinr',
        'theme file' => 'timelinr.theme.inc',
      ),
    ),
    'row' => array(
      'timelinr' => array(
        'title' => t('Views Timelinr'),
        'help' => t('Choose the fields to display in Views Timelinr.'),
        'handler' => 'timelinr_plugin_row_timelinrfields',
        'theme' => 'timelinr_view_timelinrfields',
        'theme file' => 'timelinr.theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
