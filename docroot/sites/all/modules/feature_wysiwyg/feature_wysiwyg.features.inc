<?php
/**
 * @file
 * feature_wysiwyg.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function feature_wysiwyg_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
