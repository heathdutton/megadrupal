<?php
/**
 * @file
 * recruiter_register.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function recruiter_register_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
