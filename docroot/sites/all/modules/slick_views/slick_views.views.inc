<?php
/**
 * @file
 * Contains core functions for the Views module support.
 */

/**
 * Implements hook_views_plugins().
 */
function slick_views_views_plugins() {
  return array(
    'style' => array(
      'slick' => array(
        'title' => t('Slick carousel'),
        'help' => t('Display the results in a Slick carousel widget.'),
        'handler' => 'SlickViews',
        'theme' => 'slick_views',
        'theme file' => 'slick_views.theme.inc',
        'theme path' => drupal_get_path('module', 'slick_views') . '/templates',
        'uses row plugin' => TRUE,
        // 'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
