<?php
/**
 * @file
 * Attach API.
 */

/**
 * Add plugins for Attach module.
 *
 * This hook must return a keyed array with the following elements:
 * - title: used in filter settings description. Required.
 * - description: same as title. Optional.
 * - callback: function that processes the data. Required.
 * - file: where to look for the callback function. Optional.
 * - path: path of file. Required if 'file' is not empty.
 * - options: reserved. Optional.
 * - suffixes: keyed array. Required.
 */
function hook_attach_plugin() {
  $plugins = array(
    'youtube' => array(
      'title' => 'Attach YouTube video',
      'description' => 'Use this syntax: [attach_yt|url=http://www.youtube.com/watch?v=123456789]',
      'callback' => 'mymodule_attach_youtube_process',
      'options' => array(),
      'suffixes' => array('yt'),
    ),
  );

  return $plugins;
}

/**
 * Example for hook_attach_plugin_alter().
 */
function hook_attach_plugin_alter(&$plugins) {
  // Example - change the default width and height of flash plugin
  $plugins['flash']['options']['width'] = 600;
  $plugins['flash']['options']['height'] = 400;
}

