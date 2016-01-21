<?php

/**
 * @file
 * Views hooks implemented for the Mailman Integration module.
 */

/**
 * Implements hook_views_data().
 */
function mailman_integration_views_data() {
  $data = array();
  // Mailman Lists.
  $data['mailman_integration_list']['table']['group'] = t('Mailman Mailing Lists');
  $data['mailman_integration_list']['table']['base'] = array(
    'field' => 'list_id',
    'title' => t('Mailman Mail Lists'),
    'help'  => t('Mailman Mail List By Mailman Integration module'),
  );

  $data['mailman_integration_list']['table']['join'] = array(
    'users' => array(
      'left_table' => 'mailman_list_users',
      'left_field' => 'list_id',
      'field' => 'list_id',
      'type' => 'INNER',
    ),
    'mailman_list_users' => array(
      'left_field' => 'list_id',
      'field' => 'list_id',
      'type' => 'INNER',
    ),
  );

  $data['mailman_integration_list']['list_id'] = array(
    'title' => t('List ID'),
    'help'  => t('Mailman List Id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_integration_list']['list_name'] = array(
    'title' => t('Mailman List Name'),
    'help'  => t('Mailman List Name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_integration_list']['list_owners'] = array(
    'title' => t('Mailman Owners email'),
    'help'  => t('Mailman Owners email'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_integration_list']['bundle'] = array(
    'title' => t('Mailman List Type'),
    'help'  => t('Mailman List Type, manual/role'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_integration_list']['description'] = array(
    'title' => t('Mailman List Description'),
    'help'  => t('Mailman List Description'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_integration_list']['visible_to_user'] = array(
    'title' => t('User can Subscribe'),
    'help' => t('User can Subscribe this List'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['mailman_integration_list']['created'] = array(
    'title' => t('List created date time'),
    'help' => t('List created date time'),
    'field' => array(
      'handler' => 'views_handler_field_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  // Mailman Subscribers.
  $data['mailman_list_users']['table']['group'] = t('Mailman Mailing List Subscribers');
  $data['mailman_list_users']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'INNER',
    ),
    'mailman_integration_list' => array(
      'left_field' => 'list_id',
      'field' => 'list_id',
      'type' => 'INNER',
    ),
  );

  $data['mailman_list_users']['uid'] = array(
    'title' => t('Subscribers User Id'),
    'help' => t('Subscribers uid'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Drupal User Relationship'),
    ),
  );

  $data['mailman_list_users']['list_id'] = array(
    'title' => t('List Id'),
    'help'  => t('Mailman Custom table List Id'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'relationship' => array(
      'base' => 'mailman_integration_list',
      'field' => 'list_id',
      'handler' => 'views_handler_relationship',
      'label' => t('Mailman List Relationship'),
    ),
  );

  $data['mailman_list_users']['mail'] = array(
    'title' => t('Subscribers email'),
    'help'  => t('Subscribers email'),
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
  );

  $data['mailman_list_users']['created'] = array(
    'title' => t('Subscribe date time'),
    'help'  => t('Subscribe date time'),
    'field' => array(
      'handler' => 'views_handler_field_date',
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
