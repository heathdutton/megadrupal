<?php
/**
 * @file
 * geocluster_solr_demo.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function geocluster_solr_demo_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'geocluster_test_solr';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'search_api_index_geocluster_index';
  $view->human_name = 'geocluster_test_solr';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Geocluster test solr';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'latlon';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'geocluster_lat';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'geocluster_lon';
  $handler->display->display_options['style_options']['data_source']['geofield'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'title';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'title';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Field: Indexed Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'search_api_index_geocluster_index';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_entity'] = 0;
  /* Field: Content: Geocluster ids (Solr) */
  $handler->display->display_options['fields']['geocluster_ids']['id'] = 'geocluster_ids';
  $handler->display->display_options['fields']['geocluster_ids']['table'] = 'search_api_index_geocluster_index';
  $handler->display->display_options['fields']['geocluster_ids']['field'] = 'geocluster_ids';
  /* Field: Content: Geocluster lat */
  $handler->display->display_options['fields']['geocluster_lat']['id'] = 'geocluster_lat';
  $handler->display->display_options['fields']['geocluster_lat']['table'] = 'search_api_index_geocluster_index';
  $handler->display->display_options['fields']['geocluster_lat']['field'] = 'geocluster_lat';
  /* Field: Content: Geocluster lon */
  $handler->display->display_options['fields']['geocluster_lon']['id'] = 'geocluster_lon';
  $handler->display->display_options['fields']['geocluster_lon']['table'] = 'search_api_index_geocluster_index';
  $handler->display->display_options['fields']['geocluster_lon']['field'] = 'geocluster_lon';
  /* Field: Content: Geocluster result count */
  $handler->display->display_options['fields']['geocluster_count']['id'] = 'geocluster_count';
  $handler->display->display_options['fields']['geocluster_count']['table'] = 'search_api_index_geocluster_index';
  $handler->display->display_options['fields']['geocluster_count']['field'] = 'geocluster_count';
  /* Field: Indexed Node: Place */
  $handler->display->display_options['fields']['field_place']['id'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['table'] = 'search_api_index_geocluster_index';
  $handler->display->display_options['fields']['field_place']['field'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_place']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_place']['settings'] = array(
    'data' => 'full',
  );
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_solr',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );

  /* Display: Solr JSON */
  $handler = $view->new_display('page', 'Solr JSON', 'json');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'solr-json';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Solr json';
  $handler->display->display_options['menu']['weight'] = '22';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Solr Table */
  $handler = $view->new_display('page', 'Solr Table', 'table');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'geocluster_ids' => 'geocluster_ids',
    'geocluster_lat' => 'geocluster_lat',
    'geocluster_lon' => 'geocluster_lon',
    'geocluster_count' => 'geocluster_count',
    'field_place' => 'field_place',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'geocluster_ids' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'geocluster_lat' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'geocluster_lon' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'geocluster_count' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'field_place' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_solr',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'solr-table';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Solr table';
  $handler->display->display_options['menu']['weight'] = '32';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['geocluster_test_solr'] = $view;

  return $export;
}
