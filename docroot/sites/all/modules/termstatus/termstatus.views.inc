<?php
/**
 * @file
 * Provide views data and handlers for taxonomy term status module.
 */

/**
 * Implements hook_views_data().
 */
function termstatus_views_data() {
  $data['termstatus']['table']['group']  = t('Taxonomy term');
  $data['termstatus']['table']['join'] = array(
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  $data['termstatus']['status'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the term is published.'),
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
