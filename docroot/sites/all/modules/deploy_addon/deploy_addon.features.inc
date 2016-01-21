<?php
/**
 * @file
 * deploy_addon.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function deploy_addon_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  if ($module == "deploy" && $api == "deploy_endpoints") {
    return array("version" => "1");
  }
}
