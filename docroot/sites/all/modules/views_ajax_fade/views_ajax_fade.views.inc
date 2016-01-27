<?php

/**
 * @file
 * Contains meta info for views api about the ajax fade plugin.
 */

/**
 * Implements hook_views_plugin().
 */
function views_ajax_fade_views_plugins() {
  $path = drupal_get_path('module', 'views_ajax_fade');
  $js   = $path . '/js/views_ajax_fade.js';

  $plugins = array();
  $plugins['display_extender']['views_ajax_fade'] = array(
    'title' => t('Fade Ajax in'),
    'path' => $path,
    'uses options' => TRUE,
    'handler' => 'views_plugin_ajax_fade',
  );
  // We add the js here so its only loaded when
  // our module is called and not every page.
  drupal_add_js($js, 'file');

  return $plugins;
}
