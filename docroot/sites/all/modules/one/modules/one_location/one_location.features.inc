<?php
/**
 * @file
 * one_location.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function one_location_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function one_location_views_api() {
  return array("api" => "3.0");
}
