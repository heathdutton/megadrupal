<?php
/**
 * @file
 * Default views for PM Gantt.
 */

/**
 * Implements hook_views_default_views().
 */
function pmgantt_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'pmgantt';
  $view->description = 'Provides page and block Gantt chart views for Drupal PM.';
  $view->tag = 'pm';
  $view->base_table = 'node';
  $view->human_name = 'PM Gantt';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Gantt Chart';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'Project Management Task: access';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'gantt';
  $handler->display->display_options['style_options']['id_field'] = 'nid';
  $handler->display->display_options['style_options']['name_field'] = 'title';
  $handler->display->display_options['style_options']['date_field'] = 'pm_date';
  $handler->display->display_options['style_options']['end_date_field'] = 'pm_date';
  $handler->display->display_options['style_options']['progress_field'] = 'expression';
  $handler->display->display_options['style_options']['project_id_field'] = 'nid_1';
  $handler->display->display_options['style_options']['project_date_field'] = 'pm_date_1';
  $handler->display->display_options['style_options']['columns'] = array(
    '' => '',
  );
  $handler->display->display_options['style_options']['task_type'] = 'pmtask';
  $handler->display->display_options['style_options']['scale'] = '2';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['pmtask_parent_target_id']['id'] = 'pmtask_parent_target_id';
  $handler->display->display_options['relationships']['pmtask_parent_target_id']['table'] = 'field_data_pmtask_parent';
  $handler->display->display_options['relationships']['pmtask_parent_target_id']['field'] = 'pmtask_parent_target_id';
  $handler->display->display_options['relationships']['pmtask_parent_target_id']['label'] = 'pmtask_parent';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Task title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['ui_name'] = 'Task title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Task date */
  $handler->display->display_options['fields']['pm_date']['id'] = 'pm_date';
  $handler->display->display_options['fields']['pm_date']['table'] = 'field_data_pm_date';
  $handler->display->display_options['fields']['pm_date']['field'] = 'pm_date';
  $handler->display->display_options['fields']['pm_date']['ui_name'] = 'Task date';
  $handler->display->display_options['fields']['pm_date']['label'] = '';
  $handler->display->display_options['fields']['pm_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pm_date']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Field: Progress */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['ui_name'] = 'Progress';
  $handler->display->display_options['fields']['expression']['label'] = '';
  $handler->display->display_options['fields']['expression']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['expression'] = '0';
  /* Field: Project nid */
  $handler->display->display_options['fields']['nid_1']['id'] = 'nid_1';
  $handler->display->display_options['fields']['nid_1']['table'] = 'node';
  $handler->display->display_options['fields']['nid_1']['field'] = 'nid';
  $handler->display->display_options['fields']['nid_1']['relationship'] = 'pmtask_parent_target_id';
  $handler->display->display_options['fields']['nid_1']['ui_name'] = 'Project nid';
  $handler->display->display_options['fields']['nid_1']['label'] = '';
  $handler->display->display_options['fields']['nid_1']['element_label_colon'] = FALSE;
  /* Field: Project title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'pmtask_parent_target_id';
  $handler->display->display_options['fields']['title_1']['ui_name'] = 'Project title';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  /* Field: Project date */
  $handler->display->display_options['fields']['pm_date_1']['id'] = 'pm_date_1';
  $handler->display->display_options['fields']['pm_date_1']['table'] = 'field_data_pm_date';
  $handler->display->display_options['fields']['pm_date_1']['field'] = 'pm_date';
  $handler->display->display_options['fields']['pm_date_1']['relationship'] = 'pmtask_parent_target_id';
  $handler->display->display_options['fields']['pm_date_1']['ui_name'] = 'Project date';
  $handler->display->display_options['fields']['pm_date_1']['label'] = '';
  $handler->display->display_options['fields']['pm_date_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pm_date_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  /* Sort criterion: Content: Date -  start date (pm_date) */
  $handler->display->display_options['sorts']['pm_date_value']['id'] = 'pm_date_value';
  $handler->display->display_options['sorts']['pm_date_value']['table'] = 'field_data_pm_date';
  $handler->display->display_options['sorts']['pm_date_value']['field'] = 'pm_date_value';
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
    'pmtask' => 'pmtask',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
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
    'pmtask' => 'pmtask',
  );
  /* Filter criterion: Content: Title */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'node';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['relationship'] = 'pmtask_parent_target_id';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Project';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
  );
  $handler->display->display_options['path'] = 'pm/tasks/gantt';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Gantt Chart';
  $handler->display->display_options['menu']['description'] = 'View tasks on a Gantt chart';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'project_nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
    'pmproject' => 'pmproject',
  );
  $handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
  $handler->display->display_options['block_description'] = 'Project Gantt Chart';

  $views[$view->name] = $view;

  return $views;
}
