<?php
/**
 * @file
 * ns_footer_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_footer_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_footer_content_node_info() {
  $items = array(
    'ns_footer_content' => array(
      'name' => t('Footer content'),
      'base' => 'node_content',
      'description' => t('This content will be displayed as a box with information in the footer.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
