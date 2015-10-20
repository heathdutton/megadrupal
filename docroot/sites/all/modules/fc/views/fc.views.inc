<?php
/**
 *  @file
 * Field Complete - Provides field-based completeness for any entity - views.
 */

/**
 * Implements hook_views_data_alter().
 *
 * Link each entity to the fc table
 *
 */
function fc_views_data() {
  $data['fc']['table']['group']  = t('Field Complete');

  $data['fc']['table']['join'] = array(
    // This is filled in by hook_views_data_alter().
  );

  $data['fc']['percentage'] = array(
    'title' => t('Percentage'),
    'help' => t('The Field Completeness of the connected entity as a percentage value.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
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

/**
 * Implements hook_views_data_alter().
 *
 * Link each entity to the fc table.
 */
function fc_views_data_alter(&$data) {
  foreach (entity_get_info() as $entity_type => $entity_info) {
    $data['fc']['table']['join'][$entity_info['base table']] = array(
      'left_field' => $entity_info['entity keys']['id'],
      'field' => 'entity_id',
      'extra' => array(
        array(
          'field' => 'entity_type',
          'value' => $entity_type,
        ),
      ),
    );
  }
}
