<?php

/**
 * @file
 * Advanced issue search for a specific project.
 */
$view = new view();
$view->name = 'project_issue_search_project';
$view->description = 'Project issue advanced search for specific projects';
$view->tag = 'Project issue';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'Search issues for a project';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
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
  'title' => 'title',
  'timestamp' => 'title',
  'field_issue_status' => 'field_issue_status',
  'field_issue_priority' => 'field_issue_priority',
  'field_issue_category' => 'field_issue_category',
  'field_issue_version' => 'field_issue_version',
  'field_issue_component' => 'field_issue_component',
  'comment_count' => 'comment_count',
  'new_comments' => 'comment_count',
  'last_comment_timestamp' => 'last_comment_timestamp',
  'name' => 'name',
  'created' => 'created',
  'score' => 'score',
);
$handler->display->display_options['style_options']['default'] = 'score';
$handler->display->display_options['style_options']['info'] = array(
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
  'field_issue_component' => array(
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
$handler->display->display_options['empty']['text']['id'] = 'text';
$handler->display->display_options['empty']['text']['table'] = 'views';
$handler->display->display_options['empty']['text']['field'] = 'area';
$handler->display->display_options['empty']['text']['content'] = 'No issues match your criteria.';
$handler->display->display_options['empty']['text']['format'] = '6';
/* Relationship: Assigned user */
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['id'] = 'field_issue_assigned_target_id';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['table'] = 'field_data_field_issue_assigned';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['field'] = 'field_issue_assigned_target_id';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['ui_name'] = 'Assigned user';
$handler->display->display_options['relationships']['field_issue_assigned_target_id']['label'] = 'Assigned user';
/* Relationship: Project node */
$handler->display->display_options['relationships']['field_project_target_id']['id'] = 'field_project_target_id';
$handler->display->display_options['relationships']['field_project_target_id']['table'] = 'field_data_field_project';
$handler->display->display_options['relationships']['field_project_target_id']['field'] = 'field_project_target_id';
$handler->display->display_options['relationships']['field_project_target_id']['ui_name'] = 'Project node';
$handler->display->display_options['relationships']['field_project_target_id']['label'] = 'Project node';
$handler->display->display_options['relationships']['field_project_target_id']['required'] = TRUE;
/* Relationship: Submitted by */
$handler->display->display_options['relationships']['uid']['id'] = 'uid';
$handler->display->display_options['relationships']['uid']['table'] = 'node';
$handler->display->display_options['relationships']['uid']['field'] = 'uid';
$handler->display->display_options['relationships']['uid']['ui_name'] = 'Submitted by';
$handler->display->display_options['relationships']['uid']['label'] = 'Submitted by';
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
/* Field: Component */
$handler->display->display_options['fields']['field_issue_component']['id'] = 'field_issue_component';
$handler->display->display_options['fields']['field_issue_component']['table'] = 'field_data_field_issue_component';
$handler->display->display_options['fields']['field_issue_component']['field'] = 'field_issue_component';
$handler->display->display_options['fields']['field_issue_component']['ui_name'] = 'Component';
/* Field: Content: Comment count */
$handler->display->display_options['fields']['comment_count']['id'] = 'comment_count';
$handler->display->display_options['fields']['comment_count']['table'] = 'node_comment_statistics';
$handler->display->display_options['fields']['comment_count']['field'] = 'comment_count';
$handler->display->display_options['fields']['comment_count']['label'] = 'Replies';
/* Field: Content: New comments */
$handler->display->display_options['fields']['new_comments']['id'] = 'new_comments';
$handler->display->display_options['fields']['new_comments']['table'] = 'node';
$handler->display->display_options['fields']['new_comments']['field'] = 'new_comments';
$handler->display->display_options['fields']['new_comments']['label'] = 'New replies';
$handler->display->display_options['fields']['new_comments']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['new_comments']['suffix'] = ' new';
/* Field: Content: Last comment time */
$handler->display->display_options['fields']['last_comment_timestamp']['id'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['last_comment_timestamp']['table'] = 'node_comment_statistics';
$handler->display->display_options['fields']['last_comment_timestamp']['field'] = 'last_comment_timestamp';
$handler->display->display_options['fields']['last_comment_timestamp']['label'] = 'Last updated';
$handler->display->display_options['fields']['last_comment_timestamp']['date_format'] = 'raw time ago';
/* Field: Assigned to */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['relationship'] = 'field_issue_assigned_target_id';
$handler->display->display_options['fields']['name']['ui_name'] = 'Assigned to';
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
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'field_project_target_id';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['nid']['title'] = 'Search issues for %1';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['nid']['validate']['type'] = 'project_nid_having_issue_queue';
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
$handler->display->display_options['filters']['project_type']['value'] = 'project_issue';
$handler->display->display_options['filters']['project_type']['group'] = 1;
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
/* Filter criterion: Assigned */
$handler->display->display_options['filters']['uid']['id'] = 'uid';
$handler->display->display_options['filters']['uid']['table'] = 'users';
$handler->display->display_options['filters']['uid']['field'] = 'uid';
$handler->display->display_options['filters']['uid']['relationship'] = 'field_issue_assigned_target_id';
$handler->display->display_options['filters']['uid']['ui_name'] = 'Assigned';
$handler->display->display_options['filters']['uid']['value'] = '';
$handler->display->display_options['filters']['uid']['group'] = 1;
$handler->display->display_options['filters']['uid']['exposed'] = TRUE;
$handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
$handler->display->display_options['filters']['uid']['expose']['label'] = 'Assigned';
$handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
$handler->display->display_options['filters']['uid']['expose']['identifier'] = 'assigned';
/* Filter criterion: Submitted by */
$handler->display->display_options['filters']['uid_1']['id'] = 'uid_1';
$handler->display->display_options['filters']['uid_1']['table'] = 'users';
$handler->display->display_options['filters']['uid_1']['field'] = 'uid';
$handler->display->display_options['filters']['uid_1']['relationship'] = 'uid';
$handler->display->display_options['filters']['uid_1']['ui_name'] = 'Submitted by';
$handler->display->display_options['filters']['uid_1']['value'] = '';
$handler->display->display_options['filters']['uid_1']['group'] = 1;
$handler->display->display_options['filters']['uid_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['uid_1']['expose']['operator_id'] = 'uid_1_op';
$handler->display->display_options['filters']['uid_1']['expose']['label'] = 'Submitted by';
$handler->display->display_options['filters']['uid_1']['expose']['operator'] = 'uid_1_op';
$handler->display->display_options['filters']['uid_1']['expose']['identifier'] = 'submitted';
/* Filter criterion: Content: User posted or commented */
$handler->display->display_options['filters']['uid_touch']['id'] = 'uid_touch';
$handler->display->display_options['filters']['uid_touch']['table'] = 'node';
$handler->display->display_options['filters']['uid_touch']['field'] = 'uid_touch';
$handler->display->display_options['filters']['uid_touch']['value'] = '';
$handler->display->display_options['filters']['uid_touch']['group'] = 1;
$handler->display->display_options['filters']['uid_touch']['exposed'] = TRUE;
$handler->display->display_options['filters']['uid_touch']['expose']['operator_id'] = 'uid_touch_op';
$handler->display->display_options['filters']['uid_touch']['expose']['label'] = 'Participant';
$handler->display->display_options['filters']['uid_touch']['expose']['operator'] = 'uid_touch_op';
$handler->display->display_options['filters']['uid_touch']['expose']['identifier'] = 'participant';
/* Filter criterion: Status */
$handler->display->display_options['filters']['field_issue_status_value']['id'] = 'field_issue_status_value';
$handler->display->display_options['filters']['field_issue_status_value']['table'] = 'field_data_field_issue_status';
$handler->display->display_options['filters']['field_issue_status_value']['field'] = 'field_issue_status_value';
$handler->display->display_options['filters']['field_issue_status_value']['ui_name'] = 'Status';
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
$handler->display->display_options['filters']['field_issue_priority_value']['expose']['remember_roles'] = array(
  2 => '2',
);
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
$handler->display->display_options['filters']['field_issue_category_value']['expose']['remember_roles'] = array(
  2 => '2',
);
/* Filter criterion: Content: Version (field_issue_version) */
$handler->display->display_options['filters']['field_issue_version_value']['id'] = 'field_issue_version_value';
$handler->display->display_options['filters']['field_issue_version_value']['table'] = 'field_data_field_issue_version';
$handler->display->display_options['filters']['field_issue_version_value']['field'] = 'field_issue_version_value';
$handler->display->display_options['filters']['field_issue_version_value']['group'] = 1;
$handler->display->display_options['filters']['field_issue_version_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_issue_version_value']['expose']['operator_id'] = 'field_issue_version_value_op';
$handler->display->display_options['filters']['field_issue_version_value']['expose']['label'] = 'Version';
$handler->display->display_options['filters']['field_issue_version_value']['expose']['operator'] = 'field_issue_version_value_op';
$handler->display->display_options['filters']['field_issue_version_value']['expose']['identifier'] = 'field_issue_version_value';
$handler->display->display_options['filters']['field_issue_version_value']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['field_issue_version_value']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['filters']['field_issue_version_value']['project_argument'] = 'nid';
/* Filter criterion: Component */
$handler->display->display_options['filters']['field_issue_component_value']['id'] = 'field_issue_component_value';
$handler->display->display_options['filters']['field_issue_component_value']['table'] = 'field_data_field_issue_component';
$handler->display->display_options['filters']['field_issue_component_value']['field'] = 'field_issue_component_value';
$handler->display->display_options['filters']['field_issue_component_value']['ui_name'] = 'Component';
$handler->display->display_options['filters']['field_issue_component_value']['group'] = 1;
$handler->display->display_options['filters']['field_issue_component_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_issue_component_value']['expose']['operator_id'] = 'field_issue_component_value_op';
$handler->display->display_options['filters']['field_issue_component_value']['expose']['label'] = 'Component';
$handler->display->display_options['filters']['field_issue_component_value']['expose']['operator'] = 'field_issue_component_value_op';
$handler->display->display_options['filters']['field_issue_component_value']['expose']['identifier'] = 'component';
$handler->display->display_options['filters']['field_issue_component_value']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['field_issue_component_value']['expose']['remember_roles'] = array(
  2 => '2',
);
$handler->display->display_options['filters']['field_issue_component_value']['project_argument'] = 'nid';

/* Filter criteria: Issue queue enabled taxonomy vocabularies. */
_project_issue_views_add_taxonomy_filters($handler);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['path'] = 'project/issues/search/%';
$handler->display->display_options['menu']['title'] = 'Advanced search';
$handler->display->display_options['menu']['weight'] = '0';

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
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: Content: Last comment time */
$handler->display->display_options['sorts']['last_comment_timestamp']['id'] = 'last_comment_timestamp';
$handler->display->display_options['sorts']['last_comment_timestamp']['table'] = 'node_comment_statistics';
$handler->display->display_options['sorts']['last_comment_timestamp']['field'] = 'last_comment_timestamp';
$handler->display->display_options['sorts']['last_comment_timestamp']['order'] = 'DESC';
$handler->display->display_options['path'] = 'project/issues/search/%/rss';
$handler->display->display_options['menu']['name'] = '';
$handler->display->display_options['displays'] = array(
  'page_1' => 'page_1',
  'default' => 0,
);
