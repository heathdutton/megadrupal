<?php
/**
 * @file
 * Library providing additional tools for working with ctools plugins
 */
$plugin = array (
  'title' => t('Plugin tools (ctools)'),
  'description' => t('Library for working with ctools plugins.'),
  'xu_type' => 'library'
);

/**
 * Fetch metadata on a specific ctools plugin.
 *
 * Note that this is rather redundant, you may be better writing your own
 * shorter version of this function.
 *
 * @param $module
 *  The module which owns the plugins
 * @param $type
 *  The type of plugin
 * @param $plugin
 *  Name of the plugin
 *
 * @return array
 *  An array with information about the requested plugin
 */
function xu_ctools_get_plugin($module, $type, $plugin) {
  ctools_include('plugins');
  return ctools_get_plugins($module, $type, $plugin);
}

/**
 * Fetch metadata for all module plugins.
 *
 * Note that this is rather redundant, you may be better writing your own
 * shorter version of this function.
 *
 * @param $module
 *  The module which owns the plugins
 * @param $type
 *  The type of plugin
 *
 * @return array
 *  An array of arrays with information about all available plugins
 */
function xu_ctools_get_plugins($module, $type) {
  ctools_include('plugins');
  return ctools_get_plugins($module, $type);
}

/**
 * Call a ctools plugin function
 *
 * This assumes that any arguments you pass to the plugin function are in a single
 * array.
 *
 * @param $plugin
 *  The plugin which owns the callback
 * @param $callback
 *  The callback to look for
 * @param array $args
 *  Any arguments to pass to the callback
 */
function xu_ctools_plugin_function($plugin, $callback, &$args = array()) {
  if ($function = ctools_plugin_get_function($plugin, $callback)) {
    $function($args);
  }
}

/**
 * Given a list of ctools plugins, return a listing suitable for display
 *
 *
 *
 * @param array $plugins
 *  An array of plugin information. Current we use the keys
 *  'title' The human readable title
 *  'name'  The machine name
 *  'module'  The providing module
 *  'description' A description of the module
 * @param array $options [optional]
 *  An array of options for the listing function, including:
 *  'extra_keys'  An array of extra plugin settings keys to output, as key => label pairs.
 *
 * @return mixed
 */
function xu_ctools_render_plugin_list($plugins, $options = array()) {

  $header = array(t('Title'), t('Name'), t('Provider'), t('Description'));
  if (isset($options['extra_keys']) && is_array($options['extra_keys'])) {
    foreach($options['extra_keys'] as $key => $label) {
      $header[] = $label;
    }
  }

  $rows = array();
  foreach ($plugins as $plugin) {
    $row = array();
    isset($plugin['title']) ? $row[] = $plugin['title'] : $row[] = '';
    isset($plugin['name']) ? $row[] = $plugin['name'] : $row[] = '';
    isset($plugin['module']) ? $row[] = $plugin['module'] : $row[] = '';
    isset($plugin['description']) ? $row[] = $plugin['description'] : $row[] = '';
    if (isset($options['extra_keys']) && is_array($options['extra_keys'])) {
      foreach($options['extra_keys'] as $key => $label) {
        isset($plugin[$key]) ? $row[] = $plugin[$key] : $row[] = '';
      }
    }
    $rows[] = $row;
  }
  return theme('table', $header, $rows);
}

/**
 * Wrapper to return render_plugin_list using only a plugin key
 *
 * @param $module
 * @param $plugin
 *
 * @return mixed
 * @internal param $
 */
function xu_ctools_plugins_list($module, $plugin, $options = array()) {
  $plugins = xu_ctools_get_plugins($module, $plugin);
  return xu_ctools_render_plugin_list($plugins, $options);
}
