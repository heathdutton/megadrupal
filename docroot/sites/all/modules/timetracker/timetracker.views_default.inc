<?php
/**
 * @file
 * timetracker.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function timetracker_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'field_timetrack_user_filter';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Field User filter';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';

  /* Display: Entity Reference */
  $handler = $view->new_display('entityreference', 'Entity Reference', 'entityreference_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'entityreference_style';
  $handler->display->display_options['style_options']['search_fields'] = array(
    'name' => 'name',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'entityreference_fields';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $export['field_timetrack_user_filter'] = $view;

  $view = new view;
  $view->name = 'time_report';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'time_entry';
  $view->human_name = 'Time report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Time report';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => 'time',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['columns'] = array(
    'time' => 'time',
    'url' => 'url',
    'field_project_ref' => 'field_project_ref',
    'field_casetracker_case_title' => 'field_casetracker_case_title',
    'duration' => 'duration',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'time' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'url' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_project_ref' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_casetracker_case_title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'duration' => array(
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
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_timetrack_case_target_id']['id'] = 'field_timetrack_case_target_id';
  $handler->display->display_options['relationships']['field_timetrack_case_target_id']['table'] = 'field_data_field_timetrack_case';
  $handler->display->display_options['relationships']['field_timetrack_case_target_id']['field'] = 'field_timetrack_case_target_id';
  $handler->display->display_options['relationships']['field_timetrack_case_target_id']['required'] = 1;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_timetrack_user_target_id']['id'] = 'field_timetrack_user_target_id';
  $handler->display->display_options['relationships']['field_timetrack_user_target_id']['table'] = 'field_data_field_timetrack_user';
  $handler->display->display_options['relationships']['field_timetrack_user_target_id']['field'] = 'field_timetrack_user_target_id';
  $handler->display->display_options['relationships']['field_timetrack_user_target_id']['required'] = 1;
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['field_project_ref_target_id']['id'] = 'field_project_ref_target_id';
  $handler->display->display_options['relationships']['field_project_ref_target_id']['table'] = 'field_data_field_project_ref';
  $handler->display->display_options['relationships']['field_project_ref_target_id']['field'] = 'field_project_ref_target_id';
  $handler->display->display_options['relationships']['field_project_ref_target_id']['relationship'] = 'field_timetrack_case_target_id';
  $handler->display->display_options['relationships']['field_project_ref_target_id']['label'] = 'Project';
  $handler->display->display_options['relationships']['field_project_ref_target_id']['required'] = 1;
  /* Field: Time Entry: Time */
  $handler->display->display_options['fields']['time']['id'] = 'time';
  $handler->display->display_options['fields']['time']['table'] = 'time_entry';
  $handler->display->display_options['fields']['time']['field'] = 'time';
  $handler->display->display_options['fields']['time']['label'] = '';
  $handler->display->display_options['fields']['time']['exclude'] = TRUE;
  $handler->display->display_options['fields']['time']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['time']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['time']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['time']['alter']['external'] = 0;
  $handler->display->display_options['fields']['time']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['time']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['time']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['time']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['time']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['time']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['time']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['time']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['time']['alter']['html'] = 0;
  $handler->display->display_options['fields']['time']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['time']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['time']['hide_empty'] = 0;
  $handler->display->display_options['fields']['time']['empty_zero'] = 0;
  $handler->display->display_options['fields']['time']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['time']['date_format'] = 'custom';
  $handler->display->display_options['fields']['time']['custom_date_format'] = 'l, M jS';
  /* Field: Case: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_casetracker_case';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['relationship'] = 'field_timetrack_case_target_id';
  $handler->display->display_options['fields']['url']['label'] = '';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['url']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['url']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['url']['alter']['external'] = 0;
  $handler->display->display_options['fields']['url']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['url']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['url']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['url']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['url']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['url']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['url']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['url']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['url']['alter']['html'] = 0;
  $handler->display->display_options['fields']['url']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['url']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['url']['hide_empty'] = 0;
  $handler->display->display_options['fields']['url']['empty_zero'] = 0;
  $handler->display->display_options['fields']['url']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['url']['display_as_link'] = 1;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: Case: Project */
  $handler->display->display_options['fields']['field_project_ref']['id'] = 'field_project_ref';
  $handler->display->display_options['fields']['field_project_ref']['table'] = 'field_data_field_project_ref';
  $handler->display->display_options['fields']['field_project_ref']['field'] = 'field_project_ref';
  $handler->display->display_options['fields']['field_project_ref']['relationship'] = 'field_timetrack_case_target_id';
  /* Field: Case: Title */
  $handler->display->display_options['fields']['field_casetracker_case_title']['id'] = 'field_casetracker_case_title';
  $handler->display->display_options['fields']['field_casetracker_case_title']['table'] = 'field_data_field_casetracker_case_title';
  $handler->display->display_options['fields']['field_casetracker_case_title']['field'] = 'field_casetracker_case_title';
  $handler->display->display_options['fields']['field_casetracker_case_title']['relationship'] = 'field_timetrack_case_target_id';
  $handler->display->display_options['fields']['field_casetracker_case_title']['label'] = 'Task';
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['path'] = '[url]';
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_casetracker_case_title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_casetracker_case_title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_casetracker_case_title']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['field_casetracker_case_title']['field_api_classes'] = 0;
  /* Field: Time Entry: Duration */
  $handler->display->display_options['fields']['duration']['id'] = 'duration';
  $handler->display->display_options['fields']['duration']['table'] = 'time_entry';
  $handler->display->display_options['fields']['duration']['field'] = 'duration';
  $handler->display->display_options['fields']['duration']['group_type'] = 'sum';
  $handler->display->display_options['fields']['duration']['label'] = 'Time spent';
  $handler->display->display_options['fields']['duration']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['external'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['duration']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['duration']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['duration']['alter']['html'] = 0;
  $handler->display->display_options['fields']['duration']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['duration']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['duration']['hide_empty'] = 0;
  $handler->display->display_options['fields']['duration']['empty_zero'] = 0;
  $handler->display->display_options['fields']['duration']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['duration']['granularity'] = '2';
  /* Field: Time Entry: Time */
  $handler->display->display_options['fields']['time_1']['id'] = 'time_1';
  $handler->display->display_options['fields']['time_1']['table'] = 'time_entry';
  $handler->display->display_options['fields']['time_1']['field'] = 'time';
  $handler->display->display_options['fields']['time_1']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['time_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['time_1']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['time_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['time_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['time_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['time_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['time_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['time_1']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['time_1']['date_format'] = 'custom';
  $handler->display->display_options['fields']['time_1']['custom_date_format'] = 'c';
  /* Sort criterion: Time Entry: Time */
  $handler->display->display_options['sorts']['time']['id'] = 'time';
  $handler->display->display_options['sorts']['time']['table'] = 'time_entry';
  $handler->display->display_options['sorts']['time']['field'] = 'time';
  /* Contextual filter: Field: User (field_timetrack_user) */
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['id'] = 'field_timetrack_user_target_id';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['table'] = 'field_data_field_timetrack_user';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['field'] = 'field_timetrack_user_target_id';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['break_phrase'] = 1;
  $handler->display->display_options['arguments']['field_timetrack_user_target_id']['not'] = 0;
  /* Filter criterion: Time Entry: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'time_entry';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'timetrack' => 'timetrack',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Case: Title (field_casetracker_case_title) */
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['id'] = 'field_casetracker_case_title_value';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['table'] = 'field_data_field_casetracker_case_title';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['field'] = 'field_casetracker_case_title_value';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['relationship'] = 'field_timetrack_case_target_id';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['operator'] = 'starts';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['operator_id'] = 'field_casetracker_case_title_value_op';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['label'] = 'Case';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['operator'] = 'field_casetracker_case_title_value_op';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['identifier'] = 'field_casetracker_case_title_value';
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['remember'] = 1;
  $handler->display->display_options['filters']['field_casetracker_case_title_value']['expose']['multiple'] = FALSE;
  /* Filter criterion: Project: Title (field_casetracker_project_title) */
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['id'] = 'field_casetracker_project_title_value';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['table'] = 'field_data_field_casetracker_project_title';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['field'] = 'field_casetracker_project_title_value';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['relationship'] = 'field_project_ref_target_id';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['operator'] = 'starts';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['exposed'] = TRUE;
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['expose']['operator_id'] = 'field_casetracker_project_title_value_op';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['expose']['label'] = 'Project';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['expose']['operator'] = 'field_casetracker_project_title_value_op';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['expose']['identifier'] = 'field_casetracker_project_title_value';
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['expose']['required'] = 0;
  $handler->display->display_options['filters']['field_casetracker_project_title_value']['expose']['multiple'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'time-report/by-date';
  $export['time_report'] = $view;

  return $export;
}
