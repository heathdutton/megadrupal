<?php
/**
 * @file
 * mm_views.features.inc
 */

/**
 * Implements hook_node_info().
 */
function mm_views_node_info() {
  $items = array(
    'view_node' => array(
      'name' => t('View Node'),
      'base' => 'node_content',
      'description' => t('A node that just contains a view, for display on this page in the MM tree.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
