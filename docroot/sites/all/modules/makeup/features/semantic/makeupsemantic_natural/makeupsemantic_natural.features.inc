<?php
/**
 * @file
 * makeupsemantic_natural.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function makeupsemantic_natural_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "semantic_fields" && $api == "semantic_fields") {
    return array("version" => "1");
  }
}
