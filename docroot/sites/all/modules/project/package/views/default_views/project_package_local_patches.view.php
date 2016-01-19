<?php
$view = new view();
$view->name = 'project_package_local_patches';
$view->description = 'Table of local patches to a given packaged release.';
$view->tag = 'Project package';
$view->base_table = 'node';
$view->human_name = 'Project package - local patches';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'patch_nid' => 'patch_nid',
  'title_1' => 'title_1',
  'patch_file_url' => 'patch_file_url',
);
$handler->display->display_options['style_options']['default'] = 'title';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'patch_nid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'title_1' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'patch_file_url' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Project package local patches: Package node */
$handler->display->display_options['relationships']['package_nid']['id'] = 'package_nid';
$handler->display->display_options['relationships']['package_nid']['table'] = 'project_package_local_patch';
$handler->display->display_options['relationships']['package_nid']['field'] = 'package_nid';
$handler->display->display_options['relationships']['package_nid']['required'] = TRUE;
/* Relationship: Project package local patches: Item node */
$handler->display->display_options['relationships']['item_nid']['id'] = 'item_nid';
$handler->display->display_options['relationships']['item_nid']['table'] = 'project_package_local_patch';
$handler->display->display_options['relationships']['item_nid']['field'] = 'item_nid';
$handler->display->display_options['relationships']['item_nid']['required'] = TRUE;
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_release_project_target_id']['id'] = 'field_release_project_target_id';
$handler->display->display_options['relationships']['field_release_project_target_id']['table'] = 'field_data_field_release_project';
$handler->display->display_options['relationships']['field_release_project_target_id']['field'] = 'field_release_project_target_id';
$handler->display->display_options['relationships']['field_release_project_target_id']['relationship'] = 'item_nid';
$handler->display->display_options['relationships']['field_release_project_target_id']['label'] = 'Item project node';
$handler->display->display_options['relationships']['field_release_project_target_id']['required'] = TRUE;
/* Relationship: Project package local patches: Patch node */
$handler->display->display_options['relationships']['patch_nid']['id'] = 'patch_nid';
$handler->display->display_options['relationships']['patch_nid']['table'] = 'project_package_local_patch';
$handler->display->display_options['relationships']['patch_nid']['field'] = 'patch_nid';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'field_release_project_target_id';
$handler->display->display_options['fields']['title']['label'] = 'Project';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
/* Field: Project package local patches: Patch node */
$handler->display->display_options['fields']['patch_nid']['id'] = 'patch_nid';
$handler->display->display_options['fields']['patch_nid']['table'] = 'project_package_local_patch';
$handler->display->display_options['fields']['patch_nid']['field'] = 'patch_nid';
$handler->display->display_options['fields']['patch_nid']['exclude'] = TRUE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title_1']['id'] = 'title_1';
$handler->display->display_options['fields']['title_1']['table'] = 'node';
$handler->display->display_options['fields']['title_1']['field'] = 'title';
$handler->display->display_options['fields']['title_1']['relationship'] = 'patch_nid';
$handler->display->display_options['fields']['title_1']['label'] = 'Patch issue';
$handler->display->display_options['fields']['title_1']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title_1']['alter']['text'] = '#[patch_nid]: [title_1]';
/* Field: Project package local patches: Patch URL */
$handler->display->display_options['fields']['patch_file_url']['id'] = 'patch_file_url';
$handler->display->display_options['fields']['patch_file_url']['table'] = 'project_package_local_patch';
$handler->display->display_options['fields']['patch_file_url']['field'] = 'patch_file_url';
/* Sort criterion: Content: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Contextual filter: Content: Nid */
$handler->display->display_options['arguments']['nid']['id'] = 'nid';
$handler->display->display_options['arguments']['nid']['table'] = 'node';
$handler->display->display_options['arguments']['nid']['field'] = 'nid';
$handler->display->display_options['arguments']['nid']['relationship'] = 'package_nid';
$handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['nid']['validate']['type'] = 'node';
$handler->display->display_options['arguments']['nid']['validate_options']['types'] = array(
  'project_release' => 'project_release',
);
$handler->display->display_options['arguments']['nid']['validate_options']['access'] = TRUE;
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 1;
$handler->display->display_options['filters']['status']['group'] = 1;
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
