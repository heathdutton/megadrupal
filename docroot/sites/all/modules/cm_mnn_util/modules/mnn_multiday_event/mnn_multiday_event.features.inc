<?php
/**
 * @file
 * mnn_multiday_event.features.inc
 */

/**
 * Implements hook_views_api().
 */
function mnn_multiday_event_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
