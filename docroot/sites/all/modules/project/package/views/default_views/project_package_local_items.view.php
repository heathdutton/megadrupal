<?php

$view = new view();
$view->name = 'project_package_local_items';
$view->description = 'View of all release items included in a given package release.';
$view->tag = 'Project package';
$view->base_table = 'node';
$view->human_name = 'Project package - local items';
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
  'field_release_version' => 'field_release_version',
  'view_node' => 'view_node',
  'patch_count' => 'patch_count',
  'field_release_update_status' => 'field_release_update_status',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_release_version' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'view_node' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'patch_count' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'field_release_update_status' => array(
    'sortable' => 1,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Project package: Package node */
$handler->display->display_options['relationships']['package_nid']['id'] = 'package_nid';
$handler->display->display_options['relationships']['package_nid']['table'] = 'project_package_local_release_item';
$handler->display->display_options['relationships']['package_nid']['field'] = 'package_nid';
$handler->display->display_options['relationships']['package_nid']['required'] = TRUE;
/* Relationship: Entity Reference: Referenced Entity */
$handler->display->display_options['relationships']['field_release_project_target_id']['id'] = 'field_release_project_target_id';
$handler->display->display_options['relationships']['field_release_project_target_id']['table'] = 'field_data_field_release_project';
$handler->display->display_options['relationships']['field_release_project_target_id']['field'] = 'field_release_project_target_id';
$handler->display->display_options['relationships']['field_release_project_target_id']['label'] = 'Project node';
$handler->display->display_options['relationships']['field_release_project_target_id']['required'] = TRUE;
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'field_release_project_target_id';
$handler->display->display_options['fields']['title']['label'] = 'Project';
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
/* Field: Content: Version */
$handler->display->display_options['fields']['field_release_version']['id'] = 'field_release_version';
$handler->display->display_options['fields']['field_release_version']['table'] = 'field_data_field_release_version';
$handler->display->display_options['fields']['field_release_version']['field'] = 'field_release_version';
$handler->display->display_options['fields']['field_release_version']['exclude'] = TRUE;
$handler->display->display_options['fields']['field_release_version']['alter']['make_link'] = TRUE;
/* Field: Content: Link */
$handler->display->display_options['fields']['view_node']['id'] = 'view_node';
$handler->display->display_options['fields']['view_node']['table'] = 'views_entity_node';
$handler->display->display_options['fields']['view_node']['field'] = 'view_node';
$handler->display->display_options['fields']['view_node']['label'] = 'Version';
$handler->display->display_options['fields']['view_node']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['view_node']['alter']['text'] = '[field_release_version]';
/* Field: Project package: Item patch count */
$handler->display->display_options['fields']['patch_count']['id'] = 'patch_count';
$handler->display->display_options['fields']['patch_count']['table'] = 'project_package_local_release_item';
$handler->display->display_options['fields']['patch_count']['field'] = 'patch_count';
$handler->display->display_options['fields']['patch_count']['label'] = '';
$handler->display->display_options['fields']['patch_count']['exclude'] = TRUE;
$handler->display->display_options['fields']['patch_count']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['patch_count']['alter']['text'] = '<div class="patch-count">[patch_count] applied</div>';
$handler->display->display_options['fields']['patch_count']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['patch_count']['hide_empty'] = TRUE;
/* Field: Content: Update status */
$handler->display->display_options['fields']['field_release_update_status']['id'] = 'field_release_update_status';
$handler->display->display_options['fields']['field_release_update_status']['table'] = 'field_data_field_release_update_status';
$handler->display->display_options['fields']['field_release_update_status']['field'] = 'field_release_update_status';
$handler->display->display_options['fields']['field_release_update_status']['label'] = 'Status';
$handler->display->display_options['fields']['field_release_update_status']['alter']['alter_text'] = TRUE;
$handler->display->display_options['fields']['field_release_update_status']['alter']['text'] = '<div class="update-status-[field_release_update_status-value]">[field_release_update_status]</div>[patch_count]';
$handler->display->display_options['fields']['field_release_update_status']['type'] = 'project_release_update_status';
$handler->display->display_options['fields']['field_release_update_status']['settings'] = array(
  'thousand_separator' => ' ',
  'prefix_suffix' => 1,
);
/* Sort criterion: Content: Update status (field_release_update_status) */
$handler->display->display_options['sorts']['field_release_update_status_value']['id'] = 'field_release_update_status_value';
$handler->display->display_options['sorts']['field_release_update_status_value']['table'] = 'field_data_field_release_update_status';
$handler->display->display_options['sorts']['field_release_update_status_value']['field'] = 'field_release_update_status_value';
$handler->display->display_options['sorts']['field_release_update_status_value']['order'] = 'DESC';
/* Sort criterion: Content: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'node';
$handler->display->display_options['sorts']['title']['field'] = 'title';
$handler->display->display_options['sorts']['title']['relationship'] = 'field_release_project_target_id';
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
