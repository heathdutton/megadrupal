<?php

/**
 * @file
 * Views file.
 */

/**
 * Implements hook_views_data().
 */
function link_click_count_views_data() {
  $data = array();
  $data['link_click_count']['table']['group'] = t('Link Click Count');
  $data['link_click_count']['table']['base'] = array(
    'field' => 'nid',
    'title' => t('Link count'),
    'help' => t('This table is containing the data about count view of node.'),
    'weight' => -10,
  );
  $data['link_click_count']['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join',
      'left_table' => 'node',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['link_click_count']['nid'] = array(
    'title' => t('Link Node id'),
    'help' => t('Link Node id.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data['link_click_count']['url'] = array(
    'title' => t('Link url'),
    'help' => t('url.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_field',
    ),
    'argument' => array(
      'handler' => 'views_handler_field',
    ),
  );
  $data['link_click_count']['date'] = array(
    'title' => t('Link click date'),
    'help' => t('When user view of date.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
        'empty field name' => t('Undated'),
      ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );
  return $data;
}
