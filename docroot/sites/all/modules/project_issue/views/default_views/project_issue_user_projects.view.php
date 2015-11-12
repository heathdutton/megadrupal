<?php

/**
 * @file
 * Issues across all projects owned by a user.
 */
$view = new view();
$view->name = 'project_issue_user_projects';
$view->description = 'Project issues for a specific user\'s projects';
$view->tag = 'Project issue';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'project_issue_access_per_user_queue';
$handler->display->display_options['access']['project_issue_user_argument'] = 'uid';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['pager']['options']['quantity'] = '9';
$handler->display->display_options['style_plugin'] = 'project_issue_table';
$handler->display->display_options['style_options']['columns'] = array(
  'field_project_machine_name' => 'field_project_machine_name',
  'title_1' => 'title_1',
  'title' => 'title',
  'timestamp' => 'title',
  'field_issue_status' => 'field_issue_status',
  'field_issue_priority' => 'field_issue_priority',
  'field_issue_category' => 'field_issue_category',
  'field_issue_version' => 'field_issue_version',
  'comment_count' => 'comment_count',
  'new_comments' => 'comment_count',
  'last_comment_timestamp' => 'last_comment_timestamp',
  'name' => 'name',
  'created' => 'created',
  'score' => 'score',
);
$handler->display->display_options['style_options']['default'] = 'score';
$handler->display->display_options['style_options']['info'] = array(
  'field_project_machine_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => ' ',
    'empty_column' => 0,
  ),
  'timestamp' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_issue_status' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_issue_priority' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_issue_category' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_issue_version' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'comment_count' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '<br />',
    'empty_column' => 0,
  ),
  'new_comments' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'last_comment_timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'created' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'score' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['style_options']['sticky'] = TRUE;
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['content'] = 'No issues match your criteria.';
$handler->display->display_options['empty']['area']['format'] = '1';
/* Relationship: Assigned user */
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['id'] = 'field_issue_assigned_target_id';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['table'] = 'field_data_field_issue_assigned';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['field'] = 'field_issue_assigned_target_id';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['ui_name'] = 'Assigned user';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['label'] = 'Assigned user';
/* Relationship: Project */
$handler->display->display_options['relationships']['field_project_target_id']['id'] = 'field_project_target_id';
$handler->display->display_options['relationships']['field_project_target_id']['table'] = 'field_data_field_project';
$handler->display->display_options['relationships']['field_project_target_id']['field'] = 'field_project_target_id';
$handler->display->display_options['relationships']['field_project_target_id']['ui_name'] = 'Project';
$handler->display->display_options['relationships']['field_project_target_id']['label'] = 'Project';
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['relationship'] = 'field_project_target_id';
$handler->display->display_options['relationships']['uid']['label'] = 'Project author';
/* Field: Content: Short name */
$handler->display->display_options['fields']['field_project_machine_name']['id'] = 'field_project_machine_name';
$handler->display->display_options['fields']['field_project_machine_name']['table'] = 'field_data_field_project_machine_name';
$handler->display->display_options['fields']['field_project_machine_name']['field'] = 'field_project_machine_name';
$handler->display->display_options['fields']['field_project_machine_name']['relationship'] = 'field_project_target_id';
$handler->display->display_options['fields']['field_project_machine_name']['label'] = '';
$handler->display->display_options['fields']['field_project_machine_name']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_project_machine_name']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_project_machine_name']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['field_project_machine_name']['type'] = 'text_plain';
/* Field: Project */
$handler->display->display_options['fields']['title_1']['id'] = 'title_1';
$handler->display->display_options['fields']['title_1']['table'] = 'node';
$handler->display->display_options['fields']['title_1']['field'] = 'title';
$handler->display->display_options['fields']['title_1']['relationship'] = 'field_project_target_id';
$handler->display->display_options['fields']['title_1']['ui_name'] = 'Project';
$handler->display->display_options['fields']['title_1']['label'] = 'Project';
$handler->display->display_options['fields']['title_1']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['title_1']['alter']['path'] = 'project/issues/[field_project_machine_name]';
$handler->display->display_options['fields']['title_1']['link_to_node'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Summary';
/* Field: Content: Has new content */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'history';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = '';
/* Field: Content: Status */
$handler->display->display_options['fields']['field_issue_status']['id'] = 'field_issue_status';
$handler->display->display_options['fields']['field_issue_status']['table'] = 'field_data_field_issue_status';
$handler->display->display_options['fields']['field_issue_status']['field'] = 'field_issue_status';
/* Field: Content: Priority */
$handler->display->display_options['fields']['field_issue_priority']['id'] = 'field_issue_priority';
$handler->display->display_options['fields']['field_issue_priority']['table'] = 'field_data_field_issue_priority';
$handler->display->display_options['fields']['field_issue_priority']['field'] = 'field_issue_priority';
/* Field: Content: Category */
$handler->display->display_options['fields']['field_issue_category']['id'] = 'field_issue_category';
$handler->display->display_options['fields']['field_issue_category']['table'] = 'field_data_field_issue_category';
$handler->display->display_options['fields']['field_issue_category']['field'] = 'field_issue_category';
/* Field: Content: Version */
$handler->display->display_options['fields']['field_issue_version']['id'] = 'field_issue_version';
$handler->display->display_options['fields']['field_issue_version']['table'] = 'field_data_field_issue_version';
$handler->display->display_options['fields']['field_issue_version']['field'] = 'field_issue_version';
$handler->display->display_options['fields']['field_issue_version']['element_label_colon'] = FALSE;
/* Field: Content: Comment count */
$handler->display->display_options['fields']['comment_count']['id'] = 'comment_count';
$handler->display->display_options['fields']['comment_count']['table'] = 'node_comment_statistics';
$handler->display->display_options['fields']['comment_count']['field'] = 'comment_count';
$handler->display->display_options['fields']['comment_count']['label'] = 'Replies';
/* Field: Content: New comments */
$handler->display->display_options['fields']['new_comments']['id'] = 'new_comments';
$handler->display->display_options['fields']['new_comments']['table'] = 'node';
$handler->display->display_options['fields']['new_comments']['field'] = 'new_comments';
$handler->display->display_options['fields']['new_comments']['label'] = '';
$handler->display->display_options['fields']['new_comments']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['new_comments']['suffix'] = ' new';
/* Field: Content: Last comment time */
$handler->display->display_options['fields']['last_comment_timestamp']['id'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['last_comment_timestamp']['table'] = 'node_comment_statistics';
$handler->display->display_options['fields']['last_comment_timestamp']['field'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['last_comment_timestamp']['label'] = 'Last updated';
$handler->display->display_options['fields']['last_comment_timestamp']['date_format'] = 'raw time ago';
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'field_issue_assigned_target_id';
$handler->display->display_options['fields']['name']['label'] = 'Assigned to';
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = TRUE;
/* Field: Content: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['label'] = 'Created';
$handler->display->display_options['fields']['created']['date_format'] = 'raw time ago';
/* Field: Search: Score */
$handler->display->display_options['fields']['score']['id'] = 'score';
$handler->display->display_options['fields']['score']['table'] = 'search_index';
$handler->display->display_options['fields']['score']['field'] = 'score';
$handler->display->display_options['fields']['score']['set_precision'] = TRUE;
$handler->display->display_options['fields']['score']['precision'] = '3';
$handler->display->display_options['fields']['score']['alternate_sort'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['score']['alternate_order'] = 'desc';
/* Sort criterion: Content: Last comment time */
$handler->display->display_options['sorts']['last_comment_timestamp']['id'] = 'last_comment_timestamp';
$handler->display->display_options['sorts']['last_comment_timestamp']['table'] = 'node_comment_statistics';
$handler->display->display_options['sorts']['last_comment_timestamp']['field'] = 'last_comment_timestamp';
$handler->display->display_options['sorts']['last_comment_timestamp']['order'] = 'DESC';
/* Sort criterion: Search: Score */
$handler->display->display_options['sorts']['score']['id'] = 'score';
$handler->display->display_options['sorts']['score']['table'] = 'search_index';
$handler->display->display_options['sorts']['score']['field'] = 'score';
$handler->display->display_options['sorts']['score']['order'] = 'DESC';
/* Contextual filter: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['uid']['exception']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['uid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['uid']['title'] = 'Projects by %1';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
$handler->display->display_options['arguments']['uid']['validate_options']['type'] = 'either';
$handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
  2 => '2',
);
/* Filter criterion: Content: Published or admin */
$handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['table'] = 'node';
$handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['group'] = 1;
$handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['relationship'] = 'field_project_target_id';
$handler->display->display_options['filters']['project_type']['value'] = 'project';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type_1']['id'] = 'project_type_1';
$handler->display->display_options['filters']['project_type_1']['table'] = 'node';
$handler->display->display_options['filters']['project_type_1']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type_1']['value'] = 'project_issue';
$handler->display->display_options['filters']['project_type_1']['group'] = 1;
/* Filter criterion: Search: Search Terms */
$handler->display->display_options['filters']['keys']['id'] = 'keys';
$handler->display->display_options['filters']['keys']['table'] = 'search_index';
$handler->display->display_options['filters']['keys']['field'] = 'keys';
$handler->display->display_options['filters']['keys']['group'] = 1;
$handler->display->display_options['filters']['keys']['exposed'] = TRUE;
$handler->display->display_options['filters']['keys']['expose']['operator_id'] = 'keys_op';
$handler->display->display_options['filters']['keys']['expose']['label'] = 'Search for';
$handler->display->display_options['filters']['keys']['expose']['operator'] = 'keys_op';
$handler->display->display_options['filters']['keys']['expose']['identifier'] = 'text';
/* Filter criterion: Project */
$handler->display->display_options['filters']['field_project_target_id']['id'] = 'field_project_target_id';
$handler->display->display_options['filters']['field_project_target_id']['table'] = 'field_data_field_project';
$handler->display->display_options['filters']['field_project_target_id']['field'] = 'field_project_target_id';
$handler->display->display_options['filters']['field_project_target_id']['ui_name'] = 'Project';
$handler->display->display_options['filters']['field_project_target_id']['value'] = '';
$handler->display->display_options['filters']['field_project_target_id']['group'] = 1;
$handler->display->display_options['filters']['field_project_target_id']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_project_target_id']['expose']['operator_id'] = 'field_project_target_id_op';
$handler->display->display_options['filters']['field_project_target_id']['expose']['label'] = 'Project';
$handler->display->display_options['filters']['field_project_target_id']['expose']['operator'] = 'field_project_target_id_op';
$handler->display->display_options['filters']['field_project_target_id']['expose']['identifier'] = 'projects';
$handler->display->display_options['filters']['field_project_target_id']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['field_project_target_id']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['filters']['field_project_target_id']['widget'] = 'select';
$handler->display->display_options['filters']['field_project_target_id']['project_source'] = 'owner';
$handler->display->display_options['filters']['field_project_target_id']['project_uid_argument'] = 'uid';
/* Filter criterion: Status */
$handler->display->display_options['filters']['field_issue_status_value']['id'] = 'field_issue_status_value';
$handler->display->display_options['filters']['field_issue_status_value']['table'] = 'field_data_field_issue_status';
$handler->display->display_options['filters']['field_issue_status_value']['field'] = 'field_issue_status_value';
$handler->display->display_options['filters']['field_issue_status_value']['ui_name'] = 'Status';
$handler->display->display_options['filters']['field_issue_status_value']['value'] = array(
  'Open' => 'Open',
);
$handler->display->display_options['filters']['field_issue_status_value']['group'] = 1;
$handler->display->display_options['filters']['field_issue_status_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_issue_status_value']['expose']['operator_id'] = 'field_issue_status_value_op';
$handler->display->display_options['filters']['field_issue_status_value']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['field_issue_status_value']['expose']['operator'] = 'field_issue_status_value_op';
$handler->display->display_options['filters']['field_issue_status_value']['expose']['identifier'] = 'status';
$handler->display->display_options['filters']['field_issue_status_value']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['field_issue_status_value']['expose']['remember_roles'] = array(
  2 => '2',
);
/* Filter criterion: Priority */
$handler->display->display_options['filters']['field_issue_priority_value']['id'] = 'field_issue_priority_value';
$handler->display->display_options['filters']['field_issue_priority_value']['table'] = 'field_data_field_issue_priority';
$handler->display->display_options['filters']['field_issue_priority_value']['field'] = 'field_issue_priority_value';
$handler->display->display_options['filters']['field_issue_priority_value']['ui_name'] = 'Priority';
$handler->display->display_options['filters']['field_issue_priority_value']['group'] = 1;
$handler->display->display_options['filters']['field_issue_priority_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_issue_priority_value']['expose']['operator_id'] = 'field_issue_priority_value_op';
$handler->display->display_options['filters']['field_issue_priority_value']['expose']['label'] = 'Priority';
$handler->display->display_options['filters']['field_issue_priority_value']['expose']['operator'] = 'field_issue_priority_value_op';
$handler->display->display_options['filters']['field_issue_priority_value']['expose']['identifier'] = 'priorities';
$handler->display->display_options['filters']['field_issue_priority_value']['expose']['multiple'] = TRUE;
/* Filter criterion: Category */
$handler->display->display_options['filters']['field_issue_category_value']['id'] = 'field_issue_category_value';
$handler->display->display_options['filters']['field_issue_category_value']['table'] = 'field_data_field_issue_category';
$handler->display->display_options['filters']['field_issue_category_value']['field'] = 'field_issue_category_value';
$handler->display->display_options['filters']['field_issue_category_value']['ui_name'] = 'Category';
$handler->display->display_options['filters']['field_issue_category_value']['group'] = 1;
$handler->display->display_options['filters']['field_issue_category_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_issue_category_value']['expose']['operator_id'] = 'field_issue_category_value_op';
$handler->display->display_options['filters']['field_issue_category_value']['expose']['label'] = 'Category';
$handler->display->display_options['filters']['field_issue_category_value']['expose']['operator'] = 'field_issue_category_value_op';
$handler->display->display_options['filters']['field_issue_category_value']['expose']['identifier'] = 'categories';
$handler->display->display_options['filters']['field_issue_category_value']['expose']['multiple'] = TRUE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['header'] = FALSE;
/* Header: Global: View (Views field view) */
$handler->display->display_options['header']['view_field']['id'] = 'view_field';
$handler->display->display_options['header']['view_field']['table'] = 'views';
$handler->display->display_options['header']['view_field']['field'] = 'view_field';
$handler->display->display_options['header']['view_field']['label'] = 'My Projects';
$handler->display->display_options['header']['view_field']['element_label_colon'] = FALSE;
$handler->display->display_options['header']['view_field']['view'] = 'project_issue_user_projects';
$handler->display->display_options['header']['view_field']['display'] = 'my_projects_list';
$handler->display->display_options['header']['view_field']['arguments'] = '!1';
$handler->display->display_options['path'] = 'project/user';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'My projects';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;

/* Display: Feed */
$handler = $view->new_display('feed', 'Feed', 'feed_1');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['cache'] = FALSE;
$handler->display->display_options['cache']['type'] = 'time';
$handler->display->display_options['cache']['results_lifespan'] = '300';
$handler->display->display_options['cache']['output_lifespan'] = '300';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'rss';
$handler->display->display_options['row_plugin'] = 'node_rss';
$handler->display->display_options['path'] = 'project/user/%/feed';
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'default' => 0,
);

/* Display: My Projects header */
$handler = $view->new_display('attachment', 'My Projects header', 'my_projects_list');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'field_project_type',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'field_project_machine_name' => 'field_project_machine_name',
  'nid_1' => 'nid_1',
  'title' => 'title',
  'last_comment_timestamp' => 'last_comment_timestamp',
  'open_field_field_issue_status' => 'open_field_field_issue_status',
  'nothing' => 'nothing',
  'nothing_1' => 'nothing_1',
  'nothing_2' => 'nothing_2',
  'nothing_3' => 'nothing_3',
  'edit_node' => 'edit_node',
  'nothing_4' => 'nothing_4',
  'nothing_5' => 'nothing_5',
  'field_project_type' => 'field_project_type',
);
$handler->display->display_options['style_options']['default'] = 'title';
$handler->display->display_options['style_options']['info'] = array(
  'field_project_machine_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nid_1' => array(
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
  'last_comment_timestamp' => array(
    'sortable' => 1,
    'default_sort_order' => 'desc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'open_field_field_issue_status' => array(
    'sortable' => 0,
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
  'nothing_1' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing_2' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing_3' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'edit_node' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing_4' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nothing_5' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_project_type' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Content: Author */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['label'] = 'Project author';
/* Relationship: Issues */
$handler->display->display_options['relationships']['reverse_field_project_node']['id'] = 'reverse_field_project_node';
$handler->display->display_options['relationships']['reverse_field_project_node']['table'] = 'node';
$handler->display->display_options['relationships']['reverse_field_project_node']['field'] = 'reverse_field_project_node';
$handler->display->display_options['relationships']['reverse_field_project_node']['ui_name'] = 'Issues';
$handler->display->display_options['relationships']['reverse_field_project_node']['label'] = 'Issues';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Short name */
$handler->display->display_options['fields']['field_project_machine_name']['id'] = 'field_project_machine_name';
$handler->display->display_options['fields']['field_project_machine_name']['table'] = 'field_data_field_project_machine_name';
$handler->display->display_options['fields']['field_project_machine_name']['field'] = 'field_project_machine_name';
$handler->display->display_options['fields']['field_project_machine_name']['label'] = '';
$handler->display->display_options['fields']['field_project_machine_name']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_project_machine_name']['element_label_colon'] = FALSE;
/* Field: Content: Nid */
$handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
$handler->display->display_options['fields']['nid_1']['table'] = 'node';
$handler->display->display_options['fields']['nid_1']['field'] = 'nid';
$handler->display->display_options['fields']['nid_1']['label'] = '';
$handler->display->display_options['fields']['nid_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = 'Project';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Content: Last comment time */
$handler->display->display_options['fields']['last_comment_timestamp']['id'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['last_comment_timestamp']['table'] = 'node_comment_statistics';
$handler->display->display_options['fields']['last_comment_timestamp']['field'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['last_comment_timestamp']['relationship'] = 'reverse_field_project_node';
$handler->display->display_options['fields']['last_comment_timestamp']['group_type'] = 'max';
$handler->display->display_options['fields']['last_comment_timestamp']['label'] = 'Last issue update';
$handler->display->display_options['fields']['last_comment_timestamp']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['last_comment_timestamp']['date_format'] = 'raw time ago';
/* Field: Content: Open issue count */
$handler->display->display_options['fields']['open_field_field_issue_status']['id'] = 'open_field_field_issue_status';
$handler->display->display_options['fields']['open_field_field_issue_status']['table'] = 'field_data_field_issue_status';
$handler->display->display_options['fields']['open_field_field_issue_status']['field'] = 'open_field_field_issue_status';
$handler->display->display_options['fields']['open_field_field_issue_status']['relationship'] = 'reverse_field_project_node';
$handler->display->display_options['fields']['open_field_field_issue_status']['group_type'] = 'sum';
$handler->display->display_options['fields']['open_field_field_issue_status']['label'] = 'Open issues';
/* Field: Queue view link */
$handler->display->display_options['fields']['nothing']['id'] = 'nothing';
$handler->display->display_options['fields']['nothing']['table'] = 'views';
$handler->display->display_options['fields']['nothing']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing']['ui_name'] = 'Queue view link';
$handler->display->display_options['fields']['nothing']['label'] = '';
$handler->display->display_options['fields']['nothing']['exclude'] = TRUE;
$handler->display->display_options['fields']['nothing']['alter']['text'] = 'View';
$handler->display->display_options['fields']['nothing']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['nothing']['alter']['path'] = 'project/issues/[field_project_machine_name]';
$handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
/* Field: Queue search link */
$handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
$handler->display->display_options['fields']['nothing_1']['table'] = 'views';
$handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_1']['ui_name'] = 'Queue search link';
$handler->display->display_options['fields']['nothing_1']['label'] = '';
$handler->display->display_options['fields']['nothing_1']['exclude'] = TRUE;
$handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'Search';
$handler->display->display_options['fields']['nothing_1']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['nothing_1']['alter']['path'] = 'project/issues/search/[field_project_machine_name]';
$handler->display->display_options['fields']['nothing_1']['element_label_colon'] = FALSE;
/* Field: Create issue link */
$handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
$handler->display->display_options['fields']['nothing_2']['table'] = 'views';
$handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_2']['ui_name'] = 'Create issue link';
$handler->display->display_options['fields']['nothing_2']['label'] = '';
$handler->display->display_options['fields']['nothing_2']['exclude'] = TRUE;
$handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Create';
$handler->display->display_options['fields']['nothing_2']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['nothing_2']['alter']['path'] = 'node/add/project-issue/[field_project_machine_name]';
$handler->display->display_options['fields']['nothing_2']['element_label_colon'] = FALSE;
/* Field: Issue links */
$handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
$handler->display->display_options['fields']['nothing_3']['table'] = 'views';
$handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_3']['ui_name'] = 'Issue links';
$handler->display->display_options['fields']['nothing_3']['label'] = 'Issue links';
$handler->display->display_options['fields']['nothing_3']['alter']['text'] = '[nothing] &nbsp; [nothing_1] &nbsp; [nothing_2]';
$handler->display->display_options['fields']['nothing_3']['element_label_colon'] = FALSE;
/* Field: Project edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'views_entity_node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['ui_name'] = 'Project edit link';
$handler->display->display_options['fields']['edit_node']['label'] = '';
$handler->display->display_options['fields']['edit_node']['exclude'] = TRUE;
$handler->display->display_options['fields']['edit_node']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['edit_node']['text'] = 'Edit';
/* Field: Project links */
$handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
$handler->display->display_options['fields']['nothing_5']['table'] = 'views';
$handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
$handler->display->display_options['fields']['nothing_5']['ui_name'] = 'Project links';
$handler->display->display_options['fields']['nothing_5']['label'] = 'Project links';
$handler->display->display_options['fields']['nothing_5']['alter']['text'] = '[edit_node]';
$handler->display->display_options['fields']['nothing_5']['element_label_colon'] = FALSE;
/* Field: Content: Project type */
$handler->display->display_options['fields']['field_project_type']['id'] = 'field_project_type';
$handler->display->display_options['fields']['field_project_type']['table'] = 'field_data_field_project_type';
$handler->display->display_options['fields']['field_project_type']['field'] = 'field_project_type';
$handler->display->display_options['fields']['field_project_type']['label'] = '';
$handler->display->display_options['fields']['field_project_type']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_project_type']['element_label_colon'] = FALSE;
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Short name (field_project_machine_name) */
$handler->display->display_options['sorts']['field_project_machine_name_value']['id'] = 'field_project_machine_name_value';
$handler->display->display_options['sorts']['field_project_machine_name_value']['table'] = 'field_data_field_project_machine_name';
$handler->display->display_options['sorts']['field_project_machine_name_value']['field'] = 'field_project_machine_name_value';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['uid']['exception']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['uid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['uid']['title'] = 'Projects by %1';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate']['type'] = 'user';
$handler->display->display_options['arguments']['uid']['validate_options']['type'] = 'either';
$handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
$handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
  2 => '2',
);
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published or admin */
$handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['table'] = 'node';
$handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['group'] = 1;
$handler->display->display_options['filters']['status_extra']['expose']['operator'] = FALSE;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Project */
$handler->display->display_options['filters']['field_project_target_id']['id'] = 'field_project_target_id';
$handler->display->display_options['filters']['field_project_target_id']['table'] = 'field_data_field_project';
$handler->display->display_options['filters']['field_project_target_id']['field'] = 'field_project_target_id';
$handler->display->display_options['filters']['field_project_target_id']['ui_name'] = 'Project';
$handler->display->display_options['filters']['field_project_target_id']['group'] = 1;
$handler->display->display_options['filters']['field_project_target_id']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_project_target_id']['expose']['operator_id'] = 'field_project_target_id_op';
$handler->display->display_options['filters']['field_project_target_id']['expose']['label'] = 'Project';
$handler->display->display_options['filters']['field_project_target_id']['expose']['operator'] = 'field_project_target_id_op';
$handler->display->display_options['filters']['field_project_target_id']['expose']['identifier'] = 'projects';
$handler->display->display_options['filters']['field_project_target_id']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['field_project_target_id']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['filters']['field_project_target_id']['widget'] = 'select';
$handler->display->display_options['filters']['field_project_target_id']['project_source'] = 'owner';
$handler->display->display_options['filters']['field_project_target_id']['project_uid_argument'] = 'uid';
$handler->display->display_options['displays'] = array(
  'default' => 0,
  'page_1' => 0,
);
