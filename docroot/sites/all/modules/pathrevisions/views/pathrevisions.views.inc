<?php
/**
 * @file
 * Views integration for pathrevisions.
 */

/**
 * Implements hook_views_handlers().
 */
function pathrevisions_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'pathrevisions') . '/views/handlers',
    ),
    'handlers' => array(
      'pathrevisions_views_handler_field_alias' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implements hook_views_data().
 */
function pathrevisions_views_data() {
  $data['path_revisions']['table']['group'] = t('Content revision');

  $data['path_revisions']['table']['join'] = array(
    // Directly links to node_revision table.
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // alias field.
  $data['path_revisions']['alias'] = array(
    'group' => t('Content revision'),
    'title' => t('Path'),
    'help' => t('The path alias of this content revision.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'views_handler_sort'
    ),
    'filter' => array(
      'views_handler_filter_string'
    ),
  );

  // timestamp field.
  $data['path_revisions']['timestamp'] = array(
    'group' => t('Content revision'),
    'title' => t('Updated date'),
    'help' => t('The date the path alias was last updated.'),
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

/**
 * Implements hook_views_data_alter().
 */
function pathrevisions_views_data_alter(&$data) {

}
