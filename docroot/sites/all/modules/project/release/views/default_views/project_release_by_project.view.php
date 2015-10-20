<?php

/**
 * @file
 * List of all releases associated with a particular project.
 */

$view = new view();
$view->name = 'project_release_by_project';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'node';
$view->human_name = 'Project releases by project';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Releases for [project]';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '20';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'node';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Content: Project (field_release_project) */
$handler->display->display_options['arguments']['field_release_project_target_id']['id'] = 'field_release_project_target_id';
$handler->display->display_options['arguments']['field_release_project_target_id']['table'] = 'field_data_field_release_project';
$handler->display->display_options['arguments']['field_release_project_target_id']['field'] = 'field_release_project_target_id';
$handler->display->display_options['arguments']['field_release_project_target_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['field_release_project_target_id']['title_enable'] = TRUE;
$handler->display->display_options['arguments']['field_release_project_target_id']['title'] = 'Releases for %1';
$handler->display->display_options['arguments']['field_release_project_target_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_release_project_target_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['field_release_project_target_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['field_release_project_target_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['field_release_project_target_id']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['field_release_project_target_id']['validate']['type'] = 'node';
/* Filter criterion: Content: Published or admin */
$handler->display->display_options['filters']['status_extra']['id'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['table'] = 'node';
$handler->display->display_options['filters']['status_extra']['field'] = 'status_extra';
$handler->display->display_options['filters']['status_extra']['group'] = 1;
/* Filter criterion: Project: Project system behavior */
$handler->display->display_options['filters']['project_type']['id'] = 'project_type';
$handler->display->display_options['filters']['project_type']['table'] = 'node';
$handler->display->display_options['filters']['project_type']['field'] = 'project_type';
$handler->display->display_options['filters']['project_type']['value'] = 'project_release';
$handler->display->display_options['filters']['project_type']['group'] = 1;
/* Filter criterion: Content: Core compatibility (taxonomy_vocabulary_6) */
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['id'] = 'taxonomy_vocabulary_6_tid';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['table'] = 'field_data_taxonomy_vocabulary_6';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['field'] = 'taxonomy_vocabulary_6_tid';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['group'] = 1;
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['exposed'] = TRUE;
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['expose']['operator_id'] = 'taxonomy_vocabulary_6_tid_op';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['expose']['label'] = 'API version';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['expose']['operator'] = 'taxonomy_vocabulary_6_tid_op';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['expose']['identifier'] = 'api_version';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['expose']['multiple'] = TRUE;
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['expose']['remember_roles'] = array(
  2 => 0,
  3 => 0,
  1 => 0,
  34 => 0,
  32 => 0,
  16 => 0,
  30 => 0,
  22 => 0,
  20 => 0,
  24 => 0,
  12 => 0,
  36 => 0,
  28 => 0,
  26 => 0,
  4 => 0,
  7 => 0,
  14 => 0,
  38 => 0,
);
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['type'] = 'select';
$handler->display->display_options['filters']['taxonomy_vocabulary_6_tid']['vocabulary'] = 'vocabulary_6';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['path'] = 'node/%/release';

/* Display: Feed */
$handler = $view->new_display('feed', 'Feed', 'feed');
$handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['style_plugin'] = 'rss';
$handler->display->display_options['row_plugin'] = 'node_rss';
$handler->display->display_options['row_options']['item_length'] = 'rss';
$handler->display->display_options['path'] = 'node/%/release/feed';
$handler->display->display_options['displays'] = array(
  'default' => 'default',
  'page' => 'page',
);
