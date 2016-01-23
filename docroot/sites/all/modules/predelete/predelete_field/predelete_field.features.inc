<?php
/**
 * @file
 * predelete_field.features.inc
 */

/**
 * Implementation of hook_node_info().
 */
function predelete_field_node_info() {
  $items = array(
    'predelete_example' => array(
      'name' => t('Predelete Example'),
      'base' => 'node_content',
      'description' => t('This content type has a field that determines if the node could be deleted.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
