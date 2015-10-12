<?php
/**
 * @file
 * cms_portfolio_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function cms_portfolio_content_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
