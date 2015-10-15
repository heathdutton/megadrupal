<?php
/**
 * @file
 * more_boxes_feature.features.inc
 */

/**
 * Implements hook_views_api().
 */
function more_boxes_feature_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
