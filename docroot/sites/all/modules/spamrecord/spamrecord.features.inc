<?php
/**
 * @file
 * spamrecord.features.inc
 */

/**
 * Implements hook_node_info().
 */
function spamrecord_node_info() {
  $items = array(
    'spam_message' => array(
      'name' => t('Spam message'),
      'base' => 'node_content',
      'description' => t('Record a spam call, text, or fax that you\'ve received.'),
      'has_title' => '1',
      'title_label' => t('Phone number'),
      'help' => '',
    ),
  );
  return $items;
}
