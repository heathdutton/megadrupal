<?php
/**
 * @file
 * color_tagger_example.features.inc
 */

/**
 * Implements hook_node_info().
 */
function color_tagger_example_node_info() {
  $items = array(
    'picture' => array(
      'name' => t('Picture'),
      'base' => 'node_content',
      'description' => t('An image with auto tagging color taxonomy attached.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
