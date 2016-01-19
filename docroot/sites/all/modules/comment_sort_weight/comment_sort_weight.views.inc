<?php

/**
 * Implements hook_views_data().
 */
function comment_sort_weight_views_data() {
  $data['comment_sort_weight']['table']['group']  = t('Comment');

  // For other base tables, explain how we join
  $data['comment_sort_weight']['table']['join'] = array(
    'comment' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

  $data['comment_sort_weight']['weight'] = array(
    'title' => t('Weight (hierarchical order)'),
    'help' => t('Provided by the module comment_sort_weight.'),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );

  return $data;
}
