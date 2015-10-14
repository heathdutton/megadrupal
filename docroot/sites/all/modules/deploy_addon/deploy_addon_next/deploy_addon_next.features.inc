<?php
/**
 * @file
 * deploy_addon.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function deploy_addon_next_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "deploy" && $api == "deploy_plans") {
    return array("version" => "1");
  }
}
