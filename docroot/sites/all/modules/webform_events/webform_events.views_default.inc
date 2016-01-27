<?php
/**
 * @file
 * webform_events.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function webform_events_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'display_events';
  $view->description = 'This view displays events on your site';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Display events';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Events';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'field_webform_events_description' => 'field_webform_events_description',
    'field_webform_events_end_date' => 'field_webform_events_end_date',
    'field_webform_events_participant' => 'field_webform_events_participant',
    'view_node' => 'view_node',
  );
  $handler->display->display_options['style_options']['default'] = 'field_webform_events_end_date';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_webform_events_description' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_webform_events_end_date' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_webform_events_participant' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'view_node' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Relationship: Content: Webform submission */
  $handler->display->display_options['relationships']['webform_submission']['id'] = 'webform_submission';
  $handler->display->display_options['relationships']['webform_submission']['table'] = 'node';
  $handler->display->display_options['relationships']['webform_submission']['field'] = 'webform_submission';
  $handler->display->display_options['relationships']['webform_submission']['required'] = TRUE;
  /* Relationship: Webform submissions: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'webform_submissions';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'webform_submission';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'webform_submission';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Event name';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Description */
  $handler->display->display_options['fields']['field_webform_events_description']['id'] = 'field_webform_events_description';
  $handler->display->display_options['fields']['field_webform_events_description']['table'] = 'field_data_field_webform_events_description';
  $handler->display->display_options['fields']['field_webform_events_description']['field'] = 'field_webform_events_description';
  $handler->display->display_options['fields']['field_webform_events_description']['label'] = 'Short description';
  $handler->display->display_options['fields']['field_webform_events_description']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_description']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_description']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_description']['type'] = 'text_plain';
  $handler->display->display_options['fields']['field_webform_events_description']['field_api_classes'] = TRUE;
  /* Field: Content: Due date */
  $handler->display->display_options['fields']['field_webform_events_end_date']['id'] = 'field_webform_events_end_date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['table'] = 'field_data_field_webform_events_end_date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['field'] = 'field_webform_events_end_date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['label'] = 'Signup ends';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_end_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_webform_events_end_date']['field_api_classes'] = TRUE;
  /* Field: Content: Participants */
  $handler->display->display_options['fields']['field_webform_events_participant']['id'] = 'field_webform_events_participant';
  $handler->display->display_options['fields']['field_webform_events_participant']['table'] = 'field_data_field_webform_events_participant';
  $handler->display->display_options['fields']['field_webform_events_participant']['field'] = 'field_webform_events_participant';
  $handler->display->display_options['fields']['field_webform_events_participant']['label'] = 'Max. participants';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_participant']['type'] = 'number_unformatted';
  $handler->display->display_options['fields']['field_webform_events_participant']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 0,
  );
  $handler->display->display_options['fields']['field_webform_events_participant']['field_api_classes'] = TRUE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['text'] = 'Signup';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'webform_events' => 'webform_events',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Due date (field_webform_events_end_date) */
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['id'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['table'] = 'field_data_field_webform_events_end_date';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['field'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['operator_id'] = 'field_webform_events_end_date_value_op';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['label'] = 'Due date (field_webform_events_end_date)';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['operator'] = 'field_webform_events_end_date_value_op';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['identifier'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['default_date'] = 'now -2 day';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['year_range'] = '-0:+3';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';

  /* Display: Event listing */
  $handler = $view->new_display('page', 'Event listing', 'events_listing_page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'event_row';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['style_options']['row_class_special'] = FALSE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: View area */
  $handler->display->display_options['footer']['view']['id'] = 'view';
  $handler->display->display_options['footer']['view']['table'] = 'views';
  $handler->display->display_options['footer']['view']['field'] = 'view';
  $handler->display->display_options['footer']['view']['label'] = 'Administrative tools';
  $handler->display->display_options['footer']['view']['empty'] = TRUE;
  $handler->display->display_options['footer']['view']['view_to_insert'] = 'display_events:manage_event_block';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Webform submission */
  $handler->display->display_options['relationships']['webform_submission']['id'] = 'webform_submission';
  $handler->display->display_options['relationships']['webform_submission']['table'] = 'node';
  $handler->display->display_options['relationships']['webform_submission']['field'] = 'webform_submission';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Event image */
  $handler->display->display_options['fields']['field_webform_events_event_image']['id'] = 'field_webform_events_event_image';
  $handler->display->display_options['fields']['field_webform_events_event_image']['table'] = 'field_data_field_webform_events_event_image';
  $handler->display->display_options['fields']['field_webform_events_event_image']['field'] = 'field_webform_events_event_image';
  $handler->display->display_options['fields']['field_webform_events_event_image']['label'] = '';
  $handler->display->display_options['fields']['field_webform_events_event_image']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_event_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_event_image']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_event_image']['element_wrapper_class'] = 'view-event-image';
  $handler->display->display_options['fields']['field_webform_events_event_image']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_event_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_webform_events_event_image']['settings'] = array(
    'image_style' => 'webform_events',
    'image_link' => 'content',
  );
  $handler->display->display_options['fields']['field_webform_events_event_image']['field_api_classes'] = TRUE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'webform_submission';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'view-event-title';
  /* Field: Content: Description */
  $handler->display->display_options['fields']['field_webform_events_description']['id'] = 'field_webform_events_description';
  $handler->display->display_options['fields']['field_webform_events_description']['table'] = 'field_data_field_webform_events_description';
  $handler->display->display_options['fields']['field_webform_events_description']['field'] = 'field_webform_events_description';
  $handler->display->display_options['fields']['field_webform_events_description']['label'] = '';
  $handler->display->display_options['fields']['field_webform_events_description']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['field_webform_events_description']['alter']['max_length'] = '800';
  $handler->display->display_options['fields']['field_webform_events_description']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_description']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_description']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_description']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_description']['element_wrapper_class'] = 'view-event-description';
  $handler->display->display_options['fields']['field_webform_events_description']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_description']['type'] = 'text_trimmed';
  $handler->display->display_options['fields']['field_webform_events_description']['settings'] = array(
    'trim_length' => '800',
  );
  $handler->display->display_options['fields']['field_webform_events_description']['field_api_classes'] = TRUE;
  /* Field: Content: Link */
  $handler->display->display_options['fields']['view_node']['id'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
  $handler->display->display_options['fields']['view_node']['field'] = 'view_node';
  $handler->display->display_options['fields']['view_node']['label'] = '';
  $handler->display->display_options['fields']['view_node']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['view_node']['element_wrapper_class'] = 'view-event-signup';
  $handler->display->display_options['fields']['view_node']['text'] = 'Signup';
  /* Field: Content: Due date */
  $handler->display->display_options['fields']['field_webform_events_end_date']['id'] = 'field_webform_events_end_date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['table'] = 'field_data_field_webform_events_end_date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['field'] = 'field_webform_events_end_date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['label'] = 'Sign up before';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_wrapper_class'] = 'view-event-due-date';
  $handler->display->display_options['fields']['field_webform_events_end_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_end_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_webform_events_end_date']['field_api_classes'] = TRUE;
  /* Field: Content: Participants */
  $handler->display->display_options['fields']['field_webform_events_participant']['id'] = 'field_webform_events_participant';
  $handler->display->display_options['fields']['field_webform_events_participant']['table'] = 'field_data_field_webform_events_participant';
  $handler->display->display_options['fields']['field_webform_events_participant']['field'] = 'field_webform_events_participant';
  $handler->display->display_options['fields']['field_webform_events_participant']['label'] = 'Maximum participants for the event';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_participant']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_wrapper_class'] = 'view-event-participants';
  $handler->display->display_options['fields']['field_webform_events_participant']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_webform_events_participant']['settings'] = array(
    'thousand_separator' => '',
    'prefix_suffix' => 0,
  );
  $handler->display->display_options['fields']['field_webform_events_participant']['field_api_classes'] = TRUE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'webform_events' => 'webform_events',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Due date (field_webform_events_end_date) */
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['id'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['table'] = 'field_data_field_webform_events_end_date';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['field'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['operator_id'] = 'field_webform_events_end_date_value_op';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['label'] = 'Due date (field_webform_events_end_date)';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['operator'] = 'field_webform_events_end_date_value_op';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['identifier'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['default_date'] = 'now -2 day';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['year_range'] = '-0:+3';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'word';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Search for events';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'events';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Events';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: Search and manage */
  $handler = $view->new_display('block', 'Search and manage', 'manage_event_block');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Manage events';
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'role';
  $handler->display->display_options['access']['role'] = array(
    3 => '3',
  );
  $handler->display->display_options['defaults']['exposed_form'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'title' => 'title',
    'nothing' => 'nothing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Webform submission */
  $handler->display->display_options['relationships']['webform_submission']['id'] = 'webform_submission';
  $handler->display->display_options['relationships']['webform_submission']['table'] = 'node';
  $handler->display->display_options['relationships']['webform_submission']['field'] = 'webform_submission';
  $handler->display->display_options['relationships']['webform_submission']['required'] = TRUE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'webform_submission';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'webform_events';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_wrapper_class'] = 'view-event-manage-title';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '<a href="node/[nid]/manage-event">Manage</a>';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nothing']['element_wrapper_class'] = 'view-manage-event';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'webform_events' => 'webform_events',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Due date (field_webform_events_end_date) */
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['id'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['table'] = 'field_data_field_webform_events_end_date';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['field'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['group'] = 1;
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['operator_id'] = 'field_webform_events_end_date_value_op';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['label'] = 'Due date (field_webform_events_end_date)';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['operator'] = 'field_webform_events_end_date_value_op';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['expose']['identifier'] = 'field_webform_events_end_date_value';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['default_date'] = 'now -2 day';
  $handler->display->display_options['filters']['field_webform_events_end_date_value']['year_range'] = '-0:+3';
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'word';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Manage and search events';
  $handler->display->display_options['filters']['title']['expose']['description'] = 'Start typing the event title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $translatables['display_events'] = array(
    t('Master'),
    t('Events'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Webform Submission'),
    t('Webform Submission User'),
    t('Event name'),
    t('Short description'),
    t('Signup ends'),
    t('Max. participants'),
    t('Signup'),
    t('Name'),
    t('All'),
    t('Due date (field_webform_events_end_date)'),
    t('Title'),
    t('Event listing'),
    t('Administrative tools'),
    t('Sign up before'),
    t('Maximum participants for the event'),
    t('Search for events'),
    t('Search and manage'),
    t('Manage events'),
    t('webform_events'),
    t('<a href="node/[nid]/manage-event">Manage</a>'),
    t('Manage and search events'),
    t('Start typing the event title'),
  );
  $export['display_events'] = $view;

  return $export;
}
