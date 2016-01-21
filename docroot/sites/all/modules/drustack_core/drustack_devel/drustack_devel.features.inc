<?php
/**
 * @file
 * drustack_devel.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drustack_devel_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
