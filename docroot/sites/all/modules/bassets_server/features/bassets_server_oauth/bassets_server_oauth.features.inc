<?php
/**
 * @file
 * bassets_server_oauth.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bassets_server_oauth_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "oauth_common" && $api == "oauth") {
    return array("version" => "1");
  }
}
