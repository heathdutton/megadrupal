<?php

/**
 * @file
 * Provide views data and handlers for field_tracker.module
 */

/**
 * Implements hook_views_handlers().
 */
function track_field_changes_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'track_field_changes') . '/views',
    ),
    'handlers' => array(
      'track_field_changes_handler_field_user_name' => array(
        'parent' => 'views_handler_field',
      ),
      'track_field_changes_handler_field_field_info' => array(
        'parent' => 'views_handler_field',
      ),
      'track_field_changes_handler_field_value_before' => array(
        'parent' => 'views_handler_field',
      ),
      'track_field_changes_handler_field_value_after' => array(
        'parent' => 'views_handler_field',
      ),
      'track_field_changes_handler_filter_field_info' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function track_field_changes_views_data() {
  $data = array();

  $data['track_field_changes_audit']['table']['group'] = t('Field Tracker');

  $data['track_field_changes_audit']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['track_field_changes_audit']['field_name'] = array(
    'title' => t('Field Name'),
    'help' => t('The unique id key for the active domain.'),
    'field' => array(
      'handler' => 'track_field_changes_handler_field_field_info',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'track_field_changes_handler_filter_field_info',
    ),
  );

  $data['track_field_changes_audit']['timestamp'] = array(
    'title' => t('Creation Date'),
    'help' => t('Display the time the field was updated by a user.'),
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

  $data['track_field_changes_audit']['before_value_text'] = array(
    'title' => t('Value Before'),
    'help' => t('The value before the update.'),
    'field' => array(
      'handler' => 'track_field_changes_handler_field_value_before',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['track_field_changes_audit']['after_value_text'] = array(
    'title' => t('Value After'),
    'help' => t('The value after the update.'),
    'field' => array(
      'handler' => 'track_field_changes_handler_field_value_after',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['track_field_changes_audit']['type'] = array(
    'title' => t('Track Changes Type'),
    'help' => t('New or Revision'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['track_field_changes_audit']['log'] = array(
    'title' => t('Track Changes Log'),
    'help' => t('The log message entered when the field was changed.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );

  $data['track_field_changes_audit']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user who updated the field.'),
    'field' => array(
      'handler' => 'track_field_changes_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_uid',
      'name field' => 'name',
    ),
  );

  return $data;
}
