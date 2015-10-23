<?php
/**
 * @file
 * ooyala_channel_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function ooyala_channel_example_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function ooyala_channel_example_node_info() {
  $items = array(
    'channel' => array(
      'name' => t('Channel'),
      'base' => 'node_content',
      'description' => t('Manage an Ooyala channel.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
