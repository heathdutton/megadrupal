<?php
/**
 * @file
 * trello_oauth.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function trello_oauth_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "oauthconnector" && $api == "oauthconnector") {
    return array("version" => "1");
  }
}
