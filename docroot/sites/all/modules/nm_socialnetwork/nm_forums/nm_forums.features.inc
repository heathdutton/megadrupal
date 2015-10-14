<?php
/**
 * @file
 * nm_forums.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function nm_forums_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
