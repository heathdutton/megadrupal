<?php

/**
 * Implements hook_views_data_alter()
 */
function views_term_hierarchy_weight_field_views_data_alter(&$data) {
  // taxonomy hierarchical weight
  $data['taxonomy_term_data']['hweight'] = array(
    'title' => t('Hierarchical weight'),
    'help' => t('The term hierarchical weight field'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // taxonomy hierarchical level
  $data['taxonomy_term_data']['hdepth'] = array(
    'title' => t('Hierarchical depth'),
    'help' => t('The term hierarchical depth field'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
}
