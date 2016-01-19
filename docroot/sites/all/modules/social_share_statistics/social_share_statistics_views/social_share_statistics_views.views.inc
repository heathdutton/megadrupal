<?php

/**
 * @file
 * Views plugin settings in this file.
 */

/**
 * Implements hook_views_data().
 */
function social_share_statistics_views_views_data() {
  $data = array();

  // Create a new group by which the of this module plugins would be grouped.
  $data['social_share_statistics']['table']['group'] = t('Social Share Statistics');

  // Join the table with default node and users table.
  $data['social_share_statistics']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Define field, filter and sort type plugins for Facebook shares.
  $data['social_share_statistics']['fb'] = array(
    'title' => t('Facebook shares'),
    'help' => t('Number of Facebook shares for this node.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_group_by_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Define field, filter and sort type plugins for timestamp.
  $data['social_share_statistics']['timestamp'] = array(
    'title' => t('Last Shared On'),
    'help' => t('Time at which the node was last shared.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  // Define field, filter and sort type plugins for Tweets count.
  $data['social_share_statistics']['twitter'] = array(
    'title' => t('Twitter shares'),
    'help' => t('Number of times this node has been tweeted.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_group_by_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Define field, filter and sort type plugins for User IDs.
  $data['social_share_statistics']['uid'] = array(
    'title' => t('Users'),
    'help' => t('(Users who shared the node)'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'label' => t('Users'),
      'base' => 'users',
      'base field' => 'uid',
      'relationship field' => 'uid',
    ),
  );

  return $data;
}
