<?php
/**
 * @file
 * advanced_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function advanced_page_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function advanced_page_node_info() {
  $items = array(
    'advanced_page' => array(
      'name' => t('Advanced Page'),
      'base' => 'node_content',
      'description' => t('Use <em>advanced page</em> for your static content, such as an \'About us\' page. Static page provides fields to upload your own css and javascript files.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
