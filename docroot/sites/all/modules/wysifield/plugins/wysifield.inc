<?php
/**
 * @file
 * Wysifield plugin.
 */

function wysifield_wysifield_plugin() {

  $metadata = wysifield_get_field_metadata();
  $plugins = array();

  // Parse bundles for settings information and create plugins array.
  foreach ($metadata as $plugin_name => $data) {
    $plugins[$plugin_name] = $data;
  }

  return $plugins;
}
