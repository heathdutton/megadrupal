<?php
/**
 * @file
 * voipnode.features.inc
 */

/**
 * Implements hook_node_info().
 */
function voipnode_node_info() {
  $items = array(
    'voipnode' => array(
      'name' => t('VoIP Node'),
      'base' => 'node_content',
      'description' => t('A node with extension, audio field and script already enabled.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
