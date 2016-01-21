<?php
/**
 * @file
 * Register the node_noindex views filter handlers.
 */
function node_noindex_views_data() {
  $data['node_noindex']['table']['group']  = t('Node noindex');
  $data['node_noindex']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
      'type' => 'LEFT', // Not all nodes are node_noindex.
    ),
  );

  $data['node_noindex']['noindex'] = array(
    'title' => t('No index'),
    'help' => t('Select nodes that are not indexed'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'node_noindex_filter_handler',
      'label' => t('Is indexed'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;

}

/**
 * Implements hook_views_handlers().
 */
function node_noindex_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'node_noindex'),
    ),
    'handlers' => array(
      'node_noindex_filter_handler' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}

