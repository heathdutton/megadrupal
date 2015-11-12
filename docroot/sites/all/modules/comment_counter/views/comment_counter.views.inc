<?php

/**
 * @file
 * Statistics Counter: Views support
 */

/**
 * Implements hook_views_data_alter().
 */
function comment_counter_views_data_alter(&$data) {
  if (!isset($data['node_comment_statistics'])) {
    return;
  }

  // Day counts.
  $data['node_comment_statistics']['comment_day_count'] = array(
    'title' => t('Comments today'),
    'help'  => t('The total number of comments on this node today.'),
    'field' => array(
      'handler'        => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  // Week counts.
  $data['node_comment_statistics']['comment_week_count'] = array(
    'title' => t('Comments this week'),
    'help'  => t('The total number of comments on this node this week.'),
    'field' => array(
      'handler'        => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  // Month counts.
  $data['node_comment_statistics']['comment_month_count'] = array(
    'title' => t('Comments this month'),
    'help'  => t('The total number of comments on this node this month.'),
    'field' => array(
      'handler'        => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );

  // Week counts.
  $data['node_comment_statistics']['comment_year_count'] = array(
    'title' => t('Comments this year'),
    'help'  => t('The total number of comments on this node this year.'),
    'field' => array(
      'handler'        => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
  );
}
