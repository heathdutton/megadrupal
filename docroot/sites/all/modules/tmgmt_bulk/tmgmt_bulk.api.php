<?php

/**
 * @file
 * API documentation for the TMGMT Bulk module.
 */

/**
 * Provide information about bulk_selector plugins.
 *
 * The bulk_selector plugins are extensions for the source plugins. They allow
 * to search for the source items with user provided conditions. Machine names
 * should coincide with source plugin machine names.
 *
 * Only those plugins will be used for which source plugins exist.
 *
 * It's important that plugin class must extend the TMGMTBulkSelectorBase class
 * and implement the TMGMTBulkSelectorInterface. Otherwise it's skipped.
 *
 * @return array
 *   An associative array. Keys are plugin machine names. Values are associative
 *   arrays providing plugin information. The only required property of the
 *   nested array is "plugin controller class" which should provide the plugin
 *   class name.
 *
 * @see TMGMTBulkSelectorInterface
 */
function hook_tmgmt_bulk_selector_plugin_info() {
  return array(
    'entity' => array(
      'plugin controller class' => 'TMGMTBulkSelectorEntity',
    ),
    'locale' => array(
      'plugin controller class' => 'TMGMTBulkSelectorLocale',
    ),
  );
}
