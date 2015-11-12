<?php
/**
 * @file
 * hellobar_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function hellobar_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function hellobar_content_node_info() {
  $items = array(
    'hello_message' => array(
      'name' => t('Hello Message'),
      'base' => 'node_content',
      'description' => t("Use %type to create a Hello Bar message.", array('%type' => 'Hello Message')),
      'has_title' => '1',
      'title_label' => t('Message'),
      'help' => '',
    ),
  );
  return $items;
}
