<?php

/**
 * @file Test helper functions
 * Included when simpletest is installed.
 */

/**
 * Implements hook_views_default_views().
 */
function views_access_many_views_default_views() {
  $views = array ();

  $path = drupal_get_path('module', 'views_access_many') . '/tests/views';
  $files = file_scan_directory($path, '/\.inc$/');

  foreach ($files as $file) {
    include DRUPAL_ROOT . '/' . $path . '/' . $file->filename;
    $views[$view->name] = $view;
  }

  return $views;
}


/**
 * Augments hook_views_plugins()
 * (When simpletest is installed).
 */
function _views_access_many_test_views_plugins(&$plugins) {
  $test_path = drupal_get_path('module', 'views_access_many') . '/tests';
  $plugins['access']['true'] = array (
    'title' => t('Always TRUE'),
    'help' => t('This plugin always allows access to the view.'),
    'handler' => 'views_plugin_access_true',
    'path' => $test_path,
    'file' => 'views_plugin_access_degenerative.inc',
    'uses options' => TRUE,
    'help topic' => 'access',
  );
  $plugins['access']['false'] = array (
    'title' => t('Always FALSE'),
    'help' => t('This plugin never allows access to the view.'),
    'handler' => 'views_plugin_access_false',
    'path' => $test_path,
    'file' => 'views_plugin_access_degenerative.inc',
    'uses options' => TRUE,
    'help topic' => 'access',
  );
}