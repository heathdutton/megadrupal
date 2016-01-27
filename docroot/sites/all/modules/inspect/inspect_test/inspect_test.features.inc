<?php
/**
 * @file
 * inspect_test.features.inc
 */

/**
 * Implements hook_node_info().
 */
function inspect_test_node_info() {
  $items = array(
    'inspect_test' => array(
      'name' => t('inspect_test'),
      'base' => 'node_content',
      'description' => t('Content type for testing Inspect.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
