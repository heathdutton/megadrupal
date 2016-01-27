<?php
/**
 * @file
 * austrofeedr_base_admin.features.inc
 */

/**
 * Implements hook_views_api().
 */
function austrofeedr_base_admin_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
