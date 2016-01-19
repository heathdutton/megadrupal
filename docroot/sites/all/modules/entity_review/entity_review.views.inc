<?php

/**
 * Implements hook_views_data().
 */
function entity_review_views_data() {
  $data = array();

  $data['entity_review']['table']['group'] = t('Review');
  $data['entity_review']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Review'),
  );

  $data['entity_review']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
  );

  $data['entity_review']['created_field'] = array(
    'title' => t('Created date'),
    'help' => t('The date the review was created.'),
    'group' => t('Review'),
    'real field' => 'created',
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

  $data['entity_review']['status_field'] = array(
    'title' => t('Published'),
    'help' => t('Whether or not the content is published.'),
    'group' => t('Review'),
    'real field' => 'status',
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Published'),
      'type' => 'yes-no',
      'use equal' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  foreach (entity_get_info() as $entity_type => $entity_info) {
    if ($entity_type != 'review') { // Skip itself.
      $table = $entity_info['base table'];

      $data['entity_review'][$table] = array(
        'title' => t('%label review', array('%label' => $entity_info['label'])),
        'group' => t('Review'),
        'help' => t('Relate to %label.', array('%label' => $entity_info['label'])),
        'relationship' => array(
          'base' => $table,
          'base field' => $entity_info['entity keys']['id'], // The name of the field on the joined table.
          'field' => 'eid',
          'handler' => 'views_handler_relationship',
          'label' => $entity_info['label'],
          'title' => $entity_info['label'],
          'extra' => array(
            array(
              'table' => 'entity_review',
              'field' => 'type',
              'value' => $entity_type,
            )
          )
        ),
      );
    }
  }

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function entity_review_views_data_alter(&$data) {}
