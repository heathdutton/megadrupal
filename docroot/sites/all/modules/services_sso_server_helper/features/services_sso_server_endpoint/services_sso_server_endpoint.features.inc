<?php
/**
 * @file
 * services_sso_server_endpoint.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function services_sso_server_endpoint_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
