<?php

/**
 * Implements hook_views_data().
 *
 * Add field, filter and sort handlers for our "hotness" field.
 */
function scoville_views_data() {
  return array(
    'scoville' => array(
      'table' => array(
        'group' => t('Content statistics'),
        'join' => array(
          'node' => array(
            'left_field' => 'nid',
            'field' => 'nid',
          ),
        ),
      ),
      'hotness' => array(
        'title' => t('Hotness'),
        'help' => t('A measurement of the popularity of a node, weighted towards recently popular nodes.'),
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
      ),
    ),
  );
}
