<?php
/**
 * @file
 * nm_activity.features.inc
 */


/**
 * Implements hook_ctools_plugin_api().
 */
function nm_activity_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function nm_activity_views_api() {
  return array("version" => "3.0");
}