<?php

/**
 * Implementation of hook_views_handlers().
 */
function entity_soft_delete_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'entity_soft_delete') . '/includes',
    ),
    'handlers' => array(),
  );
}

function entity_soft_delete_views_data_alter(&$data) {
  $entity_types = _entity_soft_delete_entity_types();
  foreach ($entity_types as $entity_type => $entity_type_info) {
    if ($entity_type_info['integrated'] && isset($data[$entity_type_info['base table']])) {
      $data[$entity_type_info['base table']]['deleted'] = array(
        'title' => t('Deleted content'),
        'help' => t('Whether or not the content is deleted.'),
        'field' => array(
          'handler' => 'views_handler_field_boolean',
          'click sortable' => TRUE,
          'output formats' => array(
            'deleted-notdeleted' => array(t('Deleted'), t('Not deleted')),
          ),
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_boolean_operator',
          'label' => t('Deleted content'),
          'type' => 'yes-no',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      );
      $data[$entity_type_info['base table']]['deletion_uid'] = array(
        'title' => t('Delete User'),
        'help' => t('The user whom deleted/undeleted the content.'),
        'relationship' => array(
          'title' => t('Deleter'),
          'help' => t("The User ID of the entity deleter."),
          'base' => 'users',
          'base field' => 'uid',
          'handler' => 'views_handler_relationship',
          'label' => t('author'),
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
        ),
        'field' => array(
          'handler' => 'views_handler_field_user',
        ),
      );
      $data[$entity_type_info['base table']]['deletion_timestamp'] = array(
        'title' => t('Delete Time'),
        'help' => t('The time when the content has been deleted/undeleted.'),
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
    }
  }
}
