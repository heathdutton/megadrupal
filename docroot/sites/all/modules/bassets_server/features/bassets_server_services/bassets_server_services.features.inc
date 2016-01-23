<?php
/**
 * @file
 * bassets_server_services.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bassets_server_services_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
  if ($module == "services_client_connection" && $api == "default_services_client_connection") {
    return array("version" => "1");
  }
}
