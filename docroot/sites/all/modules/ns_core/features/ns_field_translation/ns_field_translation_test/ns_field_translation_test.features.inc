<?php
/**
 * @file
 * ns_field_translation_test.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_field_translation_test_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_field_translation_test_node_info() {
  $items = array(
    'ns_field_translation_test' => array(
      'name' => t('Translation test'),
      'base' => 'node_content',
      'description' => t('Translation test module'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
