<?php
/**
 * @file
 * ngapp_example_rest_server.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ngapp_example_rest_server_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
