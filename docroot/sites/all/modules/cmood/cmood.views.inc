<?php

/**
 * @file
 * Used to expose nodes mood to views.
 */

/**
 * Implements hook_views_data().
 */
function cmood_views_data() {
  $data = array();
  $data['node_mood']['table']['group'] = t('Node mood');
  $data['node_mood']['table']['base'] = array(
    'title' => t('Node mood'),
    'help' => t('Stores nodes with respective mood value'),
  );
  $data['node_mood']['id'] = array(
    'title' => t('ID'),
    'help' => t('The record ID.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['node_mood']['mood'] = array(
    'title' => t('Mood'),
    'help' => t('The node mood.'),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['node_mood']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['node_mood']['nid'] = array(
    'title' => t('Node ID'),
    'help' => t('The node ID.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
  );
  return $data;
}
