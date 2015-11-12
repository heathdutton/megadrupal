<?php

/**
 * @file
 * Defines a view for displaying menus
 */

/*
 * View 'architecture_list_all_menus'
 */
$view = views_new_view();
$view->name = 'architecture_list_all_menus';
$view->description = 'Gives you the lay of the land for your menus.';
$view->tag = 'architecture';
$view->human_name = 'architecture: Menus';
$view->view_php = '';
$view->base_table = 'menu_links';
$view->is_cacheable = FALSE;
$view->api_version = '3.0-alpha1';
$view->disabled = FALSE;
/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'architecture: List All Menus';
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'use architecture';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager'] = array(
  'type' => 'full',
  'options' => array(
    'items_per_page'=> '0',
    'offset' => '0',
    'id' => '0',
  ),
);
$handler->display->display_options['style_plugin'] = 'grid';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'menu',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = '6';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: : Link title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'menu_links';
$handler->display->display_options['fields']['title']['field'] = 'link_title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: : Menu title */
$handler->display->display_options['fields']['menu']['id'] = 'menu';
$handler->display->display_options['fields']['menu']['table'] = 'menu_links';
$handler->display->display_options['fields']['menu']['field'] = 'menu_name';
$handler->display->display_options['fields']['menu']['label'] = '';
$handler->display->display_options['fields']['menu']['exclude'] = TRUE;
$handler->display->display_options['fields']['menu']['element_label_colon'] = FALSE;
/* Field: : Router path */
$handler->display->display_options['fields']['router']['id'] = 'router';
$handler->display->display_options['fields']['router']['table'] = 'menu_links';
$handler->display->display_options['fields']['router']['field'] = 'router_path';
$handler->display->display_options['fields']['router']['label'] = '';
$handler->display->display_options['fields']['router']['exclude'] = TRUE;
$handler->display->display_options['fields']['router']['element_label_colon'] = FALSE;
/* Filter criterion: : Link title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'menu_links';
$handler->display->display_options['filters']['title']['field'] = 'link_title';
$handler->display->display_options['filters']['title']['operator'] = 'longerthan';
$handler->display->display_options['filters']['title']['value'] = '0';
/* Filter criterion: : Menu title */
$handler->display->display_options['filters']['menu']['id'] = 'menu';
$handler->display->display_options['filters']['menu']['table'] = 'menu_links';
$handler->display->display_options['filters']['menu']['field'] = 'menu_name';
$handler->display->display_options['filters']['menu']['exposed'] = TRUE;
$handler->display->display_options['filters']['menu']['expose']['operator_id'] = 'menu_op';
$handler->display->display_options['filters']['menu']['expose']['label'] = 'Menu';
$handler->display->display_options['filters']['menu']['expose']['description'] = 'Select which menu to display';
$handler->display->display_options['filters']['menu']['expose']['operator'] = 'menu_op';
$handler->display->display_options['filters']['menu']['expose']['identifier'] = 'menu';
$handler->display->display_options['filters']['menu']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  5 => 0,
  4 => 0,
  3 => 0,
  6 => 0,
  7 => 0,
);
/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
/* Menu settings */
$handler->display->display_options['path'] = 'admin/reports/architecture/menus';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Menus';
$handler->display->display_options['menu']['description'] = 'List of menus';
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
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: architecture: Field Name */
  $handler->display->display_options['fields']['field_name']['id'] = 'field_name';
  $handler->display->display_options['fields']['field_name']['table'] = 'field_config';
  $handler->display->display_options['fields']['field_name']['field'] = 'field_name';
  $handler->display->display_options['fields']['field_name']['label'] = 'Name';
  $handler->display->display_options['fields']['field_name']['element_label_colon'] = FALSE;
  /* Field: architecture: Field type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'field_config';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: architecture: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'field_config_instance';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['element_label_colon'] = FALSE;
  /* Field: Nodetype: Nodetype machine name */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'node_type';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['label'] = '';
  $handler->display->display_options['fields']['type_1']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['type_1']['empty'] = '[no nodetype machine name]';
  /* Field: Nodetype: Nodetype Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'node_type';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  $handler->display->display_options['fields']['module']['label'] = 'Nodetype defined by module';
  $handler->display->display_options['fields']['module']['exclude'] = TRUE;
  $handler->display->display_options['fields']['module']['empty'] = '[none]';
  /* Field: Nodetype: Nodetype Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'node_type';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Nodetype';
  $handler->display->display_options['fields']['name']['empty'] = '[no nodetype associated]';
  /* Field: Field instance: Label */
  $handler->display->display_options['fields']['field_label']['id'] = 'field_label';
  $handler->display->display_options['fields']['field_label']['table'] = 'field_config_instance';
  $handler->display->display_options['fields']['field_label']['field'] = 'field_label';
  $handler->display->display_options['fields']['field_label']['empty'] = '[no options set]';
  /* Field: Field instance: Description */
  $handler->display->display_options['fields']['field_description']['id'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['table'] = 'field_config_instance';
  $handler->display->display_options['fields']['field_description']['field'] = 'field_description';
  $handler->display->display_options['fields']['field_description']['empty'] = '[no description]';
  /* Field: Field instance: Required */
  $handler->display->display_options['fields']['field_required']['id'] = 'field_required';
  $handler->display->display_options['fields']['field_required']['table'] = 'field_config_instance';
  $handler->display->display_options['fields']['field_required']['field'] = 'field_required';
  $handler->display->display_options['fields']['field_required']['empty'] = '[no option set]';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;

  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAhis') . '-menus.csv';
  /* Menu settings */
  $handler->display->display_options['path'] = 'admin/reports/architecture/menus/export';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Menus export';
  $handler->display->display_options['menu']['description'] = 'Export list of menus';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}