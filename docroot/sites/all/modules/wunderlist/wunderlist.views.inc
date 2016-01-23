<?php
/**
 * @file
 * Provide views data and handlers for wunderlist.module.
 */

/**
 * Implements hook_views_data().
 */
function wunderlist_views_data() {
  $data['wunderlist_list']['table']['group']  = t('Wunderlist');

  $data['wunderlist_list']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Wunderlist list'),
    'help' => t('Wunderlist list items.'),
    'weight' => 10,
  );

  $data['wunderlist_list']['id'] = array(
    'title' => t('Wunderlist list id'),
    'help' => t('The ID of the Wunderlist list item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'base' => 'wunderlist_task',
      'base field' => 'list_id',
      'handler' => 'views_handler_relationship',
      'label' => t('Child tasks'),
      'title' => t('Child tasks.'),
    ),
  );

  $data['wunderlist_list']['title'] = array(
    'title' => t('Wunderlist list title'),
    'help' => t('The Title of the Wunderlist list item.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['owner_type'] = array(
    'title' => t('Wunderlist list owner type'),
    'help' => t('The owner type of the Wunderlist list item.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['owner_id'] = array(
    'title' => t('Wunderlist list owner id'),
    'help' => t('The owner ID of the Wunderlist list item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['list_type'] = array(
    'title' => t('Wunderlist list type'),
    'help' => t('The type of the Wunderlist list item.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['public'] = array(
    'title' => t('Wunderlist public'),
    'help' => t('The Wunderlist list status.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['revision'] = array(
    'title' => t('Wunderlist list revision ID'),
    'help' => t('The revision ID of the Wunderlist list item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['type'] = array(
    'title' => t('Wunderlist type'),
    'help' => t('The type of the Wunderlist item.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['created_at'] = array(
    'title' => t('Wunderlist creation date'),
    'help' => t('The creation date of the Wunderlist list item.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_list']['created_by_request_id'] = array(
    'title' => t('Wunderlist created by request id'),
    'help' => t('The created by request id of the Wunderlist item.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['table']['group']  = t('Wunderlist task');

  $data['wunderlist_task']['table']['base'] = array(
    'field' => 'id',
    'title' => t('Wunderlist task'),
    'help' => t('Wunderlist tasks.'),
    'weight' => 10,
  );

  $data['wunderlist_task']['id'] = array(
    'title' => t('Wunderlist task id'),
    'help' => t('The ID of the Wunderlist task.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['assignee_id'] = array(
    'title' => t('Wunderlist task assignee id'),
    'help' => t('The assignee ID of the Wunderlist task.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['created_at'] = array(
    'title' => t('Wunderlist task creation date'),
    'help' => t('The creation date of the Wunderlist task.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['created_by_id'] = array(
    'title' => t('Wunderlist task created by id'),
    'help' => t('The creator ID of the Wunderlist task.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'base' => 'wunderlist_user',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('User id'),
      'title' => t('Task author.'),
    ),
  );

  $data['wunderlist_task']['due_date'] = array(
    'title' => t('Wunderlist task due date'),
    'help' => t('The due date of the Wunderlist task.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['completed'] = array(
    'title' => t('Wunderlist task status'),
    'help' => t('The status of the Wunderlist task.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['starred'] = array(
    'title' => t('Wunderlist starred'),
    'help' => t('Wunderlist starred.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['list_id'] = array(
    'title' => t('Wunderlist task list id'),
    'help' => t('The list id the task belongs to.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'relationship' => array(
      'base' => 'wunderlist_list',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Parent list'),
      'title' => t('The list this task belongs to.'),
    ),
  );

  $data['wunderlist_task']['revision'] = array(
    'title' => t('Wunderlist task revision id'),
    'help' => t('The revision id of the task.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['title'] = array(
    'title' => t('Wunderlist task title'),
    'help' => t('The title of the task.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['type'] = array(
    'title' => t('Wunderlist task type'),
    'help' => t('The type of the task.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_task']['created_by_request_id'] = array(
    'title' => t('Wunderlist task created by request id'),
    'help' => t('The task created by request id.'),
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_user']['table']['group']  = t('Wunderlist user');

  $data['wunderlist_user']['id'] = array(
    'title' => t('Wunderlist user id'),
    'help' => t('Wunderlist user id.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['wunderlist_user']['avatar'] = array(
    'title' => t('Wunderlist user avatar'),
    'help' => t('Wunderlist user avatar.'),
    'field' => array(
      'handler' => 'WunderlistViewsHandlerFieldAvatar',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}
