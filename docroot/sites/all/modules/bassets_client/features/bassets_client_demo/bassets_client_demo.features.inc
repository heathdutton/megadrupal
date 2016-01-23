<?php
/**
 * @file
 * bassets_client_demo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function bassets_client_demo_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function bassets_client_demo_node_info() {
  $items = array(
    'bassets_demo' => array(
      'name' => t('Bassets Demo'),
      'base' => 'node_content',
      'description' => t('A ready to use content type with a file field and media selector, a text field to use wysiwyg media selector.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
