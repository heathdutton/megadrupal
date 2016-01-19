<?php
/**
 * @file
 * Provides function for integration with Rules.
 */

/**
 * Implements hook_rules_event_info().
 */
function comment_abuse_rules_event_info() {
  $items = array(
    'complaint_insert' => array(
      'label' => t('After reporting new complaint on comment'),
      'group' => t('comment'),
      'variables' => array('comment' => array('type' => 'comment', 'label' => 'Complaint')),
    ),
  );
  return $items;
}

