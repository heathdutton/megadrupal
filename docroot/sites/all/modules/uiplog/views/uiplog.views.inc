<?php

/**
 * @file
 * Provides uiplog view fields and filters.
 */

/**
 * Implements hook_views_data().
 */
function uiplog_views_data() {
  $data['uiplog']['table']['group'] = t('User IP');
  $data['uiplog']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('User IP table'),
    'help' => t('Maps to uid in user table.'),
    'weight' => -10,
  );
  $data['uiplog']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  // uid field
  $data['uiplog']['uid'] = array(
    'title' => t('User id'),
    'help' => t('Unique id of user'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User uid'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  // IP field
  $data['uiplog']['ip'] = array(
    'title' => t('IP Address'),
    'help' => t('IP address of user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  // timestamp field.
  $data['uiplog']['timestamp'] = array(
    'title' => t('Login Timestamp'),
    'help' => t('Time of login.'),
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
