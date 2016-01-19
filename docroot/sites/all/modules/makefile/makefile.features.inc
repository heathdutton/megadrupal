<?php
/**
 * @file
 * makefile.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function makefile_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "file_entity" && $api == "file_type") {
    return array("version" => "1");
  }
}
