<?php

/**
 * @file
 * Provides views plugins.
 */

/**
 * Implements hook_views_plugins().
 */
function dfv_views_plugins() {
  $plugins = array(
    'display' => array(
      'dfv' => array(
        'title' => t('Dependent Field Values'),
        'admin' => t('Dependent Field Values Source'),
        'help' => 'Selects referenceable entities for an entity reference field',
        'handler' => 'dfv_plugin_display',
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        // Custom property, used with views_get_applicable_views() to retrieve
        // all views with a 'dfv' display.
        'dfv display' => TRUE,
      ),
    ),
    'style' => array(
      'dfv_style' => array(
        'title' => t('dfv list'),
        'help' => 'Returns results as a PHP array of labels and rendered rows.',
        'handler' => 'dfv_plugin_style',
        'theme' => 'views_view_unformatted',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'dfv',
        'even empty' => TRUE,
      ),
    ),
    'row' => array(
      'dfv_fields' => array(
        'title' => t('Inline fields'),
        'help' => t('Displays the fields with an optional template.'),
        'handler' => 'dfv_plugin_row_fields',
        'theme' => 'views_view_fields',
        'theme path' => drupal_get_path('module', 'views') . '/theme',
        'theme file' => 'theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'dfv',
      ),
    ),
  );
  return $plugins;
}
