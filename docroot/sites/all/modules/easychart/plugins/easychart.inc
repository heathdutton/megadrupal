<?php

/**
 * @file
 * Provides the Easychart Wysiwyg plugin.
 */

/**
 * Implements hook_wysiwyg_plugin().
 */
function easychart_easychart_plugin() {

  // add necessary jQuery ui libs
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'ui.dialog');

  drupal_add_js(array('easychart' => array('module_path' => drupal_get_path('module', 'easychart'))), 'setting');

  $plugins['easychart'] = array(
    'title' => t('Easychart'),
    'vendor url' => 'http://drupal.org/project/easychart',
    'icon path' => drupal_get_path('module', 'easychart') . '/plugins/images',
    'icon file' => 'icon.png',
    'icon title' => t('Add a chart'),
    'js path' => drupal_get_path('module', 'easychart') . '/plugins/js',
    'js file' => 'plugin.js',
    'css path' => drupal_get_path('module', 'easychart') . '/plugins/css',
    'css file' => 'plugin.css',
    'settings' => array(),
  );
  return $plugins;
}
