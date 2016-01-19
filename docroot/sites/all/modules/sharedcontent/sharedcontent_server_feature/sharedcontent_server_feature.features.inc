<?php
/**
 * @file
 * sharedcontent_server_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function sharedcontent_server_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function sharedcontent_server_feature_views_api() {
  return array("version" => "3.0");
}
