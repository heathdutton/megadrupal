<?php

/**
 * Implements hook_views_data_alter().
 */
function entity_translation_hierarchy_views_data_alter(&$data) {
  $data['entity_translation']['blocking'] = array(
    'title' => t('Blocking'),
    'help' => t('Flag to mark the translation as blocking.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'validate type' => 'boolean',
    ),
    'filter' => array(
      'handler' => 'entity_translation_hierarchy_views_handler_filter_blocking',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['entity_translation']['language']['filter']['handler'] = 'language_hierarchy_views_handler_filter_language';
}
