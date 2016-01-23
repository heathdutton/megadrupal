<?php
/**
 * @file
 * app_example.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function app_example_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'employees';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'eck_eck_employee';
  $view->human_name = 'Employees';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Employees';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Employee: Avatar */
  $handler->display->display_options['fields']['field_employee_avatar']['id'] = 'field_employee_avatar';
  $handler->display->display_options['fields']['field_employee_avatar']['table'] = 'field_data_field_employee_avatar';
  $handler->display->display_options['fields']['field_employee_avatar']['field'] = 'field_employee_avatar';
  $handler->display->display_options['fields']['field_employee_avatar']['label'] = 'Photo';
  $handler->display->display_options['fields']['field_employee_avatar']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_employee_avatar']['settings'] = array(
    'image_style' => 'medium',
    'image_link' => '',
  );
  /* Field: Employee: Name */
  $handler->display->display_options['fields']['field_employee_name']['id'] = 'field_employee_name';
  $handler->display->display_options['fields']['field_employee_name']['table'] = 'field_data_field_employee_name';
  $handler->display->display_options['fields']['field_employee_name']['field'] = 'field_employee_name';
  /* Field: Employee: Email */
  $handler->display->display_options['fields']['field_employee_email']['id'] = 'field_employee_email';
  $handler->display->display_options['fields']['field_employee_email']['table'] = 'field_data_field_employee_email';
  $handler->display->display_options['fields']['field_employee_email']['field'] = 'field_employee_email';
  /* Field: Employee: Address */
  $handler->display->display_options['fields']['field_employee_address']['id'] = 'field_employee_address';
  $handler->display->display_options['fields']['field_employee_address']['table'] = 'field_data_field_employee_address';
  $handler->display->display_options['fields']['field_employee_address']['field'] = 'field_employee_address';
  $handler->display->display_options['fields']['field_employee_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['field_employee_address']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Employee: Birthdate */
  $handler->display->display_options['fields']['field_employee_dob']['id'] = 'field_employee_dob';
  $handler->display->display_options['fields']['field_employee_dob']['table'] = 'field_data_field_employee_dob';
  $handler->display->display_options['fields']['field_employee_dob']['field'] = 'field_employee_dob';
  $handler->display->display_options['fields']['field_employee_dob']['settings'] = array(
    'format_type' => 'medium',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Employee: Position */
  $handler->display->display_options['fields']['field_employee_position']['id'] = 'field_employee_position';
  $handler->display->display_options['fields']['field_employee_position']['table'] = 'field_data_field_employee_position';
  $handler->display->display_options['fields']['field_employee_position']['field'] = 'field_employee_position';
  /* Field: Employee: Department */
  $handler->display->display_options['fields']['field_employee_department']['id'] = 'field_employee_department';
  $handler->display->display_options['fields']['field_employee_department']['table'] = 'field_data_field_employee_department';
  $handler->display->display_options['fields']['field_employee_department']['field'] = 'field_employee_department';
  /* Filter criterion: Employee: eck_employee type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eck_eck_employee';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'employee' => 'employee',
  );
  /* Filter criterion: Employee: Name (field_employee_name) */
  $handler->display->display_options['filters']['field_employee_name_value']['id'] = 'field_employee_name_value';
  $handler->display->display_options['filters']['field_employee_name_value']['table'] = 'field_data_field_employee_name';
  $handler->display->display_options['filters']['field_employee_name_value']['field'] = 'field_employee_name_value';
  $handler->display->display_options['filters']['field_employee_name_value']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_employee_name_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_employee_name_value']['expose']['operator_id'] = 'field_employee_name_value_op';
  $handler->display->display_options['filters']['field_employee_name_value']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['field_employee_name_value']['expose']['operator'] = 'field_employee_name_value_op';
  $handler->display->display_options['filters']['field_employee_name_value']['expose']['identifier'] = 'field_employee_name_value';
  $handler->display->display_options['filters']['field_employee_name_value']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Employee: Email (field_employee_email) */
  $handler->display->display_options['filters']['field_employee_email_email']['id'] = 'field_employee_email_email';
  $handler->display->display_options['filters']['field_employee_email_email']['table'] = 'field_data_field_employee_email';
  $handler->display->display_options['filters']['field_employee_email_email']['field'] = 'field_employee_email_email';
  $handler->display->display_options['filters']['field_employee_email_email']['operator'] = 'contains';
  $handler->display->display_options['filters']['field_employee_email_email']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_employee_email_email']['expose']['operator_id'] = 'field_employee_email_email_op';
  $handler->display->display_options['filters']['field_employee_email_email']['expose']['label'] = 'Email';
  $handler->display->display_options['filters']['field_employee_email_email']['expose']['operator'] = 'field_employee_email_email_op';
  $handler->display->display_options['filters']['field_employee_email_email']['expose']['identifier'] = 'field_employee_email_email';
  $handler->display->display_options['filters']['field_employee_email_email']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'employees';
  $export['employees'] = $view;

  $view = new view();
  $view->name = 'meeting_rooms';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Meeting rooms';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Meeting rooms';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_room_size' => 'field_room_size',
    'field_room_floor' => 'field_room_floor',
    'field_room_number' => 'field_room_number',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_room_size' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_room_floor' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_room_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Number */
  $handler->display->display_options['fields']['field_room_number']['id'] = 'field_room_number';
  $handler->display->display_options['fields']['field_room_number']['table'] = 'field_data_field_room_number';
  $handler->display->display_options['fields']['field_room_number']['field'] = 'field_room_number';
  $handler->display->display_options['fields']['field_room_number']['label'] = 'Room No.';
  /* Field: Content: Capability */
  $handler->display->display_options['fields']['field_room_size']['id'] = 'field_room_size';
  $handler->display->display_options['fields']['field_room_size']['table'] = 'field_data_field_room_size';
  $handler->display->display_options['fields']['field_room_size']['field'] = 'field_room_size';
  $handler->display->display_options['fields']['field_room_size']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_room_size']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Field: Content: Floor */
  $handler->display->display_options['fields']['field_room_floor']['id'] = 'field_room_floor';
  $handler->display->display_options['fields']['field_room_floor']['table'] = 'field_data_field_room_floor';
  $handler->display->display_options['fields']['field_room_floor']['field'] = 'field_room_floor';
  $handler->display->display_options['fields']['field_room_floor']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 1,
  );
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'meeting_room' => 'meeting_room',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'meeting-rooms';
  $export['meeting_rooms'] = $view;

  return $export;
}
