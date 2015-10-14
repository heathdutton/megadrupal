<?php

/**
 * @file
 * Provide views data for term_authoring_info.module.
 */

/**
 * Implements hook_views_data().
 */
function term_authoring_info_views_data() {

  // Define the base group of this table.
  $data['term_authoring_info']['table']['group'] = t('Taxonomy term');

  // For other base tables, explain how we join.
  $data['term_authoring_info']['table']['join'] = array(
    // Directly links to taxonomy_term_data table.
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  // Created field.
  $data['term_authoring_info']['created'] = array(
    'title' => t('Post date'),
    'help' => t('The date the content was posted.'),
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

  // Changed field.
  $data['term_authoring_info']['changed'] = array(
    'title' => t('Updated date'),
    'help' => t('The date the term was last updated.'),
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

  // Uid field.
  $data['term_authoring_info']['uid'] = array(
    'title' => t('Author uid'),
    'help' => t('The user authoring the term. If you need more fields than the uid add the content: author relationship.'),
    'relationship' => array(
      'title' => t('Author'),
      'help' => t('Relate term to the user who created it.'),
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field_user',
    ),
  );

  return $data;
}
