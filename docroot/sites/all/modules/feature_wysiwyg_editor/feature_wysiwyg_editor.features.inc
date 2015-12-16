<?php
/**
 * @file
 * feature_wysiwyg_editor.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function feature_wysiwyg_editor_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
