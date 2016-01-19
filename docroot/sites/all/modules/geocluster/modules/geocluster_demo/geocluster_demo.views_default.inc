<?php
/**
 * @file
 * geocluster_demo.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function geocluster_demo_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'geocluster_test_default';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'geocluster_test_default';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Geocluster test default';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'geofield';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['geofield'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'title';
  $handler->display->display_options['style_options']['data_source']['description_field'] = '';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Field: Content: Place */
  $handler->display->display_options['fields']['field_place']['id'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['field_place']['field'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_place']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_place']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_place']['type'] = 'geofield_latlon';
  $handler->display->display_options['fields']['field_place']['settings'] = array(
    'data' => 'full',
    'format' => 'decimal_degrees',
    'labels' => 1,
  );
  $handler->display->display_options['fields']['field_place']['group_column'] = 'geocluster_index_4';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  /* Contextual filter: Custom: Bounding box */
  $handler->display->display_options['arguments']['bbox_argument']['id'] = 'bbox_argument';
  $handler->display->display_options['arguments']['bbox_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['bbox_argument']['field'] = 'bbox_argument';
  $handler->display->display_options['arguments']['bbox_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['bbox_argument']['default_argument_type'] = 'querystring';
  $handler->display->display_options['arguments']['bbox_argument']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['bbox_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['bbox_argument']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['geocluster_enabled'] = 0;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_php',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );

  /* Display: Default GeoJSON */
  $handler = $view->new_display('page', 'Default GeoJSON', 'json');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'default-json';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Default json';
  $handler->display->display_options['menu']['weight'] = '23';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Default Table */
  $handler = $view->new_display('page', 'Default Table', 'table');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_place' => 'field_place',
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_place' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['geocluster_enabled'] = 0;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_php',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'default-table';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Default  table';
  $handler->display->display_options['menu']['weight'] = '33';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['geocluster_test_default'] = $view;

  $view = new view();
  $view->name = 'geocluster_test_mysql';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'geocluster_test_mysql';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Geocluster test mysql';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'latlon';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'geocluster_lat';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'geocluster_lon';
  $handler->display->display_options['style_options']['data_source']['geofield'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'geocluster_count';
  $handler->display->display_options['style_options']['data_source']['description_field'] = 'geocluster_ids';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Field: Content: Place */
  $handler->display->display_options['fields']['field_place']['id'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['field_place']['field'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_place']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_place']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_place']['settings'] = array(
    'data' => 'full',
  );
  $handler->display->display_options['fields']['field_place']['group_column'] = 'geocluster_index_4';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['group_type'] = 'group_concat';
  /* Field: Content: Geocluster lat (field_place) */
  $handler->display->display_options['fields']['geocluster_lat']['id'] = 'geocluster_lat';
  $handler->display->display_options['fields']['geocluster_lat']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_lat']['field'] = 'geocluster_lat';
  /* Field: Content: Geocluster lon (field_place) */
  $handler->display->display_options['fields']['geocluster_lon']['id'] = 'geocluster_lon';
  $handler->display->display_options['fields']['geocluster_lon']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_lon']['field'] = 'geocluster_lon';
  /* Field: Content: Geocluster result count (field_place) */
  $handler->display->display_options['fields']['geocluster_count']['id'] = 'geocluster_count';
  $handler->display->display_options['fields']['geocluster_count']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_count']['field'] = 'geocluster_count';
  /* Field: Content: Geocluster ids (field_place) */
  $handler->display->display_options['fields']['geocluster_ids']['id'] = 'geocluster_ids';
  $handler->display->display_options['fields']['geocluster_ids']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_ids']['field'] = 'geocluster_ids';
  /* Contextual filter: Custom: Bounding box */
  $handler->display->display_options['arguments']['bbox_argument']['id'] = 'bbox_argument';
  $handler->display->display_options['arguments']['bbox_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['bbox_argument']['field'] = 'bbox_argument';
  $handler->display->display_options['arguments']['bbox_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['bbox_argument']['default_argument_type'] = 'querystring';
  $handler->display->display_options['arguments']['bbox_argument']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['bbox_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['bbox_argument']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_php',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );

  /* Display: MySQL GeoJSON */
  $handler = $view->new_display('page', 'MySQL GeoJSON', 'json');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'latlon';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'geocluster_lat';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'geocluster_lon';
  $handler->display->display_options['style_options']['data_source']['geofield'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'title';
  $handler->display->display_options['style_options']['data_source']['description_field'] = '';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_mysql',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'mysql-json';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'MySQL json';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Table */
  $handler = $view->new_display('page', 'Table', 'table');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_place' => 'field_place',
    'title' => 'title',
    'geocluster_lat' => 'geocluster_lat',
    'geocluster_lon' => 'geocluster_lon',
    'geocluster_count' => 'geocluster_count',
    'geocluster_ids' => 'geocluster_ids',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_place' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'geocluster_ids' => array(
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
    'algorithm' => 'geocluster_mysql',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'mysql-table';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'MySQL  table';
  $handler->display->display_options['menu']['weight'] = '30';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['geocluster_test_mysql'] = $view;

  $view = new view();
  $view->name = 'geocluster_test_php';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'geocluster_test_php';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Geocluster test php';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_geojson';
  $handler->display->display_options['style_options']['data_source']['value'] = 'latlon';
  $handler->display->display_options['style_options']['data_source']['latitude'] = 'geocluster_lat';
  $handler->display->display_options['style_options']['data_source']['longitude'] = 'geocluster_lon';
  $handler->display->display_options['style_options']['data_source']['geofield'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['wkt'] = 'field_place';
  $handler->display->display_options['style_options']['data_source']['name_field'] = 'geocluster_count';
  $handler->display->display_options['style_options']['data_source']['description_field'] = 'geocluster_ids';
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 0;
  /* Field: Content: Place */
  $handler->display->display_options['fields']['field_place']['id'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['field_place']['field'] = 'field_place';
  $handler->display->display_options['fields']['field_place']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_place']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_place']['click_sort_column'] = 'geom';
  $handler->display->display_options['fields']['field_place']['settings'] = array(
    'data' => 'full',
  );
  $handler->display->display_options['fields']['field_place']['group_column'] = 'geocluster_index_4';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['group_type'] = 'count';
  /* Field: Content: Geocluster lat (field_place) */
  $handler->display->display_options['fields']['geocluster_lat']['id'] = 'geocluster_lat';
  $handler->display->display_options['fields']['geocluster_lat']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_lat']['field'] = 'geocluster_lat';
  /* Field: Content: Geocluster lon (field_place) */
  $handler->display->display_options['fields']['geocluster_lon']['id'] = 'geocluster_lon';
  $handler->display->display_options['fields']['geocluster_lon']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_lon']['field'] = 'geocluster_lon';
  /* Field: Content: Geocluster result count (field_place) */
  $handler->display->display_options['fields']['geocluster_count']['id'] = 'geocluster_count';
  $handler->display->display_options['fields']['geocluster_count']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_count']['field'] = 'geocluster_count';
  /* Field: Content: Geocluster ids (field_place) */
  $handler->display->display_options['fields']['geocluster_ids']['id'] = 'geocluster_ids';
  $handler->display->display_options['fields']['geocluster_ids']['table'] = 'field_data_field_place';
  $handler->display->display_options['fields']['geocluster_ids']['field'] = 'geocluster_ids';
  /* Contextual filter: Custom: Bounding box */
  $handler->display->display_options['arguments']['bbox_argument']['id'] = 'bbox_argument';
  $handler->display->display_options['arguments']['bbox_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['bbox_argument']['field'] = 'bbox_argument';
  $handler->display->display_options['arguments']['bbox_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['bbox_argument']['default_argument_type'] = 'querystring';
  $handler->display->display_options['arguments']['bbox_argument']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['bbox_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['bbox_argument']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'algorithm' => 'geocluster_php',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );

  /* Display: PHP GeoJSON */
  $handler = $view->new_display('page', 'PHP GeoJSON', 'json');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['geocluster_enabled'] = 1;
  $handler->display->display_options['geocluster_options'] = array(
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'php-json';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'PHP json';
  $handler->display->display_options['menu']['weight'] = '21';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: PHP Table */
  $handler = $view->new_display('page', 'PHP Table', 'table');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'field_place' => 'field_place',
    'title' => 'title',
    'geocluster_lat' => 'geocluster_lat',
    'geocluster_lon' => 'geocluster_lon',
    'geocluster_count' => 'geocluster_count',
    'geocluster_ids' => 'geocluster_ids',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'field_place' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
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
    'geocluster_ids' => array(
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
    'algorithm' => 'geocluster_php',
    'cluster_field' => 'field_place',
    'cluster_distance' => '65',
  );
  $handler->display->display_options['path'] = 'php-table';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'PHP  table';
  $handler->display->display_options['menu']['weight'] = '31';
  $handler->display->display_options['menu']['name'] = 'main-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $export['geocluster_test_php'] = $view;

  return $export;
}
