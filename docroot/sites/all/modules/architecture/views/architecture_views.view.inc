<?php
/**
 * @file
 * Defines a view for displaying views.
 */

/*
 * View 'architecture_list_all_views'
 */
$view = new view();
$view->name = 'architecture_list_all_views';
$view->description = 'Gives you the lay of the land for your Views';
$view->tag = 'architecture';
$view->base_table = 'views';
$view->human_name = 'architecture: Views';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'architecture: List all Views';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'use architecture';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
/* Field: Views: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'views';
$handler->display->display_options['fields']['title']['field'] = 'title';
/* Field: Views: Display */
$handler->display->display_options['fields']['display_title']['id'] = 'display_title';
$handler->display->display_options['fields']['display_title']['table'] = 'views';
$handler->display->display_options['fields']['display_title']['field'] = 'display_title';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page');
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'tag',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  1 => array(
    'field' => 'base_table',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  2 => array(
    'field' => 'title',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  3 => array(
    'field' => 'enabled',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  4 => array(
    'field' => 'storage',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  5 => array(
    'field' => 'description',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'display_title' => 'display_title',
  'enabled' => 'enabled',
  'description' => 'description',
  'access' => 'access',
  'base_table' => 'base_table',
  'tag' => 'tag',
  'storage' => 'storage',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'display_title' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'enabled' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'description' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'access' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'base_table' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'tag' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'storage' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Views: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'views';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['exclude'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['text'] = '<h3>[title]</h3>';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Views: Display */
$handler->display->display_options['fields']['display_title']['id'] = 'display_title';
$handler->display->display_options['fields']['display_title']['table'] = 'views';
$handler->display->display_options['fields']['display_title']['field'] = 'display_title';
/* Field: Views: Status */
$handler->display->display_options['fields']['enabled']['id'] = 'enabled';
$handler->display->display_options['fields']['enabled']['table'] = 'views';
$handler->display->display_options['fields']['enabled']['field'] = 'enabled';
$handler->display->display_options['fields']['enabled']['label'] = 'Enabled?';
$handler->display->display_options['fields']['enabled']['exclude'] = TRUE;
$handler->display->display_options['fields']['enabled']['element_label_colon'] = FALSE;
/* Field: Views: Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'views';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['exclude'] = TRUE;
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
/* Field: Views: Access */
$handler->display->display_options['fields']['access']['id'] = 'access';
$handler->display->display_options['fields']['access']['table'] = 'views';
$handler->display->display_options['fields']['access']['field'] = 'access';
/* Field: Views: Table */
$handler->display->display_options['fields']['base_table']['id'] = 'base_table';
$handler->display->display_options['fields']['base_table']['table'] = 'views';
$handler->display->display_options['fields']['base_table']['field'] = 'base_table';
$handler->display->display_options['fields']['base_table']['exclude'] = TRUE;
$handler->display->display_options['fields']['base_table']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['base_table']['alter']['text'] = '<h2>[base_table]<h2>';
/* Field: Views: Tag */
$handler->display->display_options['fields']['tag']['id'] = 'tag';
$handler->display->display_options['fields']['tag']['table'] = 'views';
$handler->display->display_options['fields']['tag']['field'] = 'tag';
$handler->display->display_options['fields']['tag']['exclude'] = TRUE;
$handler->display->display_options['fields']['tag']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['tag']['alter']['text'] = '<h1>[tag]</h1>';
$handler->display->display_options['fields']['tag']['empty'] = '<h1>[no tag]</h1>';
/* Field: Views: Storage */
$handler->display->display_options['fields']['storage']['id'] = 'storage';
$handler->display->display_options['fields']['storage']['table'] = 'views';
$handler->display->display_options['fields']['storage']['field'] = 'storage';
$handler->display->display_options['fields']['storage']['exclude'] = TRUE;
/* Field: Views: Path */
$handler->display->display_options['fields']['path']['id'] = 'path';
$handler->display->display_options['fields']['path']['table'] = 'views';
$handler->display->display_options['fields']['path']['field'] = 'path';
$handler->display->display_options['fields']['path']['exclude'] = FALSE;
/* Field: Views: query */
$handler->display->display_options['fields']['view_query']['id'] = 'view_query';
$handler->display->display_options['fields']['view_query']['table'] = 'views';
$handler->display->display_options['fields']['view_query']['field'] = 'view_query';
$handler->display->display_options['fields']['view_query']['exclude'] = FALSE;
/* Field: Views: Fields */
$handler->display->display_options['fields']['fields']['id'] = 'fields';
$handler->display->display_options['fields']['fields']['table'] = 'views';
$handler->display->display_options['fields']['fields']['field'] = 'fields';

$handler->display->display_options['path'] = 'admin/reports/architecture/views';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Views';
$handler->display->display_options['menu']['description'] = 'List of all the views';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;

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
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAHis') . '-views.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/views/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Views export';
  $handler->display->display_options['menu']['description'] = 'Export list of views';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}