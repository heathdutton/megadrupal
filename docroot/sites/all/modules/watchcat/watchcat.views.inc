<?php
/**
 * @file
 * Watchcat views integration.
 */


/**
 * Implements hook_views_data()
 */
function watchcat_views_data() {
  $data['watchcat']['table']['group']  = t('Watchcat');

  $data['watchcat']['table']['base'] = array(
    'title' => t('Watchcat'),
    'field' => 'created',
    'defaults' => array(
      'field' => 'summary',
    ),
  );

  $data['watchcat']['wid'] = array(
    'title' => t('Wid'),
    'help' => t('The Watchcat ID.'),
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

  $data['watchcat']['actor_id'] = array(
    'title' => t('Actor ID'),
    'help' => t('The activity actor ID.'),
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
      'handler' => 'views_handler_argument_actor',
    ),
  );

  $data['watchcat']['verb'] = array(
    'title' => t('Verb'),
    'help' => t('The activity verb.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
      'link_to_node default' => FALSE,
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

  $data['watchcat']['object_id'] = array(
    'title' => t('Object ID'),
    'help' => t('The activity object ID.'),
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
      'handler' => 'views_handler_argument_object',
    ),
  );

  $data['watchcat']['target_id'] = array(
    'title' => t('Target ID'),
    'help' => t('The activity target ID.'),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_target',
    ),
  );

  $data['watchcat']['title'] = array(
    'title' => t('Title'),
    'help' => t('The activity title.'),
    'field' => array(
      'handler' => 'watchcat_handler_field',
      'click sortable' => TRUE,
      'link_to_detail default' => FALSE,
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

  $data['watchcat']['summary'] = array(
    'title' => t('Summary'),
    'help' => t('The activity summary.'),
    'field' => array(
      'handler' => 'watchcat_handler_field',
      'click sortable' => TRUE,
      'link_to_detail default' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['watchcat']['uid'] = array(
    'title' => t('Author'),
    'help' => t('Relate content to the user who created it.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('author'),
    ),
  );

  $data['watchcat']['created'] = array(
    'title' => t('Timestamp'),
    'help' => t('The date and time the activity occured.'),
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

  $data['watchcat']['entity'] = array(
    'title' => t('Entity'),
    'help' => '...',
    'argument' => array(
      'handler' => 'watchcat_handler_argument_entity',
    ),
  );

  return $data;
}
