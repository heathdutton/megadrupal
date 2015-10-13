<?php

/**
 * @file
 * Provide views data and handlers for taxonomy_term_statistics.module.
 */

/**
 * Implements hook_views_data().
 */
function taxonomy_term_statistics_views_data() {
  // Basic table information.

  // ----------------------------------------------------------------
  // taxonomy_term_counter table

  $data['taxonomy_term_counter']['table']['group']  = t('Taxonomy term statistics');

  $data['taxonomy_term_counter']['table']['join'] = array(
    // ...to the taxonomy_term_data table
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  // totalcount
  $data['taxonomy_term_counter']['totalcount'] = array(
    'title' => t('Total views'),
    'help' => t('The total number of times the taxonomy term has been viewed.'),

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
  );

  // daycount
  $data['taxonomy_term_counter']['daycount'] = array(
    'title' => t('Views today'),
    'help' => t('The total number of times the taxonomy term has been viewed today.'),

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
  );

  // timestamp
  $data['taxonomy_term_counter']['timestamp'] = array(
    'title' => t('Most recent view'),
    'help' => t('The most recent time the taxonomy_term has been viewed.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
