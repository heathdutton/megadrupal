<?php
/**
 * @file
 * at_slider.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function at_slider_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "bean_admin_ui" && $api == "bean") {
    return array("version" => "5");
  }
}
