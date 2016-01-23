<?php

/**
 * @file
 * Contains integration with Views module.
 */

/**
 * Implements hook_views_data().
 */
function entity_abuse_views_data() {

  // Describe "entity_abuse_cache" table.
  $data['entity_abuse_cache']['table']['group'] = t('Entity abuse cache');

  $data['entity_abuse_cache']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'entity_id',
    ),
    'comment' => array(
      'left_field' => 'cid',
      'field' => 'entity_id',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'entity_id',
    ),
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'entity_id',
    ),
  );

  $data['entity_abuse_cache']['entity_id'] = array(
    'title' => t('Entity ID'),
    'help'   => t('ID of entity which was complained.'),
    'group'  => t('Entity abuse cache'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['entity_abuse_cache']['entity_type'] = array(
    'title'  => t('Entity type'),
    'help'   => t('Type of entity which was complained.'),
    'group'  => t('Entity abuse cache'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['entity_abuse_cache']['abuses'] = array(
    'title' => t('Amount of abuses'),
    'help'   => t('Total amount of abuses for an entity.'),
    'group'  => t('Entity abuse cache'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['entity_abuse_cache']['timestamp'] = array(
    'title' => t('Date of the latest abuse'),
    'help'   => t('Date when entity was abused last time.'),
    'group'  => t('Entity abuse cache'),
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

  // Describe "entity_abuse" table.
  $data['entity_abuse']['table']['group'] = t('Entity abuse');

  $data['entity_abuse']['table']['base'] = array(
    'field' => 'entity_abuse_id',
    'title' => t('Entity abuse'),
    'help' => t('Contains single entity abuse entries.'),
    'weight' => -10,
  );

  $data['entity_abuse']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'entity_id',
    ),
    'comment' => array(
      'left_field' => 'cid',
      'field' => 'entity_id',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'entity_id',
    ),
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'entity_id',
    ),
  );

  $data['entity_abuse']['entity_id'] = array(
    'title' => t('Entity ID'),
    'help'   => t('ID of entity which was complained.'),
    'group'  => t('Entity abuse'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['entity_abuse']['entity_type'] = array(
    'title'  => t('Entity type'),
    'help'   => t('Type of entity which was complained.'),
    'group'  => t('Entity abuse'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['entity_abuse']['uid'] = array(
    'title' => t('User'),
    'help'   => t('User ID who complained for a content.'),
    'group'  => t('Entity abuse'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
      'title' => t('User'),
      'help' => t('Used has complained to the content.'),
    ),
  );

  $data['entity_abuse']['reason'] = array(
    'title' => t('Abuse reason'),
    'help' => t('Reason of adding complain.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['entity_abuse']['timestamp'] = array(
    'title' => t('Date of abuse'),
    'help'   => t('Date when entity was complained.'),
    'group'  => t('Entity abuse'),
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
