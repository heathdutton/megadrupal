<?php

/**
 *  @file
 *  Views integration.
 */

/**
 * Implements hook_views_data()
 */
function nodequeue_scheduling_views_data() {
  $data['nodequeue_scheduling']['table']['group']  = t('Nodequeue');

  $data['nodequeue_scheduling']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
      'type' => 'LEFT',
    ),
  );

  $data['nodequeue_scheduling']['publish_on'] = array(
    'title' => t('Publish on'),
    'help' => t('The scheduled date this item will appear in the nodequeue.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['nodequeue_scheduling']['unpublish_on'] = array(
    'title' => t('Unpublish on'),
    'help' => t('The scheduled date this item will disappear from the nodequeue.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  return $data;
}
