<?php
/**
 * @file
 * wechat_mass.features.inc
 */

/**
 * Implements hook_node_info().
 */
function wechat_mass_node_info() {
  $items = array(
    'wechat_mass_message' => array(
      'name' => t('Wechat mass message'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
