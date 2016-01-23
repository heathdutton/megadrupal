<?php

/**
 * @file
 *
 */

/**
 * Implements hook_views_data().
 */
function node_delete_views_data() {
  $data = array();

  $data['node_delete']['table']['group'] = t('Content deleted');

  $data['node_delete']['table']['join']['node'] = array(
      'left_field' => 'nid',
      'field' => 'nid',
  );

  // Node deleted
  $data['node_delete']['node_delete_deleted'] = array(
      'title' => t('Deleted'),
      'help' => t('Whether or not the content is deleted.'),
      'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
          'output formats' => array(
              'deleted-notdeleted' => array(t('Deleted'), t('Not deleted')),
          ),
      ),
      'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
      )
  );

  return $data;
}
