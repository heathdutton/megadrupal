<?php
/**
 * @file
 * Provide ctools plugin definitions and implementation hooks.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 * @see http://panels3.dev.logrus.com/help/ctools/plugins-creating
 * @see http://panels3.dev.logrus.com/help/ctools/plugins-implementing
 */

/**
 * Fetch metadata on a specific plugin.
 *
 * @param $type
 *   Plugin type.
 * @param $plugin
 *   Name of plugin.
 * @return
 *   An array of information about the requested plugin.
 */
function worker_plugins_get($type, $plugin) {
  ctools_include('plugins');
  return ctools_get_plugins('worker', $type, $plugin);
}

/**
 * Fetch metadata on the plugin appropriate for handling the URL scheme.
 *
 * @param $url
 *   A VCS repository URL.
 * @return
 *   An array of information about the requested plugin.
 * @see worker_plugins_get()
 */
function worker_plugins_get_vcs($url) {
  // Note: parse_url() will not parse some non-standard urls (e.g., git).
  // Possibly use a custom format?
  list($scheme) = explode('://', $url);
  return worker_plugins_get('vcs', $scheme);
}

/**
 * Fetch metadata for all plugins.
 *
 * @return
 *   An array of arrays with information about all available plugins.
 */
function worker_plugins_get_all($type) {
  ctools_include('plugins');
  return ctools_get_plugins('worker', $type);
}

/**
 * Implements hook_ctools_plugin_type().
 */
function worker_ctools_plugin_type() {
  return array(
    'database' => array(),
    'vcs' => array(),
    'worker' => array(),
  );
}

/**
 * Implements hook_ctools_plugin_directory().
 */
function worker_ctools_plugin_directory($module, $plugin) {
  if ($module == 'worker' && ($plugin == 'database' || $plugin == 'vcs' || $plugin == 'worker')) {
    return 'plugins/' . $plugin;
  }
}
