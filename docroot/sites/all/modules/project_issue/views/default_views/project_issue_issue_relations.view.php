<?php

/**
 * @file
 * Issues that are related to a specific issue.
 */

$view = new view();
$view->name = 'project_issue_issue_relations';
$view->description = 'Project issues that are related to a specific issue.';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Related issues';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['row_plugin'] = 'project_issue_node';
/* Sort criterion: Content: Nid */
$handler->display->display_options['sorts']['nid']['id'] = 'nid';
$handler->display->display_options['sorts']['nid']['table'] = 'node';
$handler->display->display_options['sorts']['nid']['field'] = 'nid';
/* Filter criterion: Content: Published or admin */
$handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['table'] = 'node';
$handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['group'] = 1;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project_issue';

/* Display: Block: Referenced by */
$handler = $view->new_display('block', 'Block: Referenced by', 'block_referenced');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Referenced by';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_issue_related_target_id']['id'] = 'field_issue_related_target_id';
$handler->display->display_options['relationships']['field_issue_related_target_id']['table'] = 'field_data_field_issue_related';
$handler->display->display_options['relationships']['field_issue_related_target_id']['field'] = 'field_issue_related_target_id';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'field_issue_related_target_id';
$handler->display->display_options['arguments']['nid']['default_action'] = 'default';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['nid']['validate']['type'] = 'project_issue_nid';
$handler->display->display_options['block_description'] = 'Project issue: Referenced by';

/* Display: Block: Child issues */
$handler = $view->new_display('block', 'Block: Child issues', 'block_child_issues');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Child issues';
$handler->display->display_options['defaults']['pager'] = FALSE;
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['defaults']['relationships'] = FALSE;
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_issue_parent_target_id']['id'] = 'field_issue_parent_target_id';
$handler->display->display_options['relationships']['field_issue_parent_target_id']['table'] = 'field_data_field_issue_parent';
$handler->display->display_options['relationships']['field_issue_parent_target_id']['field'] = 'field_issue_parent_target_id';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'field_issue_parent_target_id';
$handler->display->display_options['arguments']['nid']['default_action'] = 'default';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['nid']['validate']['type'] = 'project_issue_nid';
$handler->display->display_options['block_description'] = 'Project issue: Child issues';

/* Display: Block: Parent issue */
$handler = $view->new_display('block', 'Block: Parent issue', 'block_parent_issue');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Parent issue';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['style_options']['default_row_class'] = FALSE;
$handler->display->display_options['style_options']['row_class_special'] = FALSE;
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['hide_empty'] = TRUE;
$handler->display->display_options['row_options']['default_field_elements'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Parent issue */
$handler->display->display_options['fields']['field_issue_parent']['id'] = 'field_issue_parent';
$handler->display->display_options['fields']['field_issue_parent']['table'] = 'field_data_field_issue_parent';
$handler->display->display_options['fields']['field_issue_parent']['field'] = 'field_issue_parent';
$handler->display->display_options['fields']['field_issue_parent']['label'] = '';
$handler->display->display_options['fields']['field_issue_parent']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_issue_parent']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_issue_parent']['type'] = 'issue_id';
$handler->display->display_options['fields']['field_issue_parent']['settings'] = array(
  'link' => 0,
);
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'default';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published or admin */
$handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['table'] = 'node';
$handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['group'] = 1;
/* Filter criterion: Content: Parent issue (field_issue_parent) */
$handler->display->display_options['filters']['field_issue_parent_target_id']['id'] = 'field_issue_parent_target_id';
$handler->display->display_options['filters']['field_issue_parent_target_id']['table'] = 'field_data_field_issue_parent';
$handler->display->display_options['filters']['field_issue_parent_target_id']['field'] = 'field_issue_parent_target_id';
$handler->display->display_options['filters']['field_issue_parent_target_id']['operator'] = 'not empty';
$handler->display->display_options['filters']['field_issue_parent_target_id']['group'] = 1;
$handler->display->display_options['block_description'] = 'Project issue: Parent issue';

/* Display: Block: Related issues */
$handler = $view->new_display('block', 'Block: Related issues', 'block_related_issues');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Related issues';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['row_options']['hide_empty'] = TRUE;
$handler->display->display_options['row_options']['default_field_elements'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Content: Related issues */
$handler->display->display_options['fields']['field_issue_related']['id'] = 'field_issue_related';
$handler->display->display_options['fields']['field_issue_related']['table'] = 'field_data_field_issue_related';
$handler->display->display_options['fields']['field_issue_related']['field'] = 'field_issue_related';
$handler->display->display_options['fields']['field_issue_related']['label'] = '';
$handler->display->display_options['fields']['field_issue_related']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['field_issue_related']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['field_issue_related']['type'] = 'issue_id';
$handler->display->display_options['fields']['field_issue_related']['settings'] = array(
  'link' => 0,
);
$handler->display->display_options['fields']['field_issue_related']['group_rows'] = FALSE;
$handler->display->display_options['fields']['field_issue_related']['delta_offset'] = '0';
$handler->display->display_options['defaults']['sorts'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'default';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['defaults']['filter_groups'] = FALSE;
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter criterion: Content: Published or admin */
$handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['table'] = 'node';
$handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['group'] = 1;
/* Filter criterion: Content: Related issues (field_issue_related) */
$handler->display->display_options['filters']['field_issue_related_target_id']['id'] = 'field_issue_related_target_id';
$handler->display->display_options['filters']['field_issue_related_target_id']['table'] = 'field_data_field_issue_related';
$handler->display->display_options['filters']['field_issue_related_target_id']['field'] = 'field_issue_related_target_id';
$handler->display->display_options['filters']['field_issue_related_target_id']['operator'] = 'not empty';
$handler->display->display_options['filters']['field_issue_related_target_id']['group'] = 1;
$handler->display->display_options['block_description'] = 'Project issue: Related issues';
