<?php

/**
 * @file
 * Views hooks and callback registries.
 */

/**
 * Implements hook_views_default_views().
 */
function easyrec_views_callback_views_default_views(){
	$view = new view;
	$view->name = 'easyrec_recommendations';
	$view->description = '';
	$view->tag = 'default';
	$view->base_table = 'node';
	$view->human_name = 'easyrec_recommendations';
	$view->core = 7;
	$view->api_version = '3.0';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

	/* Display: Master */
	$handler = $view->new_display('default', 'Master', 'default');
	$handler->display->display_options['title'] = 'easyrec_recommendations';
	$handler->display->display_options['access']['type'] = 'perm';
	$handler->display->display_options['cache']['type'] = 'none';
	$handler->display->display_options['query']['type'] = 'views_query';
	$handler->display->display_options['query']['options']['query_comment'] = FALSE;
	$handler->display->display_options['exposed_form']['type'] = 'basic';
	$handler->display->display_options['pager']['type'] = 'none';
	$handler->display->display_options['pager']['options']['offset'] = '0';
	$handler->display->display_options['style_plugin'] = 'default';
	$handler->display->display_options['row_plugin'] = 'node';
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
	/* Contextual filter: Content: Nid */
	$handler->display->display_options['arguments']['nid']['id'] = 'nid';
	$handler->display->display_options['arguments']['nid']['table'] = 'node';
	$handler->display->display_options['arguments']['nid']['field'] = 'nid';
	$handler->display->display_options['arguments']['nid']['default_action'] = 'empty';
	$handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
	$handler->display->display_options['arguments']['nid']['default_argument_skip_url'] = 0;
	$handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
	$handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
	$handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
	$handler->display->display_options['arguments']['nid']['break_phrase'] = 1;
	$handler->display->display_options['arguments']['nid']['not'] = 0;
	/* Filter criterion: Content: Published */
	$handler->display->display_options['filters']['status']['id'] = 'status';
	$handler->display->display_options['filters']['status']['table'] = 'node';
	$handler->display->display_options['filters']['status']['field'] = 'status';
	$handler->display->display_options['filters']['status']['value'] = 1;
	$handler->display->display_options['filters']['status']['group'] = 0;
	$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

	/* Display: Page */
	$handler = $view->new_display('page', 'Page', 'page');
	$handler->display->display_options['path'] = 'easyrec-recommendations';
	
	$views[$view->name] = $view;
	
	return $views;
}