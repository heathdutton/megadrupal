<?php

/**
 * @file
 * Views plugin settings in this file.
 */

/**
 * Implements hook_views_data().
 */
function social_stats_views_views_data() {
  $data = array();

  // Create a new group by which the of this module plugins would be grouped.
  $data['social_stats_facebook']['table']['group'] = t('Social Stats');

  // Join the table with default node table.
  $data['social_stats_facebook']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Define field, filter and sort type plugins for Facebook likes.
  $data['social_stats_facebook']['fb_likes'] = array(
    'title' => t('Facebook likes'),
    'help' => t('Number of Facebook likes for this node.'),
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

  // Define field, filter and sort type plugins for Facebook shares.
  $data['social_stats_facebook']['fb_shares'] = array(
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

  // Define field, filter and sort type plugins for Facebook comments.
  $data['social_stats_facebook']['fb_comments'] = array(
    'title' => t('Facebook comments'),
    'help' => t('Number of Facebook comments on this node.'),
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

  // Define field, filter and sort type plugins for Facebook total.
  $data['social_stats_facebook']['fb_total'] = array(
    'title' => t('Facebook total'),
    'help' => t('Value of (Facebook likes + Facebook shares + Facebook comments).'),
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

  // Define field, filter and sort type plugins for Facebook timestamp.
  $data['social_stats_facebook']['changed'] = array(
    'title' => t('Facebook Updated Date'),
    'help' => t('Time at which the data from Facebook was updated.'),
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

  // Use the same group which was defined earlier to group plugins.
  $data['social_stats_twitter']['table']['group'] = t('Social Stats');

  // Join the table with default node table.
  $data['social_stats_twitter']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Define field, filter and sort type plugins for Tweets count.
  $data['social_stats_twitter']['tweets'] = array(
    'title' => t('Twitter shares'),
    'help' => t('Value of times this node has been tweeted.'),
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

  // Define field, filter and sort type plugins for Twitter timestamp.
  $data['social_stats_twitter']['changed'] = array(
    'title' => t('Twitter Updated Date'),
    'help' => t('Time at which the data from Twitter was updated.'),
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

  // Use the same group which was defined earlier to group plugins.
  $data['social_stats_linkedin']['table']['group'] = t('Social Stats');

  // Join the table with default node table.
  $data['social_stats_linkedin']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Define field, filter and sort type plugins for LinkedIn shares.
  $data['social_stats_linkedin']['linkedin_shares'] = array(
    'title' => t('LinkedIn shares'),
    'help' => t('Value of times this node has been shared on LinkedIn.'),
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

  // Define field, filter and sort type plugins for Linkedin timestamp.
  $data['social_stats_linkedin']['changed'] = array(
    'title' => t('Linkedin Updated Date'),
    'help' => t('Time at which the data from LinkedIn was updated.'),
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

  // Use the same group which was defined earlier to group plugins.
  $data['social_stats_gplus']['table']['group'] = t('Social Stats');

  // Join the table with default node table.
  $data['social_stats_gplus']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Define field, filter and sort type plugins for plusone total count.
  $data['social_stats_gplus']['total'] = array(
    'title' => t('Google+ Total Count'),
    'help' => t('Number of times this node has been +1ed and shared.'),
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

  // Define field, filter and sort type plugins for googleplus plusone.
  $data['social_stats_gplus']['plusone'] = array(
    'title' => t('Google+ plusone'),
    'help' => t('Number of times this node has been +1ed.'),
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
  
  // Define field, filter and sort type plugins for googleplus share count.
  $data['social_stats_gplus']['share'] = array(
    'title' => t('Google+ Share Count'),
    'help' => t('Number of times this node has been shared on Google+.'),
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

  // Define field, filter and sort type plugins for GooglePlus timestamp.
  $data['social_stats_gplus']['changed'] = array(
    'title' => t('GooglePlus Updated Date'),
    'help' => t('Time at which the data from GooglePlus was updated.'),
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

  // Use the same group which was defined earlier to group plugins.
  $data['social_stats_total']['table']['group'] = t('Social Stats');

  // Join the table with default node table.
  $data['social_stats_total']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Define field, filter and sort type plugins for total shares.
  $data['social_stats_total']['total_virality'] = array(
    'title' => t('Total social shares'),
    'help' => t('(Facebook total + Twitter Shares + LinkedIn Shares + Google plus shares)'),
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

  return $data;
}
