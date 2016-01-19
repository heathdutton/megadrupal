<?php

/**
 * @file
 * Define the WYSIWYG browser plugin.
 */

/**
 * Implements WYSIWYG's hook_INCLUDE_plugin().
 */
function hotblocks_wysiwyg_hotblocks_plugin() {
  // add necessary jQuery libs
  drupal_add_library('system', 'jquery.form', TRUE);
  drupal_add_library('system', 'ui');
  drupal_add_library('system', 'ui.dialog');
  drupal_add_library('system', 'ui.tabs');

  $plugin_path = drupal_get_path('module', 'hotblocks_wysiwyg') . '/plugins';

  // Load the quicksearch plugin for our modal, as well as the main hotblocks.js and modal css
  $path_to_hotblocks = drupal_get_path('module', 'hotblocks');
  drupal_add_js($path_to_hotblocks . '/js/jquery.quicksearch/jquery.quicksearch.js');
  drupal_add_js($path_to_hotblocks . '/js/hotblocks.js');
  drupal_add_css($path_to_hotblocks . '/css/hotblocks-modal.css');

  // Plugin definition.
  $plugins['hotblocks'] = array(
    'title'      => t('Hotblocks'),
    'vendor url' => 'http://drupal.org/project/hotblocks',
    'icon path'  => $plugin_path,
    'icon file'  => 'assign.png',
    'icon title' => t('Add content'),
    'js path'    => $plugin_path,
    'js file'    => 'hotblocks_wysiwyg.js',
    'css path'   => $plugin_path,
    'css file'   => 'hotblocks_wysiwyg.css',
    'settings'   => array(
      //'global' => array(
      //  'enabledPlugins' => variable_get('hotblocks_wysiwyg_wysiwyg_browser_plugins', array()),
      //  'file_directory' => variable_get('hotblocks_wysiwyg_wysiwyg_upload_directory', ''),
      //  'types' => variable_get('hotblocks_wysiwyg_wysiwyg_allowed_types', array('audio', 'image', 'video', 'document')),
      //  'id' => 'hotblocks_wysiwyg',
      //),
    ),
  );

  return $plugins;
}
