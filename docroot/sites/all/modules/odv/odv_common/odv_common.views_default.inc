<?php
/**
 * @file
 * odv_common.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function odv_common_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'odv_map';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'ODV Map';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Wien Map';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['style_plugin'] = 'openlayers_data';
  $handler->display->display_options['style_options']['data_source'] = array(
    'value' => 'wkt',
    'other_lat' => 'title',
    'other_lon' => 'title',
    'wkt' => 'field_od_geo',
    'other_top' => 'title',
    'other_right' => 'title',
    'other_bottom' => 'title',
    'other_left' => 'title',
    'name_field' => 'title',
    'description_field' => '',
  );
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
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
  /* Field: Content: Geo */
  $handler->display->display_options['fields']['field_od_geo']['id'] = 'field_od_geo';
  $handler->display->display_options['fields']['field_od_geo']['table'] = 'field_data_field_od_geo';
  $handler->display->display_options['fields']['field_od_geo']['field'] = 'field_od_geo';
  $handler->display->display_options['fields']['field_od_geo']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['external'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['field_od_geo']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['field_od_geo']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['alter']['html'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['field_od_geo']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['field_od_geo']['hide_empty'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['empty_zero'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['field_od_geo']['click_sort_column'] = 'wkt';
  $handler->display->display_options['fields']['field_od_geo']['settings'] = array(
    'data' => 'full',
  );
  $handler->display->display_options['fields']['field_od_geo']['field_api_classes'] = 0;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
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
    'odv_bicycle_rack' => 'odv_bicycle_rack',
    'odv_citybike' => 'odv_citybike',
    'odv_drinking_fountain' => 'odv_drinking_fountain',
    'odv_hospital' => 'odv_hospital',
    'odv_university' => 'odv_university',
  );
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
  /* Filter criterion: Content: City (field_od_address_city) */
  $handler->display->display_options['filters']['field_od_address_city_value']['id'] = 'field_od_address_city_value';
  $handler->display->display_options['filters']['field_od_address_city_value']['table'] = 'field_data_field_od_address_city';
  $handler->display->display_options['filters']['field_od_address_city_value']['field'] = 'field_od_address_city_value';
  $handler->display->display_options['filters']['field_od_address_city_value']['value'] = 'Wien';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'openlayers_map';
  $handler->display->display_options['style_options']['map'] = 'odv_map';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = 'wien/map';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Wien Map';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'main-menu';

  /* Display: OpenLayers Data Overlay */
  $handler = $view->new_display('openlayers', 'OpenLayers Data Overlay', 'openlayers_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['hide_empty'] = 0;
  $handler->display->display_options['row_options']['default_field_elements'] = 1;
  $export['odv_map'] = $view;

  return $export;
}
