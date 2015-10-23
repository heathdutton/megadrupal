<?php

$view = new view();
$view->name = 'architecture_list_all_fields';
$view->description = 'Gives you the lay of the land for your fields.';
$view->tag = 'architecture';
$view->base_table = 'field_config';
$view->human_name = 'architecture: Fields';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['title'] = 'architecture: List All Fields';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'access administration pages';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '0';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['pager']['options']['id'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  1 => array(
    'field' => 'type_1',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  2 => array(
    'field' => 'description',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  3 => array(
    'field' => 'module',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  4 => array(
    'field' => 'title_label',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  5 => array(
    'field' => 'help',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  6 => array(
    'field' => 'bundle',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  7 => array(
    'field' => 'nodetype_options',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  8 => array(
    'field' => 'nodetype_submitted',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  9 => array(
    'field' => 'nodetype_preview',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  10 => array(
    'field' => 'nodetype_menus',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  11 => array(
    'field' => 'nodetype_menu_parent',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
  0 => array(
    'field' => 'name',
    'rendered' => 1,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'field_name' => 'field_name',
  'type' => 'type',
  'bundle' => 'bundle',
  'description' => 'description',
  'help' => 'help',
  'type_1' => 'type_1',
  'module' => 'module',
  'name' => 'name',
  'title_label' => 'title_label',
  'nodetype_menus' => 'nodetype_menus',
  'nodetype_options' => 'nodetype_options',
  'nodetype_submitted' => 'nodetype_submitted',
  'nodetype_preview' => 'nodetype_preview',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'field_name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'type' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'bundle' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'description' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'help' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'type_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'module' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'name' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title_label' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nodetype_menus' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nodetype_options' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nodetype_submitted' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'nodetype_preview' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: Field base: Field Name */
$handler->display->display_options['fields']['field_name']['id'] = 'field_name';
$handler->display->display_options['fields']['field_name']['table'] = 'field_config';
$handler->display->display_options['fields']['field_name']['field'] = 'field_name';
$handler->display->display_options['fields']['field_name']['label'] = 'Name';
$handler->display->display_options['fields']['field_name']['element_label_colon'] = FALSE;
/* Field: Field base: Field type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'field_config';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['label'] = 'Type';
$handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
/* Field: Field instance: Bundle */
$handler->display->display_options['fields']['bundle']['id'] = 'bundle';
$handler->display->display_options['fields']['bundle']['table'] = 'field_config_instance';
$handler->display->display_options['fields']['bundle']['field'] = 'bundle';
$handler->display->display_options['fields']['bundle']['exclude'] = TRUE;
$handler->display->display_options['fields']['bundle']['element_label_colon'] = FALSE;
/* Field: Nodetype: Nodetype Description */
$handler->display->display_options['fields']['description']['id'] = 'description';
$handler->display->display_options['fields']['description']['table'] = 'node_type';
$handler->display->display_options['fields']['description']['field'] = 'description';
$handler->display->display_options['fields']['description']['label'] = '';
$handler->display->display_options['fields']['description']['exclude'] = TRUE;
$handler->display->display_options['fields']['description']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['description']['empty'] = '[nodetype has no description]';
/* Field: Nodetype: Nodetype Help */
$handler->display->display_options['fields']['help']['id'] = 'help';
$handler->display->display_options['fields']['help']['table'] = 'node_type';
$handler->display->display_options['fields']['help']['field'] = 'help';
$handler->display->display_options['fields']['help']['label'] = 'Help text';
$handler->display->display_options['fields']['help']['exclude'] = TRUE;
$handler->display->display_options['fields']['help']['empty'] = '[none]';
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
$handler->display->display_options['fields']['name']['label'] = '';
$handler->display->display_options['fields']['name']['exclude'] = TRUE;
$handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['name']['empty'] = '[no nodetype associated]';
/* Field: Nodetype: Nodetype Title Label */
$handler->display->display_options['fields']['title_label']['id'] = 'title_label';
$handler->display->display_options['fields']['title_label']['table'] = 'node_type';
$handler->display->display_options['fields']['title_label']['field'] = 'title_label';
$handler->display->display_options['fields']['title_label']['label'] = 'Title Label';
$handler->display->display_options['fields']['title_label']['exclude'] = TRUE;
$handler->display->display_options['fields']['title_label']['empty'] = '[none]';
/* Field: Menus: Nodetype menus */
$handler->display->display_options['fields']['nodetype_menus']['id'] = 'nodetype_menus';
$handler->display->display_options['fields']['nodetype_menus']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_menus']['field'] = 'nodetype_menus';
$handler->display->display_options['fields']['nodetype_menus']['label'] = 'Available menus';
$handler->display->display_options['fields']['nodetype_menus']['exclude'] = TRUE;
$handler->display->display_options['fields']['nodetype_menus']['empty'] = '[none selected]';
/* Field: Nodetype: Nodetype options */
$handler->display->display_options['fields']['nodetype_options']['id'] = 'nodetype_options';
$handler->display->display_options['fields']['nodetype_options']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_options']['field'] = 'nodetype_options';
$handler->display->display_options['fields']['nodetype_options']['exclude'] = TRUE;
$handler->display->display_options['fields']['nodetype_options']['empty'] = '[no options set]';
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
/* Field: Nodetype: Nodetype submitted */
$handler->display->display_options['fields']['nodetype_submitted']['id'] = 'nodetype_submitted';
$handler->display->display_options['fields']['nodetype_submitted']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_submitted']['field'] = 'nodetype_submitted';
$handler->display->display_options['fields']['nodetype_submitted']['exclude'] = TRUE;
$handler->display->display_options['fields']['nodetype_submitted']['empty'] = '[not set]';
/* Field: Nodetype: Nodetype preview */
$handler->display->display_options['fields']['nodetype_preview']['id'] = 'nodetype_preview';
$handler->display->display_options['fields']['nodetype_preview']['table'] = 'node_type';
$handler->display->display_options['fields']['nodetype_preview']['field'] = 'nodetype_preview';
$handler->display->display_options['fields']['nodetype_preview']['exclude'] = TRUE;
$handler->display->display_options['fields']['nodetype_preview']['empty'] = '[not set]';
/* Filter criterion: Nodetype: Nodetype Name */
$handler->display->display_options['filters']['name']['id'] = 'name';
$handler->display->display_options['filters']['name']['table'] = 'node_type';
$handler->display->display_options['filters']['name']['field'] = 'name';
$handler->display->display_options['filters']['name']['exposed'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['label'] = 'Nodetype';
$handler->display->display_options['filters']['name']['expose']['description'] = 'Human-readable name of the nodetype.';
$handler->display->display_options['filters']['name']['expose']['use_operator'] = TRUE;
$handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
$handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
$handler->display->display_options['filters']['name']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
  4 => 0,
  5 => 0,
  6 => 0,
  7 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'admin/reports/architecture/fields';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'Fields';
$handler->display->display_options['menu']['description'] = 'List of fields';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['tab_options']['weight'] = '0';

if(module_exists('views_data_export')) {
  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['path'] = 'admin/reports/architecture/fields/export';
  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAhis') . '-fields.csv';

  /* Display: Data export 2 */
  $handler = $view->new_display('views_data_export', 'Data export 2', 'views_data_export_2');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Field base: Field Name */
  $handler->display->display_options['fields']['field_name']['id'] = 'field_name';
  $handler->display->display_options['fields']['field_name']['table'] = 'field_config';
  $handler->display->display_options['fields']['field_name']['field'] = 'field_name';
  $handler->display->display_options['fields']['field_name']['label'] = 'Name';
  $handler->display->display_options['fields']['field_name']['element_label_colon'] = FALSE;
  /* Field: Field base: Field type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'field_config';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: Field instance: Bundle */
  $handler->display->display_options['fields']['bundle']['id'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['table'] = 'field_config_instance';
  $handler->display->display_options['fields']['bundle']['field'] = 'bundle';
  $handler->display->display_options['fields']['bundle']['exclude'] = TRUE;
  $handler->display->display_options['fields']['bundle']['element_label_colon'] = FALSE;
  /* Field: Field base: Field PHP */
  $handler->display->display_options['fields']['field_contains_php']['id'] = 'field_contains_php';
  $handler->display->display_options['fields']['field_contains_php']['table'] = 'field_config';
  $handler->display->display_options['fields']['field_contains_php']['field'] = 'field_contains_php';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/architecture/fields/php/export';

  $handler->display->display_options['style_options']['filename'] = variable_get('site_name') . '-' . date('YmdAhis') . '-php-fields.csv';
  /* Menu settings */
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Fields export';
  $handler->display->display_options['menu']['description'] = 'Export list of fields with PHP in them';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
}