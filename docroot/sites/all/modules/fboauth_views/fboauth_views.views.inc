<?php

/**
 * @file
 * Provide views data and handlers for fboauth_views.module.
 */

/**
 * Implements hook_views_data().
 */
function fboauth_views_views_data() {
  $data = array();

  $data['fboauth_users']['table'] = array(
    'group' => t('FBOauth Users'),
    'base'  => array(
      'field'  => 'fbid',
      'title'  => t('FBOauth Users'),
      'help'   => t('Store FBOauth Users information'),
      'weight' => -10,
    ),
    'join'  => array(
      'users' => array(
        'left_field' => 'uid',
        'field'      => 'uid',
      ),
    ),
  );

  $data['fboauth_users']['fbid'] = array(
    'title'    => t('Facebook id'),
    'help'     => t('The facebook id of mapped user'),
    'field'    => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter'   => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort'     => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // The uid field.
  $data['fboauth_users']['uid'] = array(
    'title'        => t('User uid'),
    'help'         => t('The user id mapping facebook id'),
    'relationship' => array(
      'title'   => t('User'),
      'help'    => t('User mapping to facebook id.'),
      'handler' => 'views_handler_relationship',
      'base'    => 'users',
      'field'   => 'uid',
      'label'   => t('user'),
    ),
    'filter'       => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument'     => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field'        => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  return $data;
}
