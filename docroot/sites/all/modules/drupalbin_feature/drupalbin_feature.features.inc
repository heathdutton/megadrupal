<?php
/**
 * @file
 * drupalbin_feature.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function drupalbin_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function drupalbin_feature_node_info() {
  $items = array(
    'code' => array(
      'name' => t('Code'),
      'base' => 'node_content',
      'description' => t('Source code with syntax highlighting.'),
      'has_title' => '1',
      'title_label' => t('Summary'),
      'help' => '',
    ),
  );
  return $items;
}
