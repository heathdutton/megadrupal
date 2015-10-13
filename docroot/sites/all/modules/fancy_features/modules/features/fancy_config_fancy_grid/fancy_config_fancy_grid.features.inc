<?php
/**
 * @file
 * fancy_config_fancy_grid.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fancy_config_fancy_grid_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "breakpoints" && $api == "default_breakpoint_group") {
    return array("version" => "1");
  }
  if ($module == "breakpoints" && $api == "default_breakpoints") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
