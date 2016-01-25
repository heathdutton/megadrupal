<?php

/**
 * @file
 *   Integrates the Tweet module with Views.
 */

/**
 * Implements hook_views_data().
 */
function tweet_views_data() {
  $data = array();
  $data['node']['tweet'] = array(
    'group' => t('Tweet'),
    'title' => t('Tweet button'),
    'help' => t('Display the Tweet button for the relevant node.'),
    'field' => array(
      'field' => 'nid',
      'handler' => 'tweet_views_handler_field_button',
      'click sortable' => FALSE,
    ),
  );
  return $data;
}
