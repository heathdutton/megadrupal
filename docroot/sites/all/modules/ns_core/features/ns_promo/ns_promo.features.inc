<?php
/**
 * @file
 * ns_promo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_promo_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "linkit" && $api == "linkit_profiles") {
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
function ns_promo_node_info() {
  $items = array(
    'ns_promo' => array(
      'name' => t('Promotion'),
      'base' => 'node_content',
      'description' => t('A promotion is an image or a text that promotes content on your site. Promotions are flexible and can be placed in various places on your site.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
