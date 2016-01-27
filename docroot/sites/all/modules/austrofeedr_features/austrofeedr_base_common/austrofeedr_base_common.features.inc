<?php
/**
 * @file
 * austrofeedr_base_common.features.inc
 */

/**
 * Implementation of hook_node_info().
 */
function austrofeedr_base_common_node_info() {
  $items = array(
    'update' => array(
      'name' => t('AustroFeedr Update'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
