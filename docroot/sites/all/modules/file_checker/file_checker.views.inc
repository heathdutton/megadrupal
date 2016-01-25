<?php
/**
 * @file file_checker.views.inc
 * Views API Implementations
 */

/**
 * Implements hook_views_handlers().
 *
 * Register custom handlers for file status.
 * @return
 *   array of views handlers
 */
function file_checker_views_handlers() {
  return array(
    'handlers' => array(
      // register our custom filter, with the class/file name and parent class
      'file_checker_field_file_status' => array(
        'parent' => 'views_handler_field_file_status',
      ),
      'file_checker_filter_file_status' => array(
        'parent' => 'views_handler_filter_file_status',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_filter().
 *
 * Override file status handlers for filter and field by custom ones.
 *
 * @param $data
 *   array of views data
 */
function file_checker_views_data_alter(&$data) {
  $data['files']['status'] = array(
    'title' => t('Status'),
    'help' => t('The status of the file.'),
    'field' => array(
      'handler' => 'file_checker_field_file_status',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'file_checker_filter_file_status',
    ),
  );
}
