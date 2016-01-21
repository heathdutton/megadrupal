<?php
/**
 * @file
 * ns_fact.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_fact_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ns_fact_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_fact_node_info() {
  $items = array(
    'ns_fact' => array(
      'name' => t('Fact'),
      'base' => 'node_content',
      'description' => t('A fact is a small gray box which can be added to an articles sidebar. Usually these are used to further explain something mentioned in the article.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
