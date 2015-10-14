<?php
/**
 * @file
 * Provides views data and handlers for the CPS Taxonomy module.
 */

/**
 * Implements hook_views_data().
 */
function cps_taxonomy_views_data() {
  $data['taxonomy_term_data']['cps_status'] = array(
    'title' => t('CPS published status'),
    'help' => t('Whether or not the term is published in the current CPS site version.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(t('Published'), t('Not published')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
      // Use status = 1 instead of status <> 0 in WHERE statment.
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
