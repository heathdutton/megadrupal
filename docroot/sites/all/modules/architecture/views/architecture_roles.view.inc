<?php
/**
 * @file
 * Defines a view for displaying roles.
 */

/*
 * View 'architecture_list_all_roles'
 */
$view = new view();
$view->name = 'architecture_list_all_roles';
$view->description = 'Gives you the lay of the land of your roles.';
$view->tag = 'architecture';
$view->base_table = 'role';
$view->human_name = 'architecture: Roles';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'architecture: List all Roles';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: Roles: Role ID */
$handler->display->display_options['fields']['rid']['id'] = 'rid';
$handler->display->display_options['fields']['rid']['table'] = 'role';
$handler->display->display_options['fields']['rid']['field'] = 'rid';
/* Field: Roles: Role name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'role';
$handler->display->display_options['fields']['name']['field'] = 'name';
/* Field: Roles: User count */
$handler->display->display_options['fields']['user_count']['id'] = 'user_count';
$handler->display->display_options['fields']['user_count']['table'] = 'role';
$handler->display->display_options['fields']['user_count']['field'] = 'user_count';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/reports/architecture/roles';
/* Menu settings */
$handler->display->display_options['path'] = 'admin/reports/architecture/roles';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Roles';
$handler->display->display_options['menu']['description'] = 'List of roles';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['weight'] = '0';

if(module_exists('views_data_export')) {
  // Add export link to Page view if relevant.
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['label'] = 'Export';
  $handler->display->display_options['header']['area']['content'] = '<a href="export" title="Export the result of this view">Export to CSV</a>';
  $handler->display->display_options['header']['area']['format'] = 'html';
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-roles.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/roles/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Roles export';
  $handler->display->display_options['menu']['description'] = 'Export list of roles';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}