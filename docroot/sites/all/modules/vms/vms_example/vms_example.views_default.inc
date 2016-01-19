<?php
/**
 * @file
 * vms_example.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function vms_example_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'sidebar_messages';
  $view->description = 'This is an example view for the Views Menu Support project.';
  $view->tag = 'example, Views Menu Support';
  $view->base_table = 'node';
  $view->human_name = 'Sidebar messages';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sidebar messages';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  $handler->display->display_options['row_options']['links'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['filter_groups']['groups'] = array(
    0 => 'AND',
    1 => 'AND',
  );
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'sidebar_message' => 'sidebar_message',
  );
  $handler->display->display_options['filters']['type']['group'] = 0;
  /* Filter criterion: MLID filtering: field_sidebar_mlid_direct */
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_direct']['id'] = 'vms_field_sidebar_mlid_direct';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_direct']['table'] = 'field_data_field_sidebar_mlid_direct';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_direct']['field'] = 'vms_field_sidebar_mlid_direct';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_direct']['value'] = '***VMS_CURRENT***';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_direct']['group'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'sidebar_message' => 'sidebar_message',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: MLID filtering: field_sidebar_mlid_sub */
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_sub']['id'] = 'vms_field_sidebar_mlid_sub';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_sub']['table'] = 'field_data_field_sidebar_mlid_sub';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_sub']['field'] = 'vms_field_sidebar_mlid_sub';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_sub']['value'] = '***VMS_EXCLUDE_CURRENT***';
  $handler->display->display_options['filters']['vms_field_sidebar_mlid_sub']['group'] = 1;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_description'] = 'Example: Sidebar messages';
  $export['sidebar_messages'] = $view;

  return $export;
}
