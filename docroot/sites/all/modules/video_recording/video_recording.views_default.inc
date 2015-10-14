<?php

/**
 * Implements hook_views_default_views().
 */
function video_recording_views_default_views() {
  $view = new view();
	$view->name = 'video_recordings';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'video_recording';
	$view->human_name = 'video_recordings';
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
	$handler->display->display_options['pager']['type'] = 'full';
	$handler->display->display_options['style_plugin'] = 'table';
	$handler->display->display_options['style_options']['columns'] = array(
		'video_recording_id' => 'video_recording_id',
		'field_video_recording_title' => 'field_video_recording_title',
	);
	$handler->display->display_options['style_options']['default'] = '-1';
	$handler->display->display_options['style_options']['info'] = array(
		'video_recording_id' => array(
			'sortable' => 0,
			'default_sort_order' => 'asc',
			'align' => '',
			'separator' => '',
			'empty_column' => 0,
		),
		'field_video_recording_title' => array(
			'sortable' => 0,
			'default_sort_order' => 'asc',
			'align' => '',
			'separator' => '',
			'empty_column' => 0,
		),
	);
	/* Field: Video Recording: Video recording ID */
	$handler->display->display_options['fields']['video_recording_id']['id'] = 'video_recording_id';
	$handler->display->display_options['fields']['video_recording_id']['table'] = 'video_recording';
	$handler->display->display_options['fields']['video_recording_id']['field'] = 'video_recording_id';
	$handler->display->display_options['fields']['video_recording_id']['label'] = '';
	$handler->display->display_options['fields']['video_recording_id']['exclude'] = TRUE;
	$handler->display->display_options['fields']['video_recording_id']['element_label_colon'] = FALSE;
	$handler->display->display_options['fields']['video_recording_id']['separator'] = '';
	/* Field: Video Recording: Title */
	$handler->display->display_options['fields']['field_video_recording_title']['id'] = 'field_video_recording_title';
	$handler->display->display_options['fields']['field_video_recording_title']['table'] = 'field_data_field_video_recording_title';
	$handler->display->display_options['fields']['field_video_recording_title']['field'] = 'field_video_recording_title';
	$handler->display->display_options['fields']['field_video_recording_title']['alter']['make_link'] = TRUE;
	$handler->display->display_options['fields']['field_video_recording_title']['alter']['path'] = 'video-recording/[video_recording_id]';
  $views['video_recordings'] = $view;
  
  return $views;
}