<?php

/**
 * @file
 * Default views for the Wunderlist module.
 */

/**
 * Implements hook_views_default_views().
 */
function wunderlist_views_default_views() {
  $views = array();

  // Wunderlist list view.
  $view = new view();
  $view->name = 'wunderlist_list';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'wunderlist_list';
  $view->human_name = 'Wunderlist list';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Wunderlist';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access wunderlist content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Wunderlist: Wunderlist list id */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'wunderlist_list';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = '';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['id']['element_label_colon'] = FALSE;
  /* Field: Wunderlist: Wunderlist list title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'wunderlist_list';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'wunderlist/[id]';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'wunderlist';

  $views[$view->name] = $view;

  // Wunderlist tasks view.
  $view = new view();
  $view->name = 'wunderlist_tasks';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'wunderlist_task';
  $view->human_name = 'Wunderlist tasks';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Tasks';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access wunderlist content';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = '<h2>[title_1]</h2>';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['header']['area']['tokenize'] = TRUE;
  /* Relationship: Wunderlist task: Task author. */
  $handler->display->display_options['relationships']['created_by_id']['id'] = 'created_by_id';
  $handler->display->display_options['relationships']['created_by_id']['table'] = 'wunderlist_task';
  $handler->display->display_options['relationships']['created_by_id']['field'] = 'created_by_id';
  $handler->display->display_options['relationships']['created_by_id']['required'] = TRUE;
  /* Relationship: Wunderlist task: The list this task belongs to. */
  $handler->display->display_options['relationships']['list_id']['id'] = 'list_id';
  $handler->display->display_options['relationships']['list_id']['table'] = 'wunderlist_task';
  $handler->display->display_options['relationships']['list_id']['field'] = 'list_id';
  $handler->display->display_options['relationships']['list_id']['required'] = TRUE;
  /* Field: Wunderlist task: Wunderlist task id */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'wunderlist_task';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
  /* Field: Wunderlist user: Wunderlist user avatar */
  $handler->display->display_options['fields']['avatar']['id'] = 'avatar';
  $handler->display->display_options['fields']['avatar']['table'] = 'wunderlist_user';
  $handler->display->display_options['fields']['avatar']['field'] = 'avatar';
  $handler->display->display_options['fields']['avatar']['relationship'] = 'created_by_id';
  $handler->display->display_options['fields']['avatar']['label'] = '';
  $handler->display->display_options['fields']['avatar']['element_label_colon'] = FALSE;
  /* Field: Wunderlist task: Wunderlist task title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'wunderlist_task';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: Wunderlist: Wunderlist list title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'wunderlist_list';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'list_id';
  $handler->display->display_options['fields']['title_1']['label'] = '';
  $handler->display->display_options['fields']['title_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title_1']['element_label_colon'] = FALSE;
  /* Contextual filter: Wunderlist task: Wunderlist task list id */
  $handler->display->display_options['arguments']['list_id']['id'] = 'list_id';
  $handler->display->display_options['arguments']['list_id']['table'] = 'wunderlist_task';
  $handler->display->display_options['arguments']['list_id']['field'] = 'list_id';
  $handler->display->display_options['arguments']['list_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['list_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['list_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['list_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['list_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['list_id']['limit'] = '0';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'wunderlist/%';

  $views[$view->name] = $view;
  return $views;
}
