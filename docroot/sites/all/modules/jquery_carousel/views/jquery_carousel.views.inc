<?php
/**
 * @file
 * Defines the View Style Plugins for jQuery Carousel module.
 */

/**
 * Implements hook_views_plugins().
 */
function jquery_carousel_views_plugins() {
  return array(
    'style' => array(
      'jquery_carousel' => array(
        'title' => t('jQuery Carousel'),
        'help' => t('Display the results as a jquery Carousel.'),
        'handler' => 'JqueryCarouselPluginStyleCarousel',
        'path' => drupal_get_path('module', 'jquery_carousel') . '/views/plugins',
        'type' => 'normal',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses grouping' => FALSE,
        'uses row class' => TRUE,
        'theme' => 'jquery_carousel',
        'theme path' => drupal_get_path('module', 'jquery_carousel') . '/views/theme',
        'theme file' => 'jquery_carousel.theme.inc',
      ),
    ),
  );
}
