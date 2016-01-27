<?php
/**
 * @file
 * enterprise_forum.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_forum_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
