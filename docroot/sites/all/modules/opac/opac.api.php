<?php

/**
 * @file
 * Description of hooks provided by OPAC module.
 */

/**
 * Declaration of plugins for OPAC node fields.
 */
function hook_opac_node_field_plugin_info() {
  $plugins = array(
    'plugin name' => array(
      // Label used for display.
      'label' => t('Plugin name'),

      // File to include when calling a callback function.
      'file' => 'plugin.name.inc',

      // Directory where to look for plugin.name.inc
      // If ommitted, file is expected to be in module root directory.
      'file path' => 'path/to/pluginfile/',

      // This callback takes 2 parameters:
      // - $server: an OpacServer object.
      // - $field: an array containing field informations.
      // It should returns TRUE if plugin is available for these server and
      // field. FALSE otherwise.
      'available callback' => 'plugin_name_available_callback',

      // This callback will be responsible for building value that will be
      // stored in field.
      // It takes 2 parameters:
      // - $record: an array, as one of arrays returned by getBiblios
      // - $mapping: an array that contains mapping informations.
      // It should returns the value to be stored in node field.
      'value callback' => 'plugin_name_value_callback',

      // This (optional) callback allow the plugin to add form elements to
      // mapping form.
      // It takes 3 parameters:
      // - &$form_state.
      // - $field: an array containing field informations.
      // - $mapping: an array containing mapping informations.
      // It should returns an array as expected by drupal_get_form().
      'form callback' => 'plugin_name_form_callback',
    ),
  );

  return $plugins;
}
