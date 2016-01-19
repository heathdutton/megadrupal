<?php
/**
 * @file
 * migrate_csv_to_sql_example.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function migrate_csv_to_sql_example_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function migrate_csv_to_sql_example_node_info() {
  $items = array(
    'example_human' => array(
      'name' => t('Example Human'),
      'base' => 'node_content',
      'description' => t('Describes example humans.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
