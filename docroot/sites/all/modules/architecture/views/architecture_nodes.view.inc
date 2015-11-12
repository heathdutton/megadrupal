<?php

/**
 * @file
 * Defines a view for displaying node types.
 */

$view = new view();
$view->name = 'architecture_list_all_nodetypes';
$view->description = 'Gives you the lay of the land for your nodetypes.';
$view->tag = 'architecture';
$view->base_table = 'node_type';
$view->human_name = 'architecture: Nodetypes';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'architecture: List All Nodetypes';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'use architecture';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: Nodetype: Nodetype machine name */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node_type';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['label'] = 'Machine name';
/* Field: Nodetype: Nodetype Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'node_type';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'Name';
/* Field: Nodetype: Nodetype Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'node_type';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = 'Description';
/* Field: Nodetype: Nodetype Module */
$handler->display->display_options['fields']['module']['id'] = 'module';
$handler->display->display_options['fields']['module']['table'] = 'node_type';
$handler->display->display_options['fields']['module']['field'] = 'module';
$handler->display->display_options['fields']['module']['label'] = 'Module';
$handler->display->display_options['fields']['module']['element_label_colon'] = FALSE;
/* Field: Nodetype: Nodetype Help */
$handler->display->display_options['fields']['help']['id'] = 'help';
$handler->display->display_options['fields']['help']['table'] = 'node_type';
$handler->display->display_options['fields']['help']['field'] = 'help';
$handler->display->display_options['fields']['help']['label'] = 'Help';
$handler->display->display_options['fields']['help']['element_label_colon'] = FALSE;
/* Field: Nodetype: Nodetype options */
$handler->display->display_options['fields']['nodetype_options']['id'] = 'nodetype_options';
$handler->display->display_options['fields']['nodetype_options']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_options']['field'] = 'nodetype_options';
$handler->display->display_options['fields']['nodetype_options']['label'] = 'Options';
/* Field: Nodetype: Nodetype preview */
$handler->display->display_options['fields']['nodetype_preview']['id'] = 'nodetype_preview';
$handler->display->display_options['fields']['nodetype_preview']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_preview']['field'] = 'nodetype_preview';
$handler->display->display_options['fields']['nodetype_preview']['label'] = 'Preview before submitting';
/* Field: Nodetype: Nodetype submitted */
$handler->display->display_options['fields']['nodetype_submitted']['id'] = 'nodetype_submitted';
$handler->display->display_options['fields']['nodetype_submitted']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_submitted']['field'] = 'nodetype_submitted';
$handler->display->display_options['fields']['nodetype_submitted']['label'] = 'Display byline';
/* Field: COUNT(DISTINCT Content: Nid) */
$handler->display->display_options['fields']['nid']['id'] = 'nid';
$handler->display->display_options['fields']['nid']['table'] = 'node';
$handler->display->display_options['fields']['nid']['field'] = 'nid';
$handler->display->display_options['fields']['nid']['group_type'] = 'count_distinct';
$handler->display->display_options['fields']['nid']['label'] = '# of nodes of type';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'admin/reports/architecture/nodetypes';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Nodetypes';
$handler->display->display_options['menu']['description'] = 'List of nodetypes';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['weight'] = '0';

if (module_exists('views_data_export')) {
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = 'FoxtelCMS-20130916PM164751-nodetypes.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Nodetypes export';
  $handler->display->display_options['menu']['description'] = 'Export list of nodetypes';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler = $view->new_display('views_data_export', 'Data export 2', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'admin/reports/architecture/nodetypes/export';
}