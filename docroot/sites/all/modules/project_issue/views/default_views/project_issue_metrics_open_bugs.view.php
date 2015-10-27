<?php

$view = new view();
$view->name = 'project_issue_metrics_open_bugs';
$view->description = '';
$view->tag = 'Project issue';
$view->base_table = 'sampler_project_issue_opened_vs_closed_by_category';
$view->human_name = 'Project issue metric: open bugs';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Open bugs';
$handler->display->display_options['css_class'] = 'project-issue-sparkline';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'd3_style';
$handler->display->display_options['style_options']['library'] = 'd3.sparkline';
$handler->display->display_options['style_options']['columns'] = array(
  'timestamp' => array(
    'display' => 'display',
    'aggregate' => '_none',
    'type' => 'row',
  ),
  'value_bug_open' => array(
    'display' => 'display',
    'aggregate' => '_none',
    'type' => 'column',
  ),
);
/* Field: Total open/closed issues: Timestamp */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'sampler_project_issue_opened_vs_closed_by_category';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
$handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'c';
$handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
$handler->display->display_options['fields']['timestamp']['timezone'] = 'UTC';
/* Field: Total open/closed issues: value_bug_open */
$handler->display->display_options['fields']['value_bug_open']['id'] = 'value_bug_open';
$handler->display->display_options['fields']['value_bug_open']['table'] = 'sampler_project_issue_opened_vs_closed_by_category';
$handler->display->display_options['fields']['value_bug_open']['field'] = 'value_bug_open';
$handler->display->display_options['fields']['value_bug_open']['separator'] = '';
/* Sort criterion: Total open/closed issues: Timestamp */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'sampler_project_issue_opened_vs_closed_by_category';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
/* Contextual filter: Total open/closed issues: Object ID */
$handler->display->display_options['arguments']['object_id']['id'] = 'object_id';
$handler->display->display_options['arguments']['object_id']['table'] = 'sampler_project_issue_opened_vs_closed_by_category';
$handler->display->display_options['arguments']['object_id']['field'] = 'object_id';
$handler->display->display_options['arguments']['object_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['object_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['object_id']['default_argument_skip_url'] = TRUE;
$handler->display->display_options['arguments']['object_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['object_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['object_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['object_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['object_id']['validate']['type'] = 'project_nid';
/* Filter criterion: Total open/closed issues: Timestamp */
$handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['table'] = 'sampler_project_issue_opened_vs_closed_by_category';
$handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['operator'] = '>=';
$handler->display->display_options['filters']['timestamp']['value']['value'] = '-2 years';
