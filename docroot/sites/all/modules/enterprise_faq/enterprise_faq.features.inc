<?php
/**
 * @file
 * enterprise_faq.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function enterprise_faq_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function enterprise_faq_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function enterprise_faq_node_info() {
  $items = array(
    'enterprise_faq' => array(
      'name' => t('FAQ'),
      'base' => 'node_content',
      'description' => t('Answers to frequently asked questions'),
      'has_title' => '1',
      'title_label' => t('Question'),
      'help' => '',
    ),
  );
  return $items;
}