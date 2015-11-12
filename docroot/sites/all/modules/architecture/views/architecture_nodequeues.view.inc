<?php
/**
 * @file
 * Defines a view for displaying nodequeue info.
 */

/*
 * View 'architecture_list_all_nodequeus'
*/
$view = new view();
$view->name = 'architecture_list_all_nodequeues';
$view->description = 'Gives you the lay of the land for your nodequeues.';
$view->tag = 'architecture';
$view->base_table = 'nodequeue_queue';
$view->human_name = 'architecture: Nodequeues';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'architecture: List all Nodequeues';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: Nodequeue: Queue machine name */
$handler->display->display_options['fields']['name_1']['id'] = 'name_1';
$handler->display->display_options['fields']['name_1']['table'] = 'nodequeue_queue';
$handler->display->display_options['fields']['name_1']['field'] = 'name';
/* Field: Nodequeue: Queue title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'nodequeue_queue';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Nodequeue: Added date */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'nodequeue_nodes';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['timestamp']['date_format'] = 'long';
/* Field: Nodequeue: Subqueue ID */
$handler->display->display_options['fields']['sqid']['id'] = 'sqid';
$handler->display->display_options['fields']['sqid']['table'] = 'nodequeue_nodes';
$handler->display->display_options['fields']['sqid']['field'] = 'sqid';
$handler->display->display_options['fields']['sqid']['label'] = 'Subqueue';
$handler->display->display_options['fields']['sqid']['element_label_colon'] = FALSE;
/* Field: Nodequeue: Position */
$handler->display->display_options['fields']['position']['id'] = 'position';
$handler->display->display_options['fields']['position']['table'] = 'nodequeue_nodes';
$handler->display->display_options['fields']['position']['field'] = 'position';
$handler->display->display_options['fields']['position']['element_label_colon'] = FALSE;
/* Field: Nodequeue: Node ID */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'nodequeue_nodes';
$handler->display->display_options['fields']['nid']['field'] = 'nid';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['path'] = 'admin/reports/architecture/nodequeues';
/* Menu settings */
$handler->display->display_options['path'] = 'admin/reports/architecture/nodequeues';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Nodequeues';
$handler->display->display_options['menu']['description'] = 'List of nodequeues';
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
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-nodequeues.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/nodequeues/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Nodequeues export';
  $handler->display->display_options['menu']['description'] = 'Export list of nodequeues';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}