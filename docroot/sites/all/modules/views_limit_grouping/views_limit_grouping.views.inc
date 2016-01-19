<?php

/**
 * @file views_limit_grouping.views.inc
 * Just describing our Views style plugin; nothing much to see here.
 */

/**
 * Implements hook_views_plugins().
 */
function views_limit_grouping_views_plugins() {
  return array(
    'style' => array(
      'grouping_limit' => array(
        'title' => t('Grouping Field (with Limit)'),
        'help' => t('Limit the number of rows for each grouping field'),
        'handler' => 'views_limit_grouping_style_plugin',
        'theme' => 'views_limit_grouping',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}