<?php
/**
 * @file
 * example_context_reaction_isotope.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function example_context_reaction_isotope_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}
