<?php

/**
 * Implements hook_views_default_views().
 */
function cm_aspera_views_default_views() {

	$view = new view();
	$view->name = 'aspera_show_rss_lookup';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'Aspera: Show RSS Lookup';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'Aspera Show RSS Lookup';
	$handler->display->display_options['use_more_always'] = FALSE;
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'fields';
	/* No results behavior: Global: Text area */
	$handler->display->display_options['empty']['area']['id'] = 'area';
	$handler->display->display_options['empty']['area']['table'] = 'views';
	$handler->display->display_options['empty']['area']['field'] = 'area';
	$handler->display->display_options['empty']['area']['empty'] = TRUE;
	$handler->display->display_options['empty']['area']['content'] = 'not found';
	$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
	/* Field: Content: Cablecast Show ID */
	$handler->display->display_options['fields']['field_cablecast_show_id']['id'] = 'field_cablecast_show_id';
	$handler->display->display_options['fields']['field_cablecast_show_id']['table'] = 'field_data_field_cablecast_show_id';
	$handler->display->display_options['fields']['field_cablecast_show_id']['field'] = 'field_cablecast_show_id';
	$handler->display->display_options['fields']['field_cablecast_show_id']['label'] = '';
	$handler->display->display_options['fields']['field_cablecast_show_id']['alter']['text'] = '-[field_cablecast_show_id]-';
	$handler->display->display_options['fields']['field_cablecast_show_id']['element_label_colon'] = FALSE;
	$handler->display->display_options['fields']['field_cablecast_show_id']['settings'] = array(
		'thousand_separator' => '',
		'prefix_suffix' => 0,
	);
	/* Field: Content: Title */
	$handler->display->display_options['fields']['title']['id'] = 'title';
	$handler->display->display_options['fields']['title']['table'] = 'node';
	$handler->display->display_options['fields']['title']['field'] = 'title';
	$handler->display->display_options['fields']['title']['label'] = '';
	$handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
	$handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
	$handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
	$handler->display->display_options['fields']['title']['link_to_node'] = FALSE;
	/* Field: Content: Received Filename */
	$handler->display->display_options['fields']['field_expected_filename']['id'] = 'field_expected_filename';
	$handler->display->display_options['fields']['field_expected_filename']['table'] = 'field_data_field_expected_filename';
	$handler->display->display_options['fields']['field_expected_filename']['field'] = 'field_expected_filename';
	$handler->display->display_options['fields']['field_expected_filename']['label'] = '';
	$handler->display->display_options['fields']['field_expected_filename']['element_label_colon'] = FALSE;
	/* Field: Content: Format */
	$handler->display->display_options['fields']['field_expected_file_format']['id'] = 'field_expected_file_format';
	$handler->display->display_options['fields']['field_expected_file_format']['table'] = 'field_data_field_expected_file_format';
	$handler->display->display_options['fields']['field_expected_file_format']['field'] = 'field_expected_file_format';
	$handler->display->display_options['fields']['field_expected_file_format']['label'] = '';
	$handler->display->display_options['fields']['field_expected_file_format']['element_label_colon'] = FALSE;
	/* Field: Content: Author uid */
	$handler->display->display_options['fields']['uid']['id'] = 'uid';
	$handler->display->display_options['fields']['uid']['table'] = 'node';
	$handler->display->display_options['fields']['uid']['field'] = 'uid';
	$handler->display->display_options['fields']['uid']['label'] = '';
	$handler->display->display_options['fields']['uid']['element_label_colon'] = FALSE;
	/* Field: Content: Post date */
	$handler->display->display_options['fields']['created']['id'] = 'created';
	$handler->display->display_options['fields']['created']['table'] = 'node';
	$handler->display->display_options['fields']['created']['field'] = 'created';
	$handler->display->display_options['fields']['created']['label'] = '';
	$handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
	$handler->display->display_options['fields']['created']['date_format'] = 'long';
	/* Field: Content: Nid */
	$handler->display->display_options['fields']['nid']['id'] = 'nid';
	$handler->display->display_options['fields']['nid']['table'] = 'node';
	$handler->display->display_options['fields']['nid']['field'] = 'nid';
	$handler->display->display_options['fields']['nid']['label'] = '';
	$handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
	/* Sort criterion: Content: Post date */
	$handler->display->display_options['sorts']['created']['id'] = 'created';
	$handler->display->display_options['sorts']['created']['table'] = 'node';
	$handler->display->display_options['sorts']['created']['field'] = 'created';
	$handler->display->display_options['sorts']['created']['order'] = 'DESC';
	/* Contextual filter: Content: Received Filename (field_expected_filename) */
	$handler->display->display_options['arguments']['field_expected_filename_value']['id'] = 'field_expected_filename_value';
	$handler->display->display_options['arguments']['field_expected_filename_value']['table'] = 'field_data_field_expected_filename';
	$handler->display->display_options['arguments']['field_expected_filename_value']['field'] = 'field_expected_filename_value';
	$handler->display->display_options['arguments']['field_expected_filename_value']['default_action'] = 'default';
	$handler->display->display_options['arguments']['field_expected_filename_value']['default_argument_type'] = 'raw';
	$handler->display->display_options['arguments']['field_expected_filename_value']['default_argument_options']['index'] = '1';
	$handler->display->display_options['arguments']['field_expected_filename_value']['summary']['number_of_records'] = '0';
	$handler->display->display_options['arguments']['field_expected_filename_value']['summary']['format'] = 'default_summary';
	$handler->display->display_options['arguments']['field_expected_filename_value']['summary_options']['items_per_page'] = '25';
	$handler->display->display_options['arguments']['field_expected_filename_value']['limit'] = '0';
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 1;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
	/* Filter criterion: Content: Type */
	$handler->display->display_options['filters']['type']['id'] = 'type';
	$handler->display->display_options['filters']['type']['table'] = 'node';
	$handler->display->display_options['filters']['type']['field'] = 'type';
	$handler->display->display_options['filters']['type']['value'] = array(
		'cm_show' => 'cm_show',
	);

	/* Display: Feed */
	$handler = $view->new_display('feed', 'Feed', 'feed_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'rss';
	$handler->display->display_options['row_plugin'] = 'rss_fields';
	$handler->display->display_options['row_options']['title_field'] = 'field_cablecast_show_id';
	$handler->display->display_options['row_options']['link_field'] = 'field_cablecast_show_id';
	$handler->display->display_options['row_options']['description_field'] = 'field_expected_file_format';
	$handler->display->display_options['row_options']['creator_field'] = 'uid';
	$handler->display->display_options['row_options']['date_field'] = 'created';
	$handler->display->display_options['row_options']['guid_field_options'] = array(
		'guid_field' => 'field_cablecast_show_id',
		'guid_field_is_permalink' => 0,
	);
	$handler->display->display_options['path'] = 'aspera-show-rss-lookup/%';
	$handler->display->display_options['sitename_title'] = 0;

	/* Display: Data export */
	$handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
	$handler->display->display_options['pager']['type'] = 'some';
	$handler->display->display_options['style_plugin'] = 'views_data_export_csv';
	$handler->display->display_options['style_options']['provide_file'] = 0;
	$handler->display->display_options['style_options']['parent_sort'] = 0;
	$handler->display->display_options['style_options']['quote'] = 0;
	$handler->display->display_options['style_options']['trim'] = 0;
	$handler->display->display_options['style_options']['replace_newlines'] = 0;
	$handler->display->display_options['style_options']['header'] = 0;
	$handler->display->display_options['defaults']['fields'] = FALSE;
	/* Field: Content: Cablecast Show ID */
	$handler->display->display_options['fields']['field_cablecast_show_id']['id'] = 'field_cablecast_show_id';
	$handler->display->display_options['fields']['field_cablecast_show_id']['table'] = 'field_data_field_cablecast_show_id';
	$handler->display->display_options['fields']['field_cablecast_show_id']['field'] = 'field_cablecast_show_id';
	$handler->display->display_options['fields']['field_cablecast_show_id']['label'] = '';
	$handler->display->display_options['fields']['field_cablecast_show_id']['alter']['text'] = '-[field_cablecast_show_id]-';
	$handler->display->display_options['fields']['field_cablecast_show_id']['element_label_colon'] = FALSE;
	$handler->display->display_options['fields']['field_cablecast_show_id']['settings'] = array(
		'thousand_separator' => '',
		'prefix_suffix' => 0,
	);
	/* Field: Content: Format */
	$handler->display->display_options['fields']['field_expected_file_format']['id'] = 'field_expected_file_format';
	$handler->display->display_options['fields']['field_expected_file_format']['table'] = 'field_data_field_expected_file_format';
	$handler->display->display_options['fields']['field_expected_file_format']['field'] = 'field_expected_file_format';
	$handler->display->display_options['fields']['field_expected_file_format']['label'] = '';
	$handler->display->display_options['fields']['field_expected_file_format']['element_label_colon'] = FALSE;
	$handler->display->display_options['path'] = 'aspera-show-csv-lookup/%';

  $views['cm_aspera'] = $view;
  
  return $views;
}