<?php
/**
 * @file
 * Provide an quicksand style plugin for Views. This file is autoloaded by views.
 */

/**
 * Implements hook_views_plugin().
 */
function views_quicksand_views_plugins() {
  return array(
    'style' => array(
      'views_quicksand' => array(
        'title' => t('Views Quicksand'),
        'theme' => 'views_view_quicksand',
        'help' => t('Uses the jQuery Quicksand plugin to reorder or filter your content.'),
        'handler' => 'views_quicksand_style_plugin',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

