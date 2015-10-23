<?php
/**
 * @file
 * amazon_examples.features.inc
 */

/**
 * Implementation of hook_ctools_plugin_api().
 */
function amazon_examples_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_views_api().
 */
function amazon_examples_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => 3.0);
  }
}

/**
 * Implementation of hook_node_info().
 */
function amazon_examples_node_info() {
  $items = array(
    'amazon_example' => array(
      'name' => t('Amazon Example'),
      'base' => 'node_content',
      'description' => t('This is a simple content type that demonstrates an Amazon Field and has a related view.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
