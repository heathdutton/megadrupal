<?php

/**
 * @file
 * Views plugins, handlers and hooks definition for Flipboard RSS module.
 */

/**
 * Implements hook_views_plugins().
 * Adds view types to views UI interface.
 */
function flipboardrss_views_plugins() {
  $plugins = array(
    'style' => array(
      'flipboardrss' => array(
        'title' => t('RSS Feed for Flipboard'),
        'help' => t('Generates an RSS feed compatible to Flipboard'),
        'handler' => 'flipboardrss_plugin_style_rss',
        'path' => FLIPBOARDRSS_PATH . '/views',
        'theme' => 'flipboardrss_view_rss',
        'theme file' => 'theme.inc',
        'theme path' => FLIPBOARDRSS_PATH . '/theme',
        'uses row plugin' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
//        'uses grouping' => FALSE,
        'type' => 'feed',
      ),
    ),
    'row' => array(
      'flipboardrss_fields' => array(
        'title' => t('Fields'),
        'help' => t('Display fields as RSS items.'),
        'handler' => 'flipboardrss_plugin_row_rss_fields',
        'path' => FLIPBOARDRSS_PATH . '/views',
        'theme' => 'flipboardrss_view_rss_row',
        'theme file' => 'theme.inc',
        'theme path' => FLIPBOARDRSS_PATH . '/theme',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'help topic' => 'style-row-fields',
      ),
    ),
  );

  return $plugins;
}
