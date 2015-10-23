<?php

$view = new view();
$view->name = 'project_package_remote_items';
$view->description = 'Table of remote items (e.g. external libraries) included in a given packaged release.';
$view->tag = 'Project package';
$view->base_table = 'project_package_remote_item';
$view->human_name = 'Project package - remote items';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'name' => 'name',
  'url' => 'url',
);
$handler->display->display_options['style_options']['default'] = 'name';
$handler->display->display_options['style_options']['info'] = array(
  'name' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'url' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: Project package remote items: Remote item name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'project_package_remote_item';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'Name';
/* Field: Project package remote items: Remote item URL */
$handler->display->display_options['fields']['url']['id'] = 'url';
$handler->display->display_options['fields']['url']['table'] = 'project_package_remote_item';
$handler->display->display_options['fields']['url']['field'] = 'url';
$handler->display->display_options['fields']['url']['label'] = 'URL';
$handler->display->display_options['fields']['url']['display_as_link'] = FALSE;
/* Contextual filter: Project package remote items: Package node */
$handler->display->display_options['arguments']['package_nid']['id'] = 'package_nid';
$handler->display->display_options['arguments']['package_nid']['table'] = 'project_package_remote_item';
$handler->display->display_options['arguments']['package_nid']['field'] = 'package_nid';
$handler->display->display_options['arguments']['package_nid']['default_action'] = 'not found';
$handler->display->display_options['arguments']['package_nid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['package_nid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['package_nid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['package_nid']['summary_options']['items_per_page'] = '25';
