<?php
/**
 * @file
 * ns_form.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ns_form_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function ns_form_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_node_info().
 */
function ns_form_node_info() {
  $items = array(
    'ns_form' => array(
      'name' => t('Form'),
      'base' => 'node_content',
      'description' => t('A form that can be attached to an article'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
