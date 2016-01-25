<?php

/**
 * Implements hook_views_data()
 */
function signup_share_track_exports_views_data() {
  $data = array();

  $data['signup_share_track']['table']['group'] = t('Signup Share Track');

  $data['signup_share_track']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('Signup Share Track'),
    'help' => t('Help'),
    'weight' => -10,
  );

  $data['signup_share_track']['table']['default_relationship'] = array(
    'users' => array(
      'table' => 'users',
      'field' => 'uid',
    ),
    'signup_share_track_signup' => array(
      'table' => 'signup_share_track_signup',
      'field' => 'uid_from',
    ),
  );

  $data['signup_share_track']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'signup_share_track_signup' => array(
      'left_field' => 'uid',
      'field' => 'uid_from',
    ),
  );
  
  // User ID field.
  $data['signup_share_track']['uid'] = array(
    'title' => t('User'),
    'help' => t('The name of the user.'),
    'field' => array(
      'handler' => 'signup_share_track_views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('sst Users'),
      'help' => t('Relate uids from sst table to the users table.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('users'),
    ),
  );
  // Share link field
  $data['signup_share_track']['url'] = array(
    'title' => t('Share URL'),
    'help' => t('Help'),
    'field' => array(
      'handler' => 'signup_share_track_views_handler_field_share_link',
      'click sortable' => FALSE,
    ),
  );
  // Numeric field
  $data['signup_share_track']['count'] = array(
    'title' => t('Share Count'),
    'help' => t('Help'),
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


  //signup_share_track_signup table
  $data['signup_share_track_signup']['table']['group'] = t('Signup Share Track');

  $data['signup_share_track_signup']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('Signup Share Track signup'),
    'help' => t('Help'),
    'weight' => -10,
  );

  $data['signup_share_track_signup']['table']['default_relationship'] = array(
    'users' => array(
      'table' => 'users',
      'field' => 'uid',
    ),
    'signup_share_track' => array(
      'table' => 'signup_share_track',
      'field' => 'uid',
    ),
  );


  $data['signup_share_track_signup']['uid'] = array(
    'title' => t('User'),
    'help' => t('The name of the signed up user.'),
    'field' => array(
      'handler' => 'signup_share_track_views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('ssts Users'),
      'help' => t('Relate uid from ssts table to the users table'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('users'),
    ),
  );

  $data['signup_share_track_signup']['uid_from'] = array(
    'title' => t('sst User'),
    'help' => t('The name of the user for whom we are wathcing statistics.'),
    'field' => array(
      'handler' => 'signup_share_track_views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('ssts Users'),
      'help' => t('Relate uid_from field to the sst table'),
      'handler' => 'views_handler_relationship',
      'base' => 'signup_share_track',
      'base field' => 'uid',
      'field' => 'uid_from',
      'label' => t('users'),
    ),
  );

  $data['signup_share_track_signup']['signed_up'] = array(
    'title' => t('Date signed up'),
    'help' => t('The date the user signed up.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  return $data;
}
