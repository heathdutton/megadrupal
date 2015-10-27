<?php
/**
 * @file
 * bassets_server_image_metadata_rules.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bassets_server_image_metadata_rules_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
