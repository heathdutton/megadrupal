<?php

/**
 * @file
 * reservations ui default views
 */


/**
 * Implements hook_views_default_views().
 */
function reservations_ui_views_default_views() {

  $view = new view;
  $view->name = 'reservations_ui_my_res';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: My Reservations';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My Reservations';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Reservation */
  $handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['label'] = 'Reservation Dates';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['path'] = 'node/[nid] ';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['type'] = 'date_short';
  $handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_reservations_date']['field_api_classes'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['item_nid2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_alter_empty'] = 0;
  /* Field: Reservations: Reservation status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['hide_alter_empty'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid']['not'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/reservations';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'My Reservations';
  $handler->display->display_options['menu']['weight'] = '0';


  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'reservations_ui_list_res';
  $view->description = 'List Upcoming, Incoming, Unconfirmed, and Overdue Reservations';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: Upcoming Reservations';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'field_reservations_date_value' => 'field_reservations_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'field_reservations_date_value';
  $handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'field_reservations_date_value' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'edit_node' => array(
    'separator' => '',
  ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = '<div class="messages status">There are no Upcoming Reservations</div>';
  $handler->display->display_options['empty']['text']['format'] = 'full_html';
  $handler->display->display_options['empty']['text']['tokenize'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Reservation */
  $handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['label'] = 'Reservation Dates';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_reservations_date']['field_api_classes'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Reservations: Reservation status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  /* Sort criterion: Content: Reservation -  start date (field_reservations_date) */
  $handler->display->display_options['sorts']['field_reservations_date_value']['id'] = 'field_reservations_date_value';
  $handler->display->display_options['sorts']['field_reservations_date_value']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['sorts']['field_reservations_date_value']['field'] = 'field_reservations_date_value';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Reservation -  start date (field_reservations_date) */
  $handler->display->display_options['filters']['field_reservations_date_value']['id'] = 'field_reservations_date_value';
  $handler->display->display_options['filters']['field_reservations_date_value']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['filters']['field_reservations_date_value']['field'] = 'field_reservations_date_value';
  $handler->display->display_options['filters']['field_reservations_date_value']['operator'] = '>=';
  $handler->display->display_options['filters']['field_reservations_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_reservations_date_value']['expose']['operator_id'] = 'field_reservations_date_value_op';
  $handler->display->display_options['filters']['field_reservations_date_value']['expose']['label'] = 'Show Reservations Starting After';
  $handler->display->display_options['filters']['field_reservations_date_value']['expose']['operator'] = 'field_reservations_date_value_op';
  $handler->display->display_options['filters']['field_reservations_date_value']['expose']['identifier'] = 'field_reservations_date_value';
  $handler->display->display_options['filters']['field_reservations_date_value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_reservations_date_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_reservations_date_value']['default_date'] = '12AM today';

  /* Display: Upcoming */
  $handler = $view->new_display('page', 'Upcoming', 'page_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/list/upcoming';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Upcoming';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'List Reservations';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $handler->display->display_options['tab_options']['description'] = 'List Upcoming, Incoming, Unconfirmed, and Overdue Reservations';

  /* Display: Upcoming Block */
  $handler = $view->new_display('block', 'Upcoming Block', 'block_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_reservations_date_value2']['id'] = 'field_reservations_date_value2';
  $handler->display->display_options['fields']['field_reservations_date_value2']['table'] = 'node_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date_value2']['field'] = 'field_reservations_date_value2';
  $handler->display->display_options['fields']['field_reservations_date_value2']['label'] = '';
  $handler->display->display_options['fields']['field_reservations_date_value2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['empty_zero'] = 0;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_reservations_date_value']['id'] = 'field_reservations_date_value';
  $handler->display->display_options['fields']['field_reservations_date_value']['table'] = 'node_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date_value']['field'] = 'field_reservations_date_value';
  $handler->display->display_options['fields']['field_reservations_date_value']['label'] = '';
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['text'] = '[name] reserved [item_nid2] starting at [field_reservations_date_value]
    ';
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['empty_zero'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Reservation Starts After:';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = '-1 day';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'node_data_field_reservations_date.field_reservations_date_value' => 'node_data_field_reservations_date.field_reservations_date_value',
  );
  /* Filter criterion: Reservations: Reservation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
  1 => '1',
  2 => '2',
  );
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: No Show Warning */
  $handler = $view->new_display('block', 'No Show Warning', 'block_2');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'message warning';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = FALSE;
  $handler->display->display_options['header']['text']['content'] = '<b>NO SHOWS:</b><br />';
  $handler->display->display_options['header']['text']['format'] = 'full_html';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_reservations_date_value2']['id'] = 'field_reservations_date_value2';
  $handler->display->display_options['fields']['field_reservations_date_value2']['table'] = 'node_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date_value2']['field'] = 'field_reservations_date_value2';
  $handler->display->display_options['fields']['field_reservations_date_value2']['label'] = '';
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['text'] = '[name] never picked up the [item_nid2] reserved on [field_reservations_date_value2] - [edit_node] ';
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['empty_zero'] = 0;
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_reservations_date_value']['id'] = 'field_reservations_date_value';
  $handler->display->display_options['fields']['field_reservations_date_value']['table'] = 'node_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date_value']['field'] = 'field_reservations_date_value';
  $handler->display->display_options['fields']['field_reservations_date_value']['label'] = 'Reservation';
  $handler->display->display_options['fields']['field_reservations_date_value']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['text'] = '[field_reservations_date_value]<br />
    [field_reservations_date_value2]';
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value']['empty_zero'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '<';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Reservation Starts After:';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['granularity'] = 'minute';
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'node_data_field_reservations_date.field_reservations_date_value' => 'node_data_field_reservations_date.field_reservations_date_value',
  );
  /* Filter criterion: Reservations: Reservation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
  1 => '1',
  2 => '2',
  );
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;


  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'reservations_ui_incoming_res';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: Incoming Reservations';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'name' => 'name',
  'group_nid' => 'group_nid',
  'field_reservations_date_value' => 'field_reservations_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'field_reservations_date_value';
  $handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'group_nid' => array(
    'separator' => '',
  ),
  'field_reservations_date_value' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'edit_node' => array(
    'separator' => '',
  ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = '<div class="messages status">Anticipating no returns.</div>';
  $handler->display->display_options['empty']['text']['format'] = 'full_html';
  $handler->display->display_options['empty']['text']['tokenize'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Reservation Dates */
  $handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_reservations_date']['field_api_classes'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Reservations: Reservation status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Reservation Starts After:';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = '12AM today';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'node_data_field_reservations_date.field_reservations_date_value2' => 'node_data_field_reservations_date.field_reservations_date_value2',
  );
  /* Filter criterion: Reservations: Reservation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
  3 => '3',
  );
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Incoming */
  $handler = $view->new_display('page', 'Incoming', 'page_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/list/incoming';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Incoming';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Reservations';
  $handler->display->display_options['tab_options']['weight'] = '0';


  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'reservations_ui_overdue_res';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: Overdue Reservations';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = '<div class="messages status">There are currently no overdue reservations.</div>';
  $handler->display->display_options['empty']['text']['format'] = 'full_html';
  $handler->display->display_options['empty']['text']['tokenize'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Reservation */
  $handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_reservations_date']['field_api_classes'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Reservations: Reservation status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '<';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Reservation Ended:';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['granularity'] = 'minute';
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'node_data_field_reservations_date.field_reservations_date_value2' => 'node_data_field_reservations_date.field_reservations_date_value2',
  );
  /* Filter criterion: Reservations: Reservation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
  3 => '3',
  );
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Overdue */
  $handler = $view->new_display('page', 'Overdue', 'page_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'group_nid' => 'group_nid',
  'field_reservations_date_value2' => 'field_reservations_date_value2',
  'field_reservations_date_value' => 'field_reservations_date_value',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = 'field_reservations_date_value';
  $handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'group_nid' => array(
    'separator' => '',
  ),
  'field_reservations_date_value2' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_reservations_date_value' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'item_nid2' => array(
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'edit_node' => array(
    'separator' => '',
  ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/list/overdue';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Overdue';
  $handler->display->display_options['menu']['weight'] = '4';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Upcoming Reservations';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Overdue Warning */
  $handler = $view->new_display('block', 'Overdue Warning', 'block_1');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'message warning';
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['text']['id'] = 'area';
  $handler->display->display_options['header']['text']['table'] = 'views';
  $handler->display->display_options['header']['text']['field'] = 'area';
  $handler->display->display_options['header']['text']['empty'] = FALSE;
  $handler->display->display_options['header']['text']['content'] = '<b>OVERDUE ITEMS:</b><br />';
  $handler->display->display_options['header']['text']['format'] = 'full_html';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['exclude'] = TRUE;
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['exclude'] = TRUE;
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
  /* Field: Broken/missing handler */
  $handler->display->display_options['fields']['field_reservations_date_value2']['id'] = 'field_reservations_date_value2';
  $handler->display->display_options['fields']['field_reservations_date_value2']['table'] = 'node_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date_value2']['field'] = 'field_reservations_date_value2';
  $handler->display->display_options['fields']['field_reservations_date_value2']['label'] = '';
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['text'] = '[name] didn\'t return [item_nid2] that were due [field_reservations_date_value2] - [edit_node]';
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date_value2']['empty_zero'] = 0;

  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'reservations_ui_unconfirmed_res';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: Unconfirmed Reservations';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'item_nid2' => 'item_nid2',
  'status' => 'status',
  'edit_node' => 'edit_node',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'item_nid2' => array(
    'align' => '',
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'edit_node' => array(
    'align' => '',
    'separator' => '',
  ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = '<div class="messages status">There are currently no unconfirmed reservations.</div>';
  $handler->display->display_options['empty']['text']['format'] = 'full_html';
  $handler->display->display_options['empty']['text']['tokenize'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Reservation */
  $handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
  'format_type' => 'short',
  'fromto' => 'both',
  'multiple_number' => '',
  'multiple_from' => '',
  'multiple_to' => '',
  'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_reservations_date']['field_api_classes'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Reservations: Reservation status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = '';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  /* Sort criterion: Content: Reservation -  start date (field_reservations_date) */
  $handler->display->display_options['sorts']['field_reservations_date_value']['id'] = 'field_reservations_date_value';
  $handler->display->display_options['sorts']['field_reservations_date_value']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['sorts']['field_reservations_date_value']['field'] = 'field_reservations_date_value';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
  'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>=';
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Reservation Starts After:';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = '12AM today';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
  'field_data_field_reservations_date.field_reservations_date_value' => 'field_data_field_reservations_date.field_reservations_date_value',
  );
  /* Filter criterion: Reservations: Reservation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
  1 => '1',
  );
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Unconfirmed */
  $handler = $view->new_display('page', 'Unconfirmed', 'page_1');
  $handler->display->display_options['defaults']['use_ajax'] = FALSE;
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/list/unconfirmed';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Unconfirmed';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Upcoming Reservations';
  $handler->display->display_options['tab_options']['weight'] = '0';

  $view = new view;
  $view->name = 'reservations_ui_node_res';
  $view->description = 'View is called by the reservations_ui module in node/%/manage-reservations';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: Node Reservations';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Item Reservations';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'manage reservations';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
	'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
	'title' => array(
	  'sortable' => 1,
	  'separator' => '',
	),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = '<div class="messages status">There are no reservations for this item.</div>';
  $handler->display->display_options['empty']['text']['format'] = 'full_html';
  $handler->display->display_options['empty']['text']['tokenize'] = 0;
  /* Relationship: Reservations: Reserved item nid */
  $handler->display->display_options['relationships']['item_nid']['id'] = 'item_nid';
  $handler->display->display_options['relationships']['item_nid']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['relationships']['item_nid']['field'] = 'item_nid';
  $handler->display->display_options['relationships']['item_nid']['required'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Reservation Dates */
  $handler->display->display_options['fields']['field_reservations_date']['id'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['field_reservations_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_reservations_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_reservations_date']['settings'] = array(
	'format_type' => 'short',
	'fromto' => 'both',
	'multiple_number' => '',
	'multiple_from' => '',
	'multiple_to' => '',
	'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_reservations_date']['field_api_classes'] = 0;
  /* Field: Reservations: Reserved items */
  $handler->display->display_options['fields']['item_nid2']['id'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['table'] = 'reservations_reservation_detail';
  $handler->display->display_options['fields']['item_nid2']['field'] = 'item_nid2';
  $handler->display->display_options['fields']['item_nid2']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_nid2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_nid2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_nid2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_nid2']['empty_zero'] = 0;
  /* Field: Reservations: Reservation status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Operations';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['external'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['edit_node']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_alter_empty'] = 0;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'item_nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['nid']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['nid']['title_enable'] = 1;
  $handler->display->display_options['arguments']['nid']['title'] = 'Reservations for %1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = 0;
  $handler->display->display_options['arguments']['nid']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
	'reservations_reservation' => 'reservations_reservation',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  /* Filter criterion: Date: Date (node) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'node';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['operator'] = '>';
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Show Reservations Starting After';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = '12AM today';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
	'field_data_field_reservations_date.field_reservations_date_value' => 'field_data_field_reservations_date.field_reservations_date_value',
  );


  $views[$view->name] = $view;
  
  $view = new view;
  $view->name = 'reservation_ui_res_items';
  $view->description = 'Reservations: Reservable Items';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = 'Reservations UI: Reservable Items';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['items_per_page'] = 0;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Relationship: Reservations: Reservation nid (for item nid) */
  $handler->display->display_options['relationships']['vid']['id'] = 'vid';
  $handler->display->display_options['relationships']['vid']['table'] = 'reservations_reservation_detail3';
  $handler->display->display_options['relationships']['vid']['field'] = 'vid';
  $handler->display->display_options['relationships']['vid']['label'] = 'Reservation for placeholder item node id';
  $handler->display->display_options['relationships']['vid']['required'] = 1;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/Available';
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Reservations: Allow overnight */
  $handler->display->display_options['fields']['allow_overnight']['id'] = 'allow_overnight';
  $handler->display->display_options['fields']['allow_overnight']['table'] = 'reservations_node_type';
  $handler->display->display_options['fields']['allow_overnight']['field'] = 'allow_overnight';
  $handler->display->display_options['fields']['allow_overnight']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['allow_overnight']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['allow_overnight']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['alter']['html'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['hide_empty'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['empty_zero'] = 0;
  $handler->display->display_options['fields']['allow_overnight']['not'] = 0;
  /* Field: Reservations: Allow weekends */
  $handler->display->display_options['fields']['allow_weekends']['id'] = 'allow_weekends';
  $handler->display->display_options['fields']['allow_weekends']['table'] = 'reservations_node_type';
  $handler->display->display_options['fields']['allow_weekends']['field'] = 'allow_weekends';
  $handler->display->display_options['fields']['allow_weekends']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['allow_weekends']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['allow_weekends']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['alter']['html'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['hide_empty'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['empty_zero'] = 0;
  $handler->display->display_options['fields']['allow_weekends']['not'] = 0;
  /* Field: Reservations: Bucket spare items */
  $handler->display->display_options['fields']['spare_items']['id'] = 'spare_items';
  $handler->display->display_options['fields']['spare_items']['table'] = 'reservations_node_type';
  $handler->display->display_options['fields']['spare_items']['field'] = 'spare_items';
  $handler->display->display_options['fields']['spare_items']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['spare_items']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['spare_items']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['spare_items']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['spare_items']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['spare_items']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['spare_items']['alter']['html'] = 0;
  $handler->display->display_options['fields']['spare_items']['hide_empty'] = 0;
  $handler->display->display_options['fields']['spare_items']['empty_zero'] = 0;
  /* Field: Reservations: Bucket/Resource Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'reservations_node_type';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Reservations: Max hours per reservation */
  $handler->display->display_options['fields']['max_hours_per_reservation']['id'] = 'max_hours_per_reservation';
  $handler->display->display_options['fields']['max_hours_per_reservation']['table'] = 'reservations_node_type';
  $handler->display->display_options['fields']['max_hours_per_reservation']['field'] = 'max_hours_per_reservation';
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['max_hours_per_reservation']['alter']['html'] = 0;
  $handler->display->display_options['fields']['max_hours_per_reservation']['hide_empty'] = 0;
  $handler->display->display_options['fields']['max_hours_per_reservation']['empty_zero'] = 0;
  /* Contextual filter: Content: Type */
  $handler->display->display_options['arguments']['type']['id'] = 'type';
  $handler->display->display_options['arguments']['type']['table'] = 'node';
  $handler->display->display_options['arguments']['type']['field'] = 'type';
  $handler->display->display_options['arguments']['type']['default_action'] = 'default';
  $handler->display->display_options['arguments']['type']['exception']['title_enable'] = 1;
  $handler->display->display_options['arguments']['type']['title_enable'] = 1;
  $handler->display->display_options['arguments']['type']['title'] = '%1 Detail';
  $handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['type']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['type']['specify_validation'] = 1;
  /* Filter criterion: Reservations: Reservations type */
  $handler->display->display_options['filters']['type_setting']['id'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['table'] = 'reservations_node_type';
  $handler->display->display_options['filters']['type_setting']['field'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['value'] = array(
    'bucket' => 'bucket',
    'resource' => 'resource',
  );
  $handler->display->display_options['filters']['type_setting']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Sub type -- resource/bucket */
  $handler->display->display_options['filters']['sub_type']['id'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['sub_type']['field'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['sub_type']['expose']['operator'] = FALSE;

  /* Display: Available Items */
  $handler = $view->new_display('page', 'Available Items', 'page_1');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'nid' => 'nid',
    'type' => 'type',
    'title' => 'title',
    'allow_overnight' => 'allow_overnight',
    'allow_weekends' => 'allow_weekends',
    'spare_items' => 'spare_items',
    'status' => 'status',
    'max_hours_per_reservation' => 'max_hours_per_reservation',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'nid' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'type' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'allow_overnight' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'allow_weekends' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'spare_items' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'max_hours_per_reservation' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/Available';
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Reservations: Reservations type */
  $handler->display->display_options['filters']['type_setting']['id'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['table'] = 'reservations_node_type';
  $handler->display->display_options['filters']['type_setting']['field'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['value'] = array(
    'bucket' => 'bucket',
    'resource' => 'resource',
  );
  $handler->display->display_options['filters']['type_setting']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Sub type -- resource/bucket */
  $handler->display->display_options['filters']['sub_type']['id'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['sub_type']['field'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['sub_type']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Default booking availability -- resource/bucket */
  $handler->display->display_options['filters']['default_availability']['id'] = 'default_availability';
  $handler->display->display_options['filters']['default_availability']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['default_availability']['field'] = 'default_availability';
  $handler->display->display_options['filters']['default_availability']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['default_availability']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Reservation Status of Item */
  $handler->display->display_options['filters']['item_status']['id'] = 'item_status';
  $handler->display->display_options['filters']['item_status']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['item_status']['field'] = 'item_status';
  $handler->display->display_options['filters']['item_status']['value'] = array(
    0 => '0',
  );
  $handler->display->display_options['filters']['item_status']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/current_inventory/type/%/Available';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Available';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['weight'] = '0';

  /* Display: Checked Out Items */
  $handler = $view->new_display('page', 'Checked Out Items', 'page_2');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Reservations: Reservation nid (for item nid) */
  $handler->display->display_options['relationships']['vid']['id'] = 'vid';
  $handler->display->display_options['relationships']['vid']['table'] = 'reservations_reservation_detail3';
  $handler->display->display_options['relationships']['vid']['field'] = 'vid';
  $handler->display->display_options['relationships']['vid']['label'] = 'Reservation for placeholder item node id';
  $handler->display->display_options['relationships']['vid']['required'] = 1;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'vid';
  $handler->display->display_options['fields']['title_1']['label'] = 'Checked out by';
  $handler->display->display_options['fields']['title_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title_1']['link_to_node'] = 1;
  /* Field: Content: Reservation */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_field_reservations_date';
  $handler->display->display_options['fields']['entity_id']['field'] = 'field_reservations_date';
  $handler->display->display_options['fields']['entity_id']['relationship'] = 'vid';
  $handler->display->display_options['fields']['entity_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['entity_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['entity_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['entity_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['entity_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['entity_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['entity_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['entity_id']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Reservations: Reservations type */
  $handler->display->display_options['filters']['type_setting']['id'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['table'] = 'reservations_node_type';
  $handler->display->display_options['filters']['type_setting']['field'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['value'] = array(
    'bucket' => 'bucket',
    'resource' => 'resource',
  );
  $handler->display->display_options['filters']['type_setting']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Sub type -- resource/bucket */
  $handler->display->display_options['filters']['sub_type']['id'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['sub_type']['field'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['sub_type']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Reservation Status of Item */
  $handler->display->display_options['filters']['item_status']['id'] = 'item_status';
  $handler->display->display_options['filters']['item_status']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['item_status']['field'] = 'item_status';
  $handler->display->display_options['filters']['item_status']['value'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['item_status']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Reservation status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'reservations_reservation';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'vid';
  $handler->display->display_options['filters']['status']['value'] = array(
    3 => '3',
  );
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/current_inventory/type/%/checked-out';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Checked Out';
  $handler->display->display_options['menu']['weight'] = '2';

  /* Display: Unavailable items */
  $handler = $view->new_display('page', 'Unavailable items', 'page_3');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/Unavailable';
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Reservations: Reservations type */
  $handler->display->display_options['filters']['type_setting']['id'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['table'] = 'reservations_node_type';
  $handler->display->display_options['filters']['type_setting']['field'] = 'type_setting';
  $handler->display->display_options['filters']['type_setting']['value'] = array(
    'bucket' => 'bucket',
    'resource' => 'resource',
  );
  $handler->display->display_options['filters']['type_setting']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Sub type -- resource/bucket */
  $handler->display->display_options['filters']['sub_type']['id'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['sub_type']['field'] = 'sub_type';
  $handler->display->display_options['filters']['sub_type']['value'] = array(
    1 => '1',
  );
  $handler->display->display_options['filters']['sub_type']['expose']['operator'] = FALSE;
  /* Filter criterion: Reservations: Default booking availability -- resource/bucket */
  $handler->display->display_options['filters']['default_availability']['id'] = 'default_availability';
  $handler->display->display_options['filters']['default_availability']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['filters']['default_availability']['field'] = 'default_availability';
  $handler->display->display_options['filters']['default_availability']['value'] = array(
    2 => '2',
  );
  $handler->display->display_options['filters']['default_availability']['expose']['operator'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reservations/manage/current_inventory/type/%/Unavailable';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Unavailable';
  $handler->display->display_options['menu']['weight'] = '1';

  /* Display: All Items */
  $handler = $view->new_display('page', 'All Items', 'page_4');
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['nid_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid_1']['link_to_node'] = 0;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  $handler->display->display_options['fields']['type']['link_to_node'] = 0;
  $handler->display->display_options['fields']['type']['machine_name'] = 1;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'node/[nid_1]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/all';
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Reservations: Default booking availability -- resource/bucket */
  $handler->display->display_options['fields']['default_availability']['id'] = 'default_availability';
  $handler->display->display_options['fields']['default_availability']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['fields']['default_availability']['field'] = 'default_availability';
  $handler->display->display_options['fields']['default_availability']['label'] = 'Availability';
  $handler->display->display_options['fields']['default_availability']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['default_availability']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['default_availability']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['default_availability']['alter']['html'] = 0;
  $handler->display->display_options['fields']['default_availability']['hide_empty'] = 0;
  $handler->display->display_options['fields']['default_availability']['empty_zero'] = 0;
  /* Field: Reservations: Reservation Status of Item */
  $handler->display->display_options['fields']['item_status']['id'] = 'item_status';
  $handler->display->display_options['fields']['item_status']['table'] = 'reservations_reservation_item_node';
  $handler->display->display_options['fields']['item_status']['field'] = 'item_status';
  $handler->display->display_options['fields']['item_status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['item_status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['item_status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['item_status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['item_status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['item_status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['item_status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['item_status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['item_status']['empty_zero'] = 0;
  $handler->display->display_options['path'] = 'admin/reservations/manage/current_inventory/type/%/all';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'All';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'admin/reservations/manage/current_inventory/all';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['reservations_reservable_items'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Reservation for placeholder item node id'),
    t('Nid'),
    t('Type'),
    t('Title'),
    t('node/[nid]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/Available'),
    t('Allow overnight'),
    t('Allow weekends'),
    t('Bucket spare items'),
    t('Bucket/Resource Status'),
    t('Max hours per reservation'),
    t('All'),
    t('%1 Detail'),
    t('Available Items'),
    t('Checked Out Items'),
    t('Checked out by'),
    t('Reservation'),
    t('Unavailable items'),
    t('node/[nid]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/Unavailable'),
    t('All Items'),
    t('node/[nid_1]/edit?destination=admin/reservations/manage/current_inventory/type/[type]/all'),
    t('Availability'),
    t('Reservation Status of Item'),
  );

  $views[$view->name] = $view;

  return $views;
}

