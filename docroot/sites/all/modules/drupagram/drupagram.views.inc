<?php
/**
 * @file
 * Provide views data and handlers for drupagram.module
 */

/**
 * @defgroup views_drupagram_module drupagram.module handlers
 *
 * Includes the ability to create views of just the drupagram table.
 * @{
 */


/**
 * Implements hook_views_data().
 */
function drupagram_views_data() {
  // Basic table information.

  $data['drupagram']['table']['group']  = t('Instagram');

  // Advertise this table as a possible base table
  $data['drupagram']['table']['base'] = array(
    'field' => 'drupagram_id',
    'title' => t('Instagram posts'),
    'help' => t('Instagram posts.'),
    'weight' => 10,
  );

  // Instagram ID
  $data['drupagram']['drupagram_id'] = array(
    'title' => t('Instagram post ID'),
    'help' => t('The ID of the Instagram media.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Instagram message timestamp
  $data['drupagram']['created_time'] = array(
    'title' => t('Created time'),
    'help' => t('The time the Instagram message was posted.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Instagram text
  $data['drupagram']['caption'] = array(
    'title' => t('Caption'),
    'help' => t('The caption text of the Instagram post.'),
    'field' => array(
      'handler' => 'drupagram_views_handler_field_caption',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Instagram text
  $data['drupagram']['filter'] = array(
    'title' => t('Filter'),
    'help' => t('The filter used in this Instagram post.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

    // Instagram Link
  $data['drupagram']['link'] = array(
    'title' => t('Link'),
    'help' => t('Link to the Instagram post.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
  );

  // Instagram Likes
  $data['drupagram']['likes'] = array(
    'title' => t('Likes'),
    'help' => t('Likes data'),
    'field' => array(
      'handler' => 'drupagram_views_handler_field_likes',
    ),
  );

  // Instagram Images
  $data['drupagram']['images'] = array(
    'title' => t('Instagram image'),
    'help' => t('Instagram image.'),
    'field' => array(
      'handler' => 'drupagram_views_handler_field_images',
      'click sortable' => TRUE,
    ),
  );

    // Instagram author username
  $data['drupagram']['username'] = array(
    'title' => t('Instagram author username'),
    'help' => t('The login account of the author of this post.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['drupagram_account']['table']['group']  = t('Instagram Account');

  $data['drupagram_account']['table']['join'] = array(
    'drupagram' => array(
      'left_field' => 'username',
      'field' => 'username',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      // 'left_table' => 'drupagram_user',
    ),
  );

  // Instagram username
  $data['drupagram_account']['username'] = array(
    'title' => t('Instagram account username'),
    'help' => t('The username of the Instagram account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Instagram account full name
  $data['drupagram_account']['full_name'] = array(
    'title' => t('Full name'),
    'help' => t('The full name Instagram account user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Instagram account description
  $data['drupagram_account']['bio'] = array(
    'title' => t('Bio'),
    'help' => t('The bio of the Instagram account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Instagram account location
  $data['drupagram_account']['location'] = array(
    'title' => t('Location'),
    'help' => t('The location of the Instagram account.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_field_xss',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Instagram account description
  $data['drupagram_account']['followed_by_count'] = array(
    'title' => t('Followers'),
    'help' => t('The number of users following this Instagram account.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Instagram account profile image
  $data['drupagram_account']['profile_picture'] = array(
    'title' => t('Profile picture'),
    'help' => t('The image used by the Instagram account.'),
    'field' => array(
      'handler' => 'drupagram_views_handler_field_profile_image',
      'click sortable' => TRUE,
    ),
  );

  // Instagram account url
  $data['drupagram_account']['url'] = array(
    'title' => t('URL'),
    'help' => t('The URL given by the Instagram account user.'),
    'field' => array(
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Instagram message timestamp
  $data['drupagram_account']['last_refresh'] = array(
    'title' => t('Last refresh'),
    'help' => t('The time the Instagram account statuses were retrieved.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Instagram account description
  $data['drupagram_account']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The UID of the Instagram account.'),
  );

  $data['drupagram_account']['import'] = array(
    'title' => t('Import status'),
    'help' => t('Whether posts from this Instagram account should be imported automatically.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}


/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function drupagram_views_data_alter(&$data) {
  $data['users']['table']['join']['drupagram'] = array(
    'left_table' => 'drupagram_account',
    'left_field' => 'uid',
    'field' => 'uid',
  );
  $data['users']['table']['join']['drupagram_account'] = array(
    // 'left_table' => 'drupagram_user',
    'left_field' => 'username',
    'field' => 'username',
  );
}

/**
 * @}
 */
