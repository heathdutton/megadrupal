<?php
/**
 * @file
 * Provide an rowspan style plugin for Views. This file is autoloaded by views.
 */

/**
 * Implements of hook_views_plugin().
 */
function rowspan_views_plugins() {
  return array(
    'style' => array(
      'views_rowspan' => array(
        'title' => t('Common vertical cells'),
        'theme' => 'views_view_rowspan',
        'theme path' => drupal_get_path('module', 'rowspan') . '/theme',
        'theme file' => 'theme.inc',
        'handler' => 'RowspanStylePlugin',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
