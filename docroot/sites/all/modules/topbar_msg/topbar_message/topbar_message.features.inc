<?php
/**
 * @file
 * topbar_message.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function topbar_message_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function topbar_message_node_info() {
  $items = array(
    'topbar_message' => array(
      'name' => t('Topbar Message'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Message'),
      'help' => '',
    ),
  );
  return $items;
}
