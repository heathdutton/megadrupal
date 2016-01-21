<?php
/**
 * @file
 *
 * Integration with Views using Taxonomy Nested Intervals.
 *
 * Add extra views capabilities for Taxonomy Nested Intervals
 */

/**
 * Implements hook_views_data().
 */
function views_taxonomy_nested_intervals_views_data() {
  $data = array();

  // Taxonomy Nested Intervals
  $data['taxonomy_nested_intervals']['table']['group']  = t('Taxonomy Nested Intervals');

  // Joins for this table
  $data['taxonomy_nested_intervals']['table']['join'] = array(
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
      'type' => 'INNER',
      'handler' => 'views_join_nested_intervals',
    ),
    'taxonomy_vocabulary' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'INNER',
    ),
  );

  // Columns
  $data['taxonomy_nested_intervals']['id'] = array(
    'title' => t('Interval ID'), // The item it appears as on the UI,
    'help' => t('The taxonomy nested interval ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'parent',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['taxonomy_nested_intervals']['vid'] = array(
    'title' => t('Vocabulary ID'), // The item it appears as on the UI,
    'help' => t('The taxonomy vocabulary ID'),
  );

  $data['taxonomy_nested_intervals']['tid'] = array(
    'title' => t('Term ID'), // The item it appears as on the UI,
    'help' => t('The taxonomy term ID'),
    'relationship' => array(
      'base' => 'taxonomy_term_data',
      'field' => 'tid',
      'label' => t('Term ID'),
    ),
  );

  $data['taxonomy_nested_intervals']['parent'] = array(
    'title' => t('Parent term ID'), // The item it appears as on the UI,
    'help' => t('The parent taxonomy term ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_nested_intervals_parent',
      'name field' => 'parent',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_nested_intervals_parent',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['taxonomy_nested_intervals']['distance'] = array(
    'title' => t('Distance to root'), // The item it appears as on the UI,
    'help' => t('The taxonomy term\'s distance to the root'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'distance',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
