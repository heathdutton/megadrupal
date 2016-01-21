<?php

/**
 * @file
 * Provide views data and handlers for the comment score field.
 */

/**
 * Implements hook_views_data_alter().
 */
function comment_score_views_data_alter(&$data) {
  $data['comment']['score'] = array(
    'title' => t('Comment score'),
    'help' => t('The score of a comment is how likely the comment is to be negative of spammy.'),
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
}
