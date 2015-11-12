<?php

/**
 * @file
 * Contains the referer argument default views plugin.
 */

/**
 * Implements hook_views_plugins().
 */
function views_plugin_argument_default_referer_views_plugins() {
  return array(
    'argument default' => array(
      'referer' => array(
        'title' => t("Node ID from HTTP_REFERER"),
        'handler' => 'views_plugin_argument_default_referer',
      ),
    ),
  );
}
/**
 * Default argument plugin to retrieve the Node ID from HTTP_REFERER.
 * Assumes the Node ID immediately follows 'node/' in HTTP_REFERER.
 *
 * @ingroup views_argument_default_plugins
 */
class views_plugin_argument_default_referer extends views_plugin_argument_default {

  function get_argument() {
    $path = $_SERVER['HTTP_REFERER'];
    $path = explode('node/', $path);
    if (isset($path[1])) {
      $path = explode('/', $path[1]);
      $path = explode('?', $path[0]);
      if (is_numeric($path[0])) {
        return $path[0];
      }
    }
  }
}
