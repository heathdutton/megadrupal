<?php
/**
 * @file
 * Views module integration hooks.
 */

/**
 * Implements hook_views_data().
 */
function most_viewed_views_data() {
  $data = array();
  $data['most_viewed_hits']['table']['group'] = t('MVC entities');

  $data['most_viewed_hits']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Most Viewed Content'),
    'help' => t('The primary identifier for a MVC hit.'),
    'weight' => -10,
  );

  $data['most_viewed_hits']['table']['join'] = array(
    'node' => array(
      'left_field' => 'entity_id',
      'field' => 'nid',
    ),
  );

  $data['most_viewed_hits']['id'] = array(
    'title' => t('MVC ID'),
    'help' => t('The MVC primary identifier.'),
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

  $data['most_viewed_hits']['entity_id'] = array(
    'title' => t('Entity id'),
    'help' => t('The entity identifier.'),
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

  $data['most_viewed_hits']['entity_type'] = array(
    'title' => t('Entity type'),
    'help' => t('The entity type.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['most_viewed_hits']['bundle'] = array(
    'title' => t('Bundle'),
    'help' => t('The bundle.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['most_viewed_hits']['created'] = array(
    'title' => t('Date'),
    'help' => t('The Unix timestamp when the hit was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['node']['most_viewed_sort'] = array(
    'title' => t('Most viewed sort'),
    'help' => t('Vost viewed sort'),
    'sort' => array(
      'handler' => 'most_viewed_handler_sort_most_viewed',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function most_viewed_views_handlers() {
  return array(
    'info' => array(
      'path' => MOST_VIEWED_MODULE_PATH . '/views/handlers',
    ),
    'handlers' => array(
      'most_viewed_handler_sort_most_viewed' => array(
        'parent' => 'views_handler_sort',
      ),
    ),
  );
}
