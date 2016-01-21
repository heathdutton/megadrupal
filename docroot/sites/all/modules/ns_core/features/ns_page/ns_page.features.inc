<?php
/**
 * @file
 * ns_page.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_page_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ns_page_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function ns_page_node_info() {
  $items = array(
    'ns_page' => array(
      'name' => t('Page'),
      'base' => 'node_content',
      'description' => t('A page is a static page on the site. This content is typically used for pages that doesn\'t change often, e.g your site information or contact page.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
