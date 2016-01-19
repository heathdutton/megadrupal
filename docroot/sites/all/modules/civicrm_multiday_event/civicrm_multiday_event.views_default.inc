<?php
/**
 * @file
 * civicrm_multiday_event.views_default.inc
 */

/**
 * Implementation of hook_views_default_views().
 */
function civicrm_multiday_event_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'civicrm_multiday_event_cal';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CiviCRM Multiday Event: Calendar';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'CiviCRM Multiday Event Calendar';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit all events';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['fields']['url_field'] = 'nid';
  $handler->display->display_options['style_options']['fields']['title'] = TRUE;
  $handler->display->display_options['style_options']['fields']['url'] = TRUE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<ul class="action-links">
<li><a href="/node/add/civicrm-multiday-event">Add Event</a></li>
</ul>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Relationship: Content: Sessions (field_civicrm_multiday_session) */
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['id'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['table'] = 'field_data_field_civicrm_multiday_session';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['field'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['required'] = 0;
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Field collection item: Session Date */
  $handler->display->display_options['fields']['field_session_date']['id'] = 'field_session_date';
  $handler->display->display_options['fields']['field_session_date']['table'] = 'field_data_field_session_date';
  $handler->display->display_options['fields']['field_session_date']['field'] = 'field_session_date';
  $handler->display->display_options['fields']['field_session_date']['relationship'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['fields']['field_session_date']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_session_date']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_session_date']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_session_date']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_session_date']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_session_date']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_session_date']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_session_date']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_session_date']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_session_date']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_session_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_session_date']['field_api_classes'] = 0;
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'node/[nid]';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'node/[nid]';
  $handler->display->display_options['fields']['nid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['nid']['link_to_node'] = 0;
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
    'civicrm_multiday_event' => 'civicrm_multiday_event',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/civicrm-multiday-events/manage/calendar';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Calendar';
  $handler->display->display_options['menu']['weight'] = '-45';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Events';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $export['civicrm_multiday_event_cal'] = $view;

  $view = new view;
  $view->name = 'civicrm_multiday_event_list';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'CiviCRM Multiday Event: Search';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Event Search';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'edit all events';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['expose']['items_per_page_options_all'] = 0;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'field_session_date' => 'field_session_date',
    'nid' => 'nid',
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
    'field_session_date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<ul class="action-links">
<li><a href="/node/add/civicrm-multiday-event">Add Event</a></li>
</ul>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* Relationship: Content: Sessions (field_civicrm_multiday_session) */
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['id'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['table'] = 'field_data_field_civicrm_multiday_session';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['field'] = 'field_civicrm_multiday_session_value';
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['required'] = 0;
  $handler->display->display_options['relationships']['field_civicrm_multiday_session_value']['delta'] = '-1';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Sessions */
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['id'] = 'field_civicrm_multiday_session';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['table'] = 'field_data_field_civicrm_multiday_session';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['field'] = 'field_civicrm_multiday_session';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['type'] = 'field_collection_fields';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['settings'] = array(
    'view_mode' => 'full',
  );
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['group_rows'] = 1;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['delta_offset'] = '0';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['delta_reversed'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['delta_first_last'] = 0;
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['multi_type'] = 'ul';
  $handler->display->display_options['fields']['field_civicrm_multiday_session']['field_api_classes'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'civicrm_multiday_event' => 'civicrm_multiday_event',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['required'] = 0;
  $handler->display->display_options['filters']['title']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/civicrm-multiday-events/manage/list';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Search';
  $handler->display->display_options['menu']['weight'] = '-45';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $export['civicrm_multiday_event_list'] = $view;
  
  $view = new view();
	$view->name = 'civicrm_multiday_event_upcoming_events';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'CiviCRM Multiday Event: Upcoming Events';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'CiviCRM Multiday Event: Upcoming Events';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['pager']['options']['items_per_page'] = '5';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'fields';
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['label'] = '';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	/* Sort criterion: Content: Date of First Session (field_date_of_first_session) */
	$handler->display->display_options['sorts']['field_date_of_first_session_value']['id'] = 'field_date_of_first_session_value';
	$handler->display->display_options['sorts']['field_date_of_first_session_value']['table'] = 'field_data_field_date_of_first_session';
	$handler->display->display_options['sorts']['field_date_of_first_session_value']['field'] = 'field_date_of_first_session_value';
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
		'civicrm_multiday_event' => 'civicrm_multiday_event',
	);
	/* Filter criterion: Content: Date of First Session (field_date_of_first_session) */
	$handler->display->display_options['filters']['field_date_of_first_session_value']['id'] = 'field_date_of_first_session_value';
	$handler->display->display_options['filters']['field_date_of_first_session_value']['table'] = 'field_data_field_date_of_first_session';
	$handler->display->display_options['filters']['field_date_of_first_session_value']['field'] = 'field_date_of_first_session_value';
	$handler->display->display_options['filters']['field_date_of_first_session_value']['operator'] = '>=';
	$handler->display->display_options['filters']['field_date_of_first_session_value']['form_type'] = 'date_text';
	$handler->display->display_options['filters']['field_date_of_first_session_value']['default_date'] = 'now -1 day';

	/* Display: Block */
	$handler = $view->new_display('block', 'Block', 'block');
  $export['civicrm_multiday_event_upcoming_events'] = $view;

  return $export;
}
