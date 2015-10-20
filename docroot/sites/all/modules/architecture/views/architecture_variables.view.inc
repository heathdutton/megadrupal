<?php
/**
 * @file
 * Defines a view for displaying variables.
 */

/*
 * View 'architecture_list_all_variables'
 */
$view = new view();
$view->name = 'architecture_list_all_variables';
$view->description = 'Gives you the lay of the land for your variables.';
$view->tag = 'architecture';
$view->base_table = 'variable';
$view->human_name = 'architecture: Variables';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'architecture: List all Variables';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'use architecture';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: Variables: Variable name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'variable';
$handler->display->display_options['fields']['name']['field'] = 'name';
/* Field: Variables: Variable value */
$handler->display->display_options['fields']['value']['id'] = 'value';
$handler->display->display_options['fields']['value']['table'] = 'variable';
$handler->display->display_options['fields']['value']['field'] = 'value';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/reports/architecture/variables';
/* Menu settings */
$handler->display->display_options['path'] = 'admin/reports/architecture/variables';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Variables';
$handler->display->display_options['menu']['description'] = 'List of variables';
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
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-variables.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/variables/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Variables export';
  $handler->display->display_options['menu']['description'] = 'Export list of variables';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}