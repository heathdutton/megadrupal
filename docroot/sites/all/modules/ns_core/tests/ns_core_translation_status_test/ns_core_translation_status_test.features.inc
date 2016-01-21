<?php
/**
 * @file
 * ns_core_translation_status_test.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_core_translation_status_test_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}
