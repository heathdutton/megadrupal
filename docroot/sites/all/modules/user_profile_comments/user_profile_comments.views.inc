<?php
/**
 * @file
 * Extend views
 */

/**
 * Implements hook_views_data().
 */
function user_profile_comments_views_data() {
  $data = array();

  $data['comment']['user_comments'] = array(
    'group' => t('Comment'),
    'title' => t('User profile comments'),
    'help' => t('Comments of user profiles.'),
    'filter' => array(
      'handler' => 'user_profile_comments_filter',
    ),
  );

  $data['comment']['user_profile'] = array(
    'title' => t('User profile uid'),
    'help' => t('If you need filter comments by user profile uid.'),
    'argument' => array(
      'handler' => 'user_profile_comments_argument',
    ),
  );
  return $data;
}
