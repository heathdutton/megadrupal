<?php
/**
 * @file
 * hosting_network_node.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hosting_network_node_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "oauth_common" && $api == "oauth") {
    return array("version" => "1");
  }
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
