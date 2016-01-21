<?php
/**
 * @file
 * View integration file.
 */

/**
 * Implements hook_views_data().
 */
function impression_views_data() {
  $data = array();

  $data['impression_base']['table'] = array(
    'group' => t('Impression'),
    'entity type' => 'impression_base',
    'base' => array(
      'field' => 'iid',
      'title' => t('Impression'),
      'help' => t('Impression tracking data'),
      'weight' => -3,
    ),
    'join' => array(
      'users' => array(
        'left_field' => 'uid',
        'field' => 'uid',
      ),
    )
  );

  $data['impression_base']['iid'] = array(
    'title' => t('IID'),
    'help' => t('The impression ID.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'iid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['impression_base']['bundle_type'] = array(
    'title' => t('Type'),
    'help' => t('The impression type.'),
    'field' => array(
      'handler' => 'views_handler_field_entity_bundle_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_entity_bundle',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_entity_bundle_type',
    ),
  );

  $data['impression_base']['ref'] = array(
    'title' => t('Referral'),
    'help' => t('The referral url'),
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

  $data['impression_base']['uri'] = array(
    'title' => t('URI'),
    'help' => t('The url used by user to access the webpage.'),
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

  $data['impression_base']['domain'] = array(
    'title' => t('Domain'),
    'help' => t('The Domain used by user to access the webpage.'),
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

  $data['impression_base']['ip'] = array(
    'title' => t('IP'),
    'help' => t('The IP of the visitor.'),
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

  $data['impression_base']['created'] = array(
    'title' => t('Created date'),
    'help' => t('The date when visit.'),
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

  $data['impression_base']['action'] = array(
    'title' => t('Action'),
    'help' => t('The user action like moving mouse or touching screen.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'action',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['impression_base']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('User id of the impression.'),
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
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User for the relationship'),
      'title' => t('Related user'),
    ),

  );

  return $data;
}
