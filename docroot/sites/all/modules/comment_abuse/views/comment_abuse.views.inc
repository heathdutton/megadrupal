<?php

/**
 * @file
 * Functions for views handlers definition.
 */

/**
 * Implements hook_views_handlers().
 */
function comment_abuse_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'comment_abuse') . '/handlers',
    ),
    'handlers' => array(
      'CommentAbuseHandlerFieldUserIp' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function comment_abuse_views_data() {

  // Join.
  $data['comment_abuse']['table']['join'] = array(
    'comment' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

  $data['comment_abuse']['ip'] = array(
    'title' => t('User IP'),
    'help' => t('User IP who complain comment'),
    'group' => t('Comment abuse'),
    'field' => array(
      'handler' => 'CommentAbuseHandlerFieldUserIp',
      'base' => 'comment_abuse',
      'base field' => 'ip',
    ),
  );

  $data['comment_abuse']['comment_abuse_user_id'] = array(
    'title' => t('User ID'),
    'help' => t('User ID who complained'),
    'group' => t('Comment abuse'),
    'field' => array(
      'handler' => 'views_handler_field',
      'field' => 'uid',
    ),
  );

  $data['comment_abuse']['cid'] = array(
    'title' => t('Commend ID'),
    'help' => t('ID of comment on which complained'),
    'group' => t('Comment abuse'),
    'field' => array(
      'handler' => 'views_handler_field',
      'field' => 'cid',
    ),
    'relationship' => array(
      'base' => 'comment', // The name of the table to join with.
      'base field' => 'cid', // The name of the field on the joined table.
      // 'field' => 'nid' -- see hook_views_data_alter(); not needed here.
      'handler' => 'views_handler_relationship',
      'label' => t('Complaints on comments'),
      'title' => t('Complaints on comments'),
      'help' => t('Complaints on comments'),
    ),
  );

  $data['comment_abuse']['comment_abuse_timestamp'] = array(
    'title' => t('Comment complaint timestamp'),
    'help' => t('Comment complaint timestamp'),
    'group' => t('Comment abuse'),
    'field' => array(
      'handler' => 'views_handler_field',
      'field' => 'timestamp',
    ),
  );

  // Link to view complaints.
  $data['comment_abuse']['view_complaints'] = array(
    'title' => t('View complaints'),
    'help' => t('Provide a simple link to view complaints for comment.'),
    'group' => t('Comment abuse'),
    'field' => array(
      'title' => t('View complaints'),
      'help' => t('Provide a simple link to view complaints for comment.'),
      'handler' => 'CommentAbuseHandlerFieldViewComplaints',
    ),
  );

  // Link to remove complaints.
  $data['comment_abuse']['remove_complaints'] = array(
    'title' => t('Remove complaints'),
    'help' => t('Provide a simple link to remove all complaints of comment.'),
    'group' => t('Comment abuse'),
    'field' => array(
      'title' => t('Remove complaints'),
      'help' => t('Provide a simple link to remove all complaints of comment.'),
      'handler' => 'CommentAbuseHandlerFieldRemoveComplaints',
    ),
  );

  return $data;
}
