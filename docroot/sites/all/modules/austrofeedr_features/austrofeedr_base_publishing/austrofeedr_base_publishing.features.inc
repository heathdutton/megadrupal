<?php
/**
 * @file
 * austrofeedr_base_publishing.features.inc
 */

/**
 * Implementation of hook_views_api().
 */
function austrofeedr_base_publishing_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
