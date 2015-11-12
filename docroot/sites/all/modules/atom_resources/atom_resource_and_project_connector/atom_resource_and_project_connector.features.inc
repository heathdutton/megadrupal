<?php
/**
 * @file
 * atom_resource_and_project_connector.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function atom_resource_and_project_connector_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
