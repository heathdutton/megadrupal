<?php

/**
 * @file
 * Contains views information for the mvct module.
 */

/**
 * Implements hook_views_data().
 */
function mvct_views_data() {
  $data = array();

  // Group.
  $data['mvct']['table']['group'] = t('Most Viewed per Content Type');

  $data['mvct']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Nid.
  $data['mvct']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Node'),
    ),
  );

  // view_count field.
  $data['mvct']['view_count'] = array(
    'title' => t('Number of visits'),
    'help' => t('The number of times the page has been printed.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  // last_viewed field.
  $data['mvct']['last_viewed'] = array(
    'title' => t('Last visit'),
    'help' => t('Timestamp of when the content was last viewed'),
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

  return $data;
}
