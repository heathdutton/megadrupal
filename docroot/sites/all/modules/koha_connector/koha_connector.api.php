<?php

/**
 * @file
 * Hooks provided by Koha connector module.
 */

/**
 * Declares one or more plugins that can be used by Koha connector.
 *
 * Plugins are called by getBiblios and are useful for formatting raw MARC data
 * into a more approriate format for users.
 * First parameter of callback is the entire MARC record, structured in arrays.
 * Second parameter is the configuration of a field. Plugin must return the
 * appropriate structure for this field, which is an array of strings.
 */
function hook_koha_connector_plugin_info() {
  $plugins = array(
    'my_plugin' => array(
      'callback' => 'my_plugin_callback',
      'file' => 'my_plugin.inc',
      'file path' => drupal_get_path('module', 'my_module') . '/includes',
    ),
  );
  return $plugins;
}
