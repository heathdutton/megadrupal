<?php
/**
 * @file
 * drushweb.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drushweb_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "oauth_common" && $api == "oauth") {
    return array("version" => "1");
  }
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
