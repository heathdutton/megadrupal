<?php
/**
 * @file
 * Provide views data and handlers for unsubscribe.module
 */

/**
 * Implements hook_views_data().
 */
function unsubscribe_views_data() {
  $data['unsubscribe']['table']['group'] = t('Unsubscribe');
  $data['unsubscribe']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('Unsubscribe List'),
    'help' => t("List of users who have unsubscribed from email communication."),
    //'access query tag' => 'comment_access',
  );
  $data['unsubscribe']['uid'] = array(
    'title' => t('User uid'),
    'help' => t('If you need more fields than the uid add the comment: author relationship'),
    'relationship' => array(
      'title' => t('User'),
      'help' => t('Add this relationship to pull in data about the unsubscribed user account.'),
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('unsubscribe_user'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );
  $data['unsubscribe']['timestamp'] = array(
    'title' => t('Unsubscribe timestamp'),
    'help' => t('Date and time of when a given user unsubscribed.'),
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

  // @todo Add handlers for 'remove from list' and 'add to list' links.

  return $data;
}

