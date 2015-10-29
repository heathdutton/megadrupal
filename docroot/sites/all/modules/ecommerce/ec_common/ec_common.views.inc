<?php

/**
 * @file
 * Implement common views functionality.
 */

/**
 * Implements hook_views_data().
 */
function ec_common_views_data() {
  $data = array();

  $data['views']['ec_exposed_fieldset'] = array(
    'title' => t('Move Exposed filters'),
    'help' => t('This filter will have no effect on the sections by will allow exposed filters to be moved into a fieldset'),
    'filter' => array(
      'handler' => 'ec_common_views_handler_filter_exposed_fieldset',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function ec_common_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_common') . '/views',
    ),
    'handlers' => array(
      'ec_common_views_handler_field_format_currency' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_common_views_handler_filter_exposed_fieldset' => array(
        'parent' => 'views_handler_filter',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function ec_common_views_plugins() {
  return array(
    'cache' => array(
      'time_no_post' => array(
        'path' => drupal_get_path('module', 'ec_common') . '/views',
        'title' => t('Time-based with form support'),
        'help' => t('Simple time-based caching of data with support for handing form posts'),
        'handler' => 'ec_common_views_plugin_cache_time_no_form',
        'uses options' => TRUE,
        'help topic' => 'cache-time',
        'parent' => 'time',
      ),
    ),
    'style' => array(
      'table_multi' => array(
        'title' => t('Table (multi row)'),
        'help' => t('Displays rows in a table.'),
        'handler' => 'ec_plugin_style_table_multi',
        'theme' => 'ec_view_table_multi',
        'theme path' => drupal_get_path('module', 'ec_common') . '/templates',
        'theme file' => 'theme.inc',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-table',
        'path' => drupal_get_path('module', 'ec_common') . '/views',
        'parent' => 'table',
      ),
    ),
  );
}
