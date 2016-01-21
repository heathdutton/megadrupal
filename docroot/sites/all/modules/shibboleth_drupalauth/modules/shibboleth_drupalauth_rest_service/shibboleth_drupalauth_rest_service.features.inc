<?php
/**
 * @file
 * shibboleth_drupalauth_rest_service.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function shibboleth_drupalauth_rest_service_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
