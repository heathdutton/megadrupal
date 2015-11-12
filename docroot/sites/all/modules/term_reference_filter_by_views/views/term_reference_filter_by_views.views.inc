<?php

/**
 * @file
 * Views integration for Entity Reference.
 */

/**
 * Implements hook_views_plugins().
 */
function term_reference_filter_by_views_views_plugins() {
  $plugins = array(
    'display' => array(
      'term_reference' => array(
        'title' => t('Term Reference'),
        'admin' => t('Term Reference Source'),
        'help' => 'Selects referenceable entities for an entity reference field',
        'handler' => 'term_reference_plugin_display',
        'uses hook menu' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        // Custom property, used with views_get_applicable_views() to retrieve
        // all views with a 'Term Reference' display.
        'term_reference display' => TRUE,
        'base' => array('taxonomy_term_data'),
      ),
    ),
    'style' => array(
      'term_reference_style' => array(
        'title' => t('Term Reference list'),
        'help' => 'Returns results as a PHP array of labels and rendered rows.',
        'handler' => 'term_reference_plugin_style',
        'theme' => 'views_view_unformatted',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'term_reference',
        'even empty' => TRUE,
      ),
    ),
    'row' => array(
      'term_reference_fields' => array(
        'title' => t('Inline fields'),
        'help' => t('Displays the fields with an optional template.'),
        'handler' => 'term_reference_plugin_row_fields',
        'theme' => 'views_view_fields',
        'theme path' => drupal_get_path('module', 'views') . '/theme',
        'theme file' => 'theme.inc',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'term_reference',
      ),
    ),
  );
  return $plugins;
}