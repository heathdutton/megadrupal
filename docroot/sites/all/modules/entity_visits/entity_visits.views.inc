<?php
/**
 * @file
 * Provide views data and handlers.
 */

/**
 * Implements hook_views_data().
 */
function entity_visits_views_data() {
  $data = array();

  $data['entity_visits']['table']['group'] = t('Entity Visits');

  $data['entity_visits']['table']['base'] = array(
    'title' => t('Entity Visits'),
  );

  $data['entity_visits']['eid'] = array(
    'title' => t('Viewed Entity'),
    'help' => t('The ID of the Entity that has been viewed.'),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  foreach (entity_get_info() as $info) {
    $key_field = $info['entity keys']['id'];

    $data['entity_visits']['eid_' . $key_field] = array(
      'real field' => 'eid',
      'title' => $info['label'],
      'help' => t('The ID of the Entity that has been viewed.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => $info['base table'],
        'base field' => $key_field,
        'field' => 'eid',
        'label' => $info['label'],
      ),
    );
  }

  $data['entity_visits']['uid'] = array(
    'title' => t('Visitor of Entity'),
    'help' => t('The ID of the user who viewed the entity.'),
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
      'name field' => 'name',
    ),
    'filter' => array(
      'title' => t('Name'),
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['entity_visits']['date'] = array(
    'title' => t('Date'),
    'help' => t('Date of last visit.'),
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

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function entity_visits_views_data_alter(&$data) {
  $data['users']['entity_visitor'] = array(
    'real field' => 'uid',
    'title' => t('Visitor of Entity'),
    'help' => t('The ID of the user who viewed the entity.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'entity_visits',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Entity Visits'),
    ),
  );

  foreach (entity_get_info() as $info) {
    $base_table = $info['base table'];
    $key_field = $info['entity keys']['id'];

    $data[$base_table]['entity_visits_' . $key_field] = array(
      'real field' => $key_field,
      'title' => t('Viewed Entity'),
      'help' => t('The ID of the Entity that has been viewed.'),
      'relationship' => array(
        'handler' => 'views_handler_relationship',
        'base' => 'entity_visits',
        'base field' => 'eid',
        'field' => $key_field,
        'label' => t('Entity Visits'),
      ),
    );
  }
}
