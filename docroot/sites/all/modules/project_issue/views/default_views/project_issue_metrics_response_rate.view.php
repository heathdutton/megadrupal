<?php

$view = new view();
$view->name = 'project_issue_metrics_response_rate';
$view->description = '';
$view->tag = 'Project issue';
$view->base_table = 'sampler_project_issue_responses_by_project';
$view->human_name = 'Project issue metric: response rate';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Response rate';
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
  'value_response_rate' => array(
    'display' => 'display',
    'aggregate' => '_none',
    'type' => 'column',
  ),
);
/* Footer: Global: Unfiltered text */
$handler->display->display_options['footer']['area_text_custom']['id'] = 'area_text_custom';
$handler->display->display_options['footer']['area_text_custom']['table'] = 'views';
$handler->display->display_options['footer']['area_text_custom']['field'] = 'area_text_custom';
$handler->display->display_options['footer']['area_text_custom']['content'] = '%';
/* Field: Response rate and time by project: Timestamp */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'sampler_project_issue_responses_by_project';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
$handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'c';
$handler->display->display_options['fields']['timestamp']['second_date_format'] = 'long';
$handler->display->display_options['fields']['timestamp']['timezone'] = 'UTC';
/* Field: Response rate and time by project: value_response_rate */
$handler->display->display_options['fields']['value_response_rate']['id'] = 'value_response_rate';
$handler->display->display_options['fields']['value_response_rate']['table'] = 'sampler_project_issue_responses_by_project';
$handler->display->display_options['fields']['value_response_rate']['field'] = 'value_response_rate';
$handler->display->display_options['fields']['value_response_rate']['separator'] = '';
/* Sort criterion: Response rate and time by project: Timestamp */
$handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['sorts']['timestamp']['table'] = 'sampler_project_issue_responses_by_project';
$handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
/* Contextual filter: Response rate and time by project: Object ID */
$handler->display->display_options['arguments']['object_id']['id'] = 'object_id';
$handler->display->display_options['arguments']['object_id']['table'] = 'sampler_project_issue_responses_by_project';
$handler->display->display_options['arguments']['object_id']['field'] = 'object_id';
$handler->display->display_options['arguments']['object_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['object_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['object_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['object_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['object_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['object_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['object_id']['validate']['type'] = 'project_nid';
/* Filter criterion: Response rate and time by project: Timestamp */
$handler->display->display_options['filters']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['table'] = 'sampler_project_issue_responses_by_project';
$handler->display->display_options['filters']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['filters']['timestamp']['operator'] = '>=';
$handler->display->display_options['filters']['timestamp']['value']['value'] = '-2 years';
$handler->display->display_options['filters']['timestamp']['value']['type'] = 'offset';
