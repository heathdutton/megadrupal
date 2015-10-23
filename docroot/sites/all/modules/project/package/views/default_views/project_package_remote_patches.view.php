<?php

$view = new view();
$view->name = 'project_package_remote_patches';
$view->description = 'Table of remote patches to a given packaged release.';
$view->tag = 'Project package';
$view->base_table = 'project_package_remote_item';
$view->human_name = 'Project package - remote patches';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = '';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'patch_nid' => 'patch_nid',
  'title' => 'title',
  'patch_file_url' => 'patch_file_url',
);
$handler->display->display_options['style_options']['default'] = 'name';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'patch_nid' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'patch_file_url' => array(
    'sortable' => 1,
    'separator' => '',
  ),
);
/* Relationship: Project package remote patches: Patch node */
$handler->display->display_options['relationships']['patch_nid']['id'] = 'patch_nid';
$handler->display->display_options['relationships']['patch_nid']['table'] = 'project_package_remote_patch';
$handler->display->display_options['relationships']['patch_nid']['field'] = 'patch_nid';
/* Field: Project package remote items: Remote item name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'project_package_remote_item';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'Name';
/* Field: Project package remote patches: Patch node */
$handler->display->display_options['fields']['patch_nid']['id'] = 'patch_nid';
$handler->display->display_options['fields']['patch_nid']['table'] = 'project_package_remote_patch';
$handler->display->display_options['fields']['patch_nid']['field'] = 'patch_nid';
$handler->display->display_options['fields']['patch_nid']['relationship'] = 'remote_id';
$handler->display->display_options['fields']['patch_nid']['exclude'] = TRUE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'patch_nid';
$handler->display->display_options['fields']['title']['label'] = 'Patch issue';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['text'] = '#[patch_nid]: [title]';
$handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
$handler->display->display_options['fields']['title']['alter']['path'] = 'node/[patch_nid]';
$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
/* Field: Project package remote patches: Patch URL */
$handler->display->display_options['fields']['patch_file_url']['id'] = 'patch_file_url';
$handler->display->display_options['fields']['patch_file_url']['table'] = 'project_package_remote_patch';
$handler->display->display_options['fields']['patch_file_url']['field'] = 'patch_file_url';
$handler->display->display_options['fields']['patch_file_url']['relationship'] = 'remote_id';
$handler->display->display_options['fields']['patch_file_url']['label'] = 'URL';
/* Contextual filter: Project package remote patches: Package node */
$handler->display->display_options['arguments']['package_nid_1']['id'] = 'package_nid_1';
$handler->display->display_options['arguments']['package_nid_1']['table'] = 'project_package_remote_patch';
$handler->display->display_options['arguments']['package_nid_1']['field'] = 'package_nid';
$handler->display->display_options['arguments']['package_nid_1']['default_action'] = 'not found';
$handler->display->display_options['arguments']['package_nid_1']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['package_nid_1']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['package_nid_1']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['package_nid_1']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['package_nid_1']['specify_validation'] = TRUE;
$handler->display->display_options['arguments']['package_nid_1']['validate']['type'] = 'node';
$handler->display->display_options['arguments']['package_nid_1']['validate_options']['types'] = array(
  'project_release' => 'project_release',
);
$handler->display->display_options['arguments']['package_nid_1']['validate_options']['access'] = TRUE;
