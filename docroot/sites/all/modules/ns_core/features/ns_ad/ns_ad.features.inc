<?php
/**
 * @file
 * ns_ad.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_ad_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_ad_node_info() {
  $items = array(
    'ns_ad' => array(
      'name' => t('Advertisement'),
      'base' => 'node_content',
      'description' => t('An advertisement on the site.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
