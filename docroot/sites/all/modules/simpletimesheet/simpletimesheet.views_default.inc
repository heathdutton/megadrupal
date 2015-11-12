<?php
/**
 * @file 
 * views_default objects
 * simpletimesheet.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function simpletimesheet_views_default_views() {
  $views = array();

  $view = new view;
  $view->name = 'active_activities';
  $view->description = 'List of active activities the user is a member of';
  $view->tag = 'timesheet';
  $view->base_table = 'node';
  $view->human_name = 'Active Activities';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'create time_record content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
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
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Content: Members (field_members) */
  $handler->display->display_options['arguments']['field_members_uid']['id'] = 'field_members_uid';
  $handler->display->display_options['arguments']['field_members_uid']['table'] = 'field_data_field_members';
  $handler->display->display_options['arguments']['field_members_uid']['field'] = 'field_members_uid';
  $handler->display->display_options['arguments']['field_members_uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_members_uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['field_members_uid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_members_uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_members_uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_members_uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_members_uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['field_members_uid']['not'] = 0;
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
    'activity' => 'activity',
  );
  /* Filter criterion: Content: Active (field_active) */
  $handler->display->display_options['filters']['field_active_value']['id'] = 'field_active_value';
  $handler->display->display_options['filters']['field_active_value']['table'] = 'field_data_field_active';
  $handler->display->display_options['filters']['field_active_value']['field'] = 'field_active_value';
  $handler->display->display_options['filters']['field_active_value']['value'] = array(
    1 => '1',
  );

  /* Display: References */
  $handler = $view->new_display('references', 'References', 'references_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'references_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $views['active_activities'] = $view;

  $view = new view;
  $view->name = 'activity_list';
  $view->description = '';
  $view->tag = 'timesheet';
  $view->base_table = 'node';
  $view->human_name = 'Activity list';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Activity list';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'simpletimesheet access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'name' => 'name',
    'edit_node' => 'edit_node',
    'nid' => 'nid',
    'nid_1' => 'nid',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'edit_node' => array(
      'align' => '',
      'separator' => '',
    ),
    'nid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' | ',
    ),
    'nid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'You can browse existing activities by name clicking on the above first letter pages.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Activity';
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
  $handler->display->display_options['fields']['title']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Owner';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['label'] = 'Properties';
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
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = 'Timesheet details';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'view';
  $handler->display->display_options['fields']['nid']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nid']['alter']['path'] = 'timesheet/activity_details/[nid]';
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
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['label'] = '';
  $handler->display->display_options['fields']['nid_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['text'] = 'export';
  $handler->display->display_options['fields']['nid_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['path'] = 'timesheet/csv?activity=[nid_1]';
  $handler->display->display_options['fields']['nid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['nid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['nid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['nid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['nid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['nid_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['nid_1']['link_to_node'] = 0;
  /* Sort criterion: Content: Title */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'node';
  $handler->display->display_options['sorts']['title']['field'] = 'title';
  /* Contextual filter: Content: Title */
  $handler->display->display_options['arguments']['title']['id'] = 'title';
  $handler->display->display_options['arguments']['title']['table'] = 'node';
  $handler->display->display_options['arguments']['title']['field'] = 'title';
  $handler->display->display_options['arguments']['title']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['title']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['title']['default_argument_options']['argument'] = 'a';
  $handler->display->display_options['arguments']['title']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['title']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['title']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['title']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['title']['glossary'] = 1;
  $handler->display->display_options['arguments']['title']['limit'] = '1';
  $handler->display->display_options['arguments']['title']['case'] = 'upper';
  $handler->display->display_options['arguments']['title']['path_case'] = 'lower';
  $handler->display->display_options['arguments']['title']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['title']['break_phrase'] = 0;
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
    'activity' => 'activity',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a href="/node/add/activiy?destination=timesheet/activity-list">Add new activity +</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  $handler->display->display_options['path'] = 'timesheet/activity-list';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Activity list';
  $handler->display->display_options['menu']['weight'] = '2';

  /* Display: Attachment */
  $handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Title */
  $handler->display->display_options['arguments']['title']['id'] = 'title';
  $handler->display->display_options['arguments']['title']['table'] = 'node';
  $handler->display->display_options['arguments']['title']['field'] = 'title';
  $handler->display->display_options['arguments']['title']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['title']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['title']['default_argument_options']['argument'] = 'a';
  $handler->display->display_options['arguments']['title']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['title']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['title']['summary']['format'] = 'unformatted_summary';
  $handler->display->display_options['arguments']['title']['summary_options']['count'] = FALSE;
  $handler->display->display_options['arguments']['title']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['title']['summary_options']['inline'] = '1';
  $handler->display->display_options['arguments']['title']['summary_options']['separator'] = ' | ';
  $handler->display->display_options['arguments']['title']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['title']['glossary'] = 1;
  $handler->display->display_options['arguments']['title']['limit'] = '1';
  $handler->display->display_options['arguments']['title']['case'] = 'upper';
  $handler->display->display_options['arguments']['title']['path_case'] = 'lower';
  $handler->display->display_options['arguments']['title']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['title']['break_phrase'] = 0;
  $handler->display->display_options['displays'] = array(
    'default' => 'default',
    'page' => 'page',
  );
  $handler->display->display_options['inherit_arguments'] = 0;
  $views['activity_list'] = $view;

  $view = new view;
  $view->name = 'clone_of_fullcalendar';
  $view->description = 'Implement a calendar view of existing time records.';
  $view->tag = 'timesheet';
  $view->base_table = 'node';
  $view->human_name = 'Timesheet calendar';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Timesheet calendar';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'simpletimesheet access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'fullcalendar';
  $handler->display->display_options['style_options']['display']['fc_view'] = 'agendaWeek';
  $handler->display->display_options['style_options']['display']['fc_firstday'] = '1';
  $handler->display->display_options['style_options']['modules']['fc_theme'] = 1;
  $handler->display->display_options['style_options']['modules']['fc_url_colorbox'] = 0;
  $handler->display->display_options['style_options']['modules']['fc_window'] = 0;
  $handler->display->display_options['style_options']['times']['fc_clock'] = 0;
  $handler->display->display_options['style_options']['times']['fc_default_date'] = 0;
  $handler->display->display_options['style_options']['times']['fc_date'] = array(
    'year' => '1900',
    'month' => '1',
    'day' => '1',
  );
  $handler->display->display_options['row_plugin'] = 'fullcalendar_node';
  $handler->display->display_options['row_options']['custom']['fc_title_field'] = 'title';
  $handler->display->display_options['row_options']['custom']['fc_url_field'] = 'name';
  $handler->display->display_options['row_options']['custom']['fc_date_field'] = array();
  $handler->display->display_options['row_options']['custom']['fc_title'] = 1;
  $handler->display->display_options['row_options']['custom']['fc_url'] = 0;
  $handler->display->display_options['row_options']['custom']['fc_date'] = 0;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a href="/node/add/time-record?destination=timesheet">Add new time record +</a>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = 0;
  /* No results behavior: FullCalendar: Empty calendar */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'fullcalendar';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['title']['alter']['text'] = '[name]: [title]';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Time */
  $handler->display->display_options['fields']['field_time']['id'] = 'field_time';
  $handler->display->display_options['fields']['field_time']['table'] = 'field_data_field_time';
  $handler->display->display_options['fields']['field_time']['field'] = 'field_time';
  $handler->display->display_options['fields']['field_time']['label'] = '';
  $handler->display->display_options['fields']['field_time']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_time']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_time']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_time']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_time']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_time']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_time']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_time']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_time']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_time']['field_api_classes'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'time_record' => 'time_record',
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  /* Filter criterion: Content: Activity (field_activity) */
  $handler->display->display_options['filters']['field_activity_nid']['id'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['table'] = 'field_data_field_activity';
  $handler->display->display_options['filters']['field_activity_nid']['field'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator_id'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['label'] = 'Activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['identifier'] = 'activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['reduce'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'timesheet/calendar';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Calendar';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Timesheet';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $views['clone_of_fullcalendar'] = $view;

  $view = new view;
  $view->name = 'timesheet_by';
  $view->description = 'Timesheet view by project or by user';
  $view->tag = 'timesheet';
  $view->base_table = 'node';
  $view->human_name = 'Timesheet by';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'simpletimesheet access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_activity' => 'field_activity',
    'field_time' => 'field_time',
    'php' => 'php',
    'field_activity_1' => 'field_activity_1',
    'field_activity_2' => 'field_activity_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_activity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_time' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'php' => array(
      'align' => '',
      'separator' => '',
    ),
    'field_activity_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' | ',
    ),
    'field_activity_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Footer: Global: PHP */
  $handler->display->display_options['footer']['php']['id'] = 'php';
  $handler->display->display_options['footer']['php']['table'] = 'views';
  $handler->display->display_options['footer']['php']['field'] = 'php';
  $handler->display->display_options['footer']['php']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No results available. Try using broader filters.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = 0;
  /* Relationship: Content: Activity (field_activity) */
  $handler->display->display_options['relationships']['field_activity_nid']['id'] = 'field_activity_nid';
  $handler->display->display_options['relationships']['field_activity_nid']['table'] = 'field_data_field_activity';
  $handler->display->display_options['relationships']['field_activity_nid']['field'] = 'field_activity_nid';
  $handler->display->display_options['relationships']['field_activity_nid']['required'] = 0;
  $handler->display->display_options['relationships']['field_activity_nid']['delta'] = '0';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php']['php_setup'] = '$static = drupal_http_build_query($view->get_exposed_input());';
  $handler->display->display_options['fields']['php']['php_value'] = 'if (strlen($static)) { $static = \'?\' . $static; }
return $static;
';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo $value;
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity_1']['id'] = 'field_activity_1';
  $handler->display->display_options['fields']['field_activity_1']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity_1']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['group_column'] = 'entity id';
  $handler->display->display_options['fields']['field_activity_1']['field_api_classes'] = 0;
  /* Field: SUM(Content: Time) */
  $handler->display->display_options['fields']['field_time']['id'] = 'field_time';
  $handler->display->display_options['fields']['field_time']['table'] = 'field_data_field_time';
  $handler->display->display_options['fields']['field_time']['field'] = 'field_time';
  $handler->display->display_options['fields']['field_time']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_time']['label'] = 'Hours (total)';
  $handler->display->display_options['fields']['field_time']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_time']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_time']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_time']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_time']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_time']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_time']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_time']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_time']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_time']['type'] = 'list_key';
  $handler->display->display_options['fields']['field_time']['field_api_classes'] = 0;
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity']['id'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['group_column'] = 'entity id';
  /* Sort criterion: Content: Time (field_time) */
  $handler->display->display_options['sorts']['field_time_value']['id'] = 'field_time_value';
  $handler->display->display_options['sorts']['field_time_value']['table'] = 'field_data_field_time';
  $handler->display->display_options['sorts']['field_time_value']['field'] = 'field_time_value';
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
    'time_record' => 'time_record',
  );
  /* Filter criterion: Broken/missing handler */
  $handler->display->display_options['filters']['field_date_value']['id'] = 'field_date_value';
  $handler->display->display_options['filters']['field_date_value']['table'] = 'field_data_field_date';
  $handler->display->display_options['filters']['field_date_value']['field'] = 'field_date_value';
  $handler->display->display_options['filters']['field_date_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_date_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_date_value']['expose']['operator_id'] = 'field_date_value_op';
  $handler->display->display_options['filters']['field_date_value']['expose']['operator'] = 'field_date_value_op';
  $handler->display->display_options['filters']['field_date_value']['expose']['identifier'] = 'field_date';
  $handler->display->display_options['filters']['field_date_value']['expose']['multiple'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['use_operator'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['required'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['remember'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;

  /* Display: By activity */
  $handler = $view->new_display('page', 'By activity', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Timesheet by activity';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'php' => 'php',
    'field_activity_1' => 'field_activity_1',
    'field_lenght' => 'field_lenght',
    'field_activity' => 'field_activity',
    'field_activity_2' => 'field_activity',
  );
  $handler->display->display_options['style_options']['default'] = 'field_activity_1';
  $handler->display->display_options['style_options']['info'] = array(
    'php' => array(
      'align' => '',
      'separator' => '',
    ),
    'field_activity_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_lenght' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_activity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' | ',
    ),
    'field_activity_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 0;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Parms';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php']['php_setup'] = '$static = drupal_http_build_query($view->get_exposed_input());
';
  $handler->display->display_options['fields']['php']['php_value'] = 'if (strlen($static)) { $static = \'?\' . $static; }
return $static;';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo $value;
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'CSV url';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php_1']['php_setup'] = '$static = drupal_http_build_query($view->get_exposed_input());';
  $handler->display->display_options['fields']['php_1']['php_value'] = 'if (strlen($static)) { $static = \'&\' . $static; }
return $static;
';
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php
print $value;
?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity_1']['id'] = 'field_activity_1';
  $handler->display->display_options['fields']['field_activity_1']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity_1']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_1']['type'] = 'node_reference_plain';
  $handler->display->display_options['fields']['field_activity_1']['field_api_classes'] = 0;
  /* Field: SUM(Content: Lenght) */
  $handler->display->display_options['fields']['field_lenght']['id'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['table'] = 'field_data_field_lenght';
  $handler->display->display_options['fields']['field_lenght']['field'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_lenght']['label'] = 'Time spent';
  $handler->display->display_options['fields']['field_lenght']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_lenght']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_lenght']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_lenght']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_lenght']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['field_api_classes'] = 0;
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity']['id'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['label'] = 'Details';
  $handler->display->display_options['fields']['field_activity']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['text'] = 'view';
  $handler->display->display_options['fields']['field_activity']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['path'] = 'timesheet/activity_details/[field_activity-nid][php]';
  $handler->display->display_options['fields']['field_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity']['group_column'] = 'entity id';
  $handler->display->display_options['fields']['field_activity']['field_api_classes'] = 0;
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity_2']['id'] = 'field_activity_2';
  $handler->display->display_options['fields']['field_activity_2']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity_2']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity_2']['label'] = 'export';
  $handler->display->display_options['fields']['field_activity_2']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['field_activity_2']['alter']['text'] = 'export';
  $handler->display->display_options['fields']['field_activity_2']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_activity_2']['alter']['path'] = 'timesheet/csv?activity=[field_activity_2-nid][php_1]';
  $handler->display->display_options['fields']['field_activity_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_activity_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity_2']['group_column'] = 'entity id';
  $handler->display->display_options['fields']['field_activity_2']['field_api_classes'] = 0;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'time_record' => 'time_record',
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  /* Filter criterion: Content: Time (field_time) */
  $handler->display->display_options['filters']['field_time_value']['id'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['table'] = 'field_data_field_time';
  $handler->display->display_options['filters']['field_time_value']['field'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_time_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_time_value']['expose']['operator_id'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['operator'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['identifier'] = 'period';
  $handler->display->display_options['filters']['field_time_value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_time_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_time_value']['default_date'] = 'now -14 days';
  $handler->display->display_options['filters']['field_time_value']['default_to_date'] = 'now';
  $handler->display->display_options['path'] = 'timesheet/by_activity';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'By activity';
  $handler->display->display_options['menu']['weight'] = '4';

  /* Display: Details by activity */
  $handler = $view->new_display('page', 'Details by activity', 'page_4');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Timesheet details by activity';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = 'php_1';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'field_time' => 'field_time',
    'php' => 'php',
    'php_1' => 'php_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_time' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'php' => array(
      'align' => '',
      'separator' => '',
    ),
    'php_1' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: PHP */
  $handler->display->display_options['footer']['php']['id'] = 'php';
  $handler->display->display_options['footer']['php']['table'] = 'views';
  $handler->display->display_options['footer']['php']['field'] = 'php';
  $handler->display->display_options['footer']['php']['empty'] = FALSE;
  $handler->display->display_options['footer']['php']['php_output'] = '<?php
$parms = drupal_http_build_query($view->get_exposed_input());
if (strlen($parms)) {$parms = \'&\' . $parms;}
print \'<a href="/timesheet/csv?activity=\' .  $view->args[0]  . $parms . \'"><img src="/\' . drupal_get_path(\'module\', \'views_data_export\') . \'/images/csv.png"></a>\';
?>';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No results available. Please check an activity has been selected. In case select one from the <a href="/timesheet/activity-list">activity list</a>. If an activity has already been selected try using broader filters.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 1;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
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
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Lenght */
  $handler->display->display_options['fields']['field_lenght']['id'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['table'] = 'field_data_field_lenght';
  $handler->display->display_options['fields']['field_lenght']['field'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['label'] = 'Time spent';
  $handler->display->display_options['fields']['field_lenght']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_lenght']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_lenght']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_lenght']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_lenght']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['field_api_classes'] = 0;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php']['php_setup'] = '$static =  drupal_http_build_query($view->get_exposed_input());';
  $handler->display->display_options['fields']['php']['php_value'] = 'if (strlen($static)) { $static = \'?\' . $static; }
return $static;';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo $value;
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'Date';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_value'] = '$node = node_load($row->field_lenght);
$field = field_get_items(\'node\', $node, \'field_time\', $node->language);
return $field[0][\'value\'];
';
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php print date(\'Y-m-d\', $value);?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'field_activity_nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['nid']['title_enable'] = 1;
  $handler->display->display_options['arguments']['nid']['title'] = 'Timesheet details for activity %1';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['nid']['not'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'time_record' => 'time_record',
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  /* Filter criterion: Content: Time (field_time:value2) */
  $handler->display->display_options['filters']['field_time_value2']['id'] = 'field_time_value2';
  $handler->display->display_options['filters']['field_time_value2']['table'] = 'field_data_field_time';
  $handler->display->display_options['filters']['field_time_value2']['field'] = 'field_time_value2';
  $handler->display->display_options['filters']['field_time_value2']['operator'] = 'between';
  $handler->display->display_options['filters']['field_time_value2']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_time_value2']['expose']['operator_id'] = 'field_time_value2_op';
  $handler->display->display_options['filters']['field_time_value2']['expose']['operator'] = 'field_time_value2_op';
  $handler->display->display_options['filters']['field_time_value2']['expose']['identifier'] = 'period';
  $handler->display->display_options['filters']['field_time_value2']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_time_value2']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_time_value2']['default_date'] = 'now -14 days';
  $handler->display->display_options['filters']['field_time_value2']['default_to_date'] = 'now';
  $handler->display->display_options['path'] = 'timesheet/activity_details';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Activity details';
  $handler->display->display_options['menu']['weight'] = '5';

  /* Display: By user */
  $handler = $view->new_display('page', 'By user', 'page_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Timesheet by user';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'php' => 'php',
    'name' => 'name',
    'field_lenght' => 'field_lenght',
    'uid' => 'uid',
    'uid_1' => 'uid',
  );
  $handler->display->display_options['style_options']['default'] = 'name';
  $handler->display->display_options['style_options']['info'] = array(
    'php' => array(
      'align' => '',
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_lenght' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'uid' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' | ',
    ),
    'uid_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = FALSE;
  $handler->display->display_options['empty']['area']['content'] = 'No results available. Try using broader filters.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  $handler->display->display_options['empty']['area']['tokenize'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php']['php_setup'] = '$static = drupal_http_build_query($view->get_exposed_input());';
  $handler->display->display_options['fields']['php']['php_value'] = 'if (strlen($static)) { $static = \'?\' . $static; }
return $static;
';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo $value;
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'CSV url';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php_1']['php_setup'] = '$static = drupal_http_build_query($view->get_exposed_input());';
  $handler->display->display_options['fields']['php_1']['php_value'] = 'if (strlen($static)) { $static = \'&\' . $static; }
return $static;';
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php
print $value;
?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: SUM(Content: Lenght) */
  $handler->display->display_options['fields']['field_lenght']['id'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['table'] = 'field_data_field_lenght';
  $handler->display->display_options['fields']['field_lenght']['field'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['group_type'] = 'sum';
  $handler->display->display_options['fields']['field_lenght']['label'] = 'Time spent';
  $handler->display->display_options['fields']['field_lenght']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_lenght']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_lenght']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_lenght']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_lenght']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['field_api_classes'] = 0;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'users';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid']['label'] = 'Details';
  $handler->display->display_options['fields']['uid']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['text'] = 'view';
  $handler->display->display_options['fields']['uid']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['path'] = 'timesheet/user_details/[uid][php]';
  $handler->display->display_options['fields']['uid']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['replace_spaces'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uid']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uid']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uid']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['uid']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['uid']['link_to_user'] = 0;
  /* Field: User: Uid */
  $handler->display->display_options['fields']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['fields']['uid_1']['table'] = 'users';
  $handler->display->display_options['fields']['uid_1']['field'] = 'uid';
  $handler->display->display_options['fields']['uid_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['uid_1']['label'] = '';
  $handler->display->display_options['fields']['uid_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['uid_1']['alter']['text'] = 'export';
  $handler->display->display_options['fields']['uid_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['uid_1']['alter']['path'] = 'timesheet/csv?uid=[name][php_1]';
  $handler->display->display_options['fields']['uid_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['uid_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['uid_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['uid_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['uid_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['uid_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['uid_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['uid_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['uid_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['uid_1']['link_to_user'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'time_record' => 'time_record',
  );
  /* Filter criterion: Content: Activity (field_activity) */
  $handler->display->display_options['filters']['field_activity_nid']['id'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['table'] = 'field_data_field_activity';
  $handler->display->display_options['filters']['field_activity_nid']['field'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator_id'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['label'] = 'Activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['identifier'] = 'activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['reduce'] = 0;
  /* Filter criterion: Content: Time (field_time) */
  $handler->display->display_options['filters']['field_time_value']['id'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['table'] = 'field_data_field_time';
  $handler->display->display_options['filters']['field_time_value']['field'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_time_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_time_value']['expose']['operator_id'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['operator'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['identifier'] = 'period';
  $handler->display->display_options['filters']['field_time_value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_time_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_time_value']['default_date'] = 'now -14 days';
  $handler->display->display_options['filters']['field_time_value']['default_to_date'] = 'now';
  $handler->display->display_options['path'] = 'timesheet/by_user';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'By user';
  $handler->display->display_options['menu']['weight'] = '5';

  /* Display: Details by user */
  $handler = $view->new_display('page', 'Details by user', 'page_3');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = 'php_1';
  $handler->display->display_options['style_options']['columns'] = array(
    'php' => 'php',
    'field_activity' => 'field_activity',
    'title' => 'title',
    'field_lenght' => 'field_lenght',
    'php_1' => 'php_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'php' => array(
      'align' => '',
      'separator' => '',
    ),
    'field_activity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'field_lenght' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'php_1' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['order'] = 'desc';
  $handler->display->display_options['style_options']['empty_table'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: PHP */
  $handler->display->display_options['footer']['php']['id'] = 'php';
  $handler->display->display_options['footer']['php']['table'] = 'views';
  $handler->display->display_options['footer']['php']['field'] = 'php';
  $handler->display->display_options['footer']['php']['empty'] = FALSE;
  $handler->display->display_options['footer']['php']['php_output'] = '<?php
$parms = drupal_http_build_query($view->get_exposed_input());
if (strlen($parms)) {$parms = \'&\' . $parms;}
$account = user_load((int)$view->args[0]);
print \'<a href="/timesheet/csv?uid=\' .  $account->name  . $parms . \'"><img src="/\' . drupal_get_path(\'module\', \'views_data_export\') . \'/images/csv.png"></a>\';
?>
';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 1;
  $handler->display->display_options['fields']['php']['php_setup'] = '$static = drupal_http_build_query($view->get_exposed_input());';
  $handler->display->display_options['fields']['php']['php_value'] = 'if (strlen($static)) { $static = \'?\' . $static; }
return $static;
';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo $value;
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity']['id'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity']['group_column'] = 'entity id';
  $handler->display->display_options['fields']['field_activity']['field_api_classes'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Description';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Content: Lenght */
  $handler->display->display_options['fields']['field_lenght']['id'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['table'] = 'field_data_field_lenght';
  $handler->display->display_options['fields']['field_lenght']['field'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['label'] = 'Time spent';
  $handler->display->display_options['fields']['field_lenght']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_lenght']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_lenght']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_lenght']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_lenght']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['field_api_classes'] = 0;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'Date';
  $handler->display->display_options['fields']['php_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['php_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php_1']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_value'] = '$node = node_load($row->field_lenght);
$field = field_get_items(\'node\', $node, \'field_time\', $node->language);
return $field[0][\'value\'];';
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php
echo date(\'Y-m-d\', $value);
?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Time (field_time) */
  $handler->display->display_options['sorts']['field_time_value']['id'] = 'field_time_value';
  $handler->display->display_options['sorts']['field_time_value']['table'] = 'field_data_field_time';
  $handler->display->display_options['sorts']['field_time_value']['field'] = 'field_time_value';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['title_enable'] = 1;
  $handler->display->display_options['arguments']['uid']['title'] = 'Timesheet details for user %1';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'php';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['code'] = 'global $user;
return $user->uid;';
  $handler->display->display_options['arguments']['uid']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
  $handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = 0;
  $handler->display->display_options['arguments']['uid']['validate']['fail'] = 'empty';
  $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid']['not'] = 0;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'time_record' => 'time_record',
  );
  /* Filter criterion: Content: Activity (field_activity) */
  $handler->display->display_options['filters']['field_activity_nid']['id'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['table'] = 'field_data_field_activity';
  $handler->display->display_options['filters']['field_activity_nid']['field'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator_id'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['label'] = 'Activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['identifier'] = 'activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['reduce'] = 0;
  /* Filter criterion: Content: Time (field_time) */
  $handler->display->display_options['filters']['field_time_value']['id'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['table'] = 'field_data_field_time';
  $handler->display->display_options['filters']['field_time_value']['field'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_time_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_time_value']['expose']['operator_id'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['operator'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['identifier'] = 'period';
  $handler->display->display_options['filters']['field_time_value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_time_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_time_value']['default_date'] = 'now -14 days';
  $handler->display->display_options['filters']['field_time_value']['default_to_date'] = 'now';
  $handler->display->display_options['path'] = 'timesheet/user_details';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'User details';
  $handler->display->display_options['menu']['weight'] = '5';

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'Date';
  $handler->display->display_options['fields']['php']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['php']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['php']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['php']['alter']['external'] = 0;
  $handler->display->display_options['fields']['php']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['php']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['php']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['php']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['php']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['php']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['php']['alter']['html'] = 0;
  $handler->display->display_options['fields']['php']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['php']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['php']['hide_empty'] = 0;
  $handler->display->display_options['fields']['php']['empty_zero'] = 0;
  $handler->display->display_options['fields']['php']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_value'] = '$node = node_load($row->field_lenght);
$field = field_get_items(\'node\', $node, \'field_time\', $node->language);
return $field[0][\'value\'];
';
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
echo date(\'Y-m-d\', $value);
?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Content: Activity */
  $handler->display->display_options['fields']['field_activity']['id'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['table'] = 'field_data_field_activity';
  $handler->display->display_options['fields']['field_activity']['field'] = 'field_activity';
  $handler->display->display_options['fields']['field_activity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_activity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_activity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_activity']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['field_activity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_activity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_activity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_activity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_activity']['field_api_classes'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 0;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 0;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Description';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 0;
  /* Field: Content: Lenght */
  $handler->display->display_options['fields']['field_lenght']['id'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['table'] = 'field_data_field_lenght';
  $handler->display->display_options['fields']['field_lenght']['field'] = 'field_lenght';
  $handler->display->display_options['fields']['field_lenght']['label'] = 'Time spent';
  $handler->display->display_options['fields']['field_lenght']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_lenght']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_lenght']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_lenght']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_lenght']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_lenght']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_lenght']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_lenght']['type'] = 'computed_field_computed_value';
  $handler->display->display_options['fields']['field_lenght']['field_api_classes'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'time_record' => 'time_record',
  );
  /* Filter criterion: Content: Time (field_time) */
  $handler->display->display_options['filters']['field_time_value']['id'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['table'] = 'field_data_field_time';
  $handler->display->display_options['filters']['field_time_value']['field'] = 'field_time_value';
  $handler->display->display_options['filters']['field_time_value']['operator'] = 'between';
  $handler->display->display_options['filters']['field_time_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_time_value']['expose']['operator_id'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['label'] = 'Time';
  $handler->display->display_options['filters']['field_time_value']['expose']['operator'] = 'field_time_value_op';
  $handler->display->display_options['filters']['field_time_value']['expose']['identifier'] = 'period';
  $handler->display->display_options['filters']['field_time_value']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['field_time_value']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['field_time_value']['default_date'] = 'now -14 days';
  $handler->display->display_options['filters']['field_time_value']['default_to_date'] = 'now';
  /* Filter criterion: Content: Activity (field_activity) */
  $handler->display->display_options['filters']['field_activity_nid']['id'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['table'] = 'field_data_field_activity';
  $handler->display->display_options['filters']['field_activity_nid']['field'] = 'field_activity_nid';
  $handler->display->display_options['filters']['field_activity_nid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator_id'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['label'] = 'Activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['operator'] = 'field_activity_nid_op';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['identifier'] = 'activity';
  $handler->display->display_options['filters']['field_activity_nid']['expose']['reduce'] = 0;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['multiple'] = FALSE;
  $handler->display->display_options['filters']['uid']['expose']['reduce'] = 0;
  $handler->display->display_options['path'] = 'timesheet/csv';
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'page_2' => 'page_2',
    'default' => 0,
    'page_4' => 0,
    'page_3' => 0,
  );
  $handler->display->display_options['use_batch'] = '0';
  $views['timesheet_by'] = $view;

  return $views;
}
