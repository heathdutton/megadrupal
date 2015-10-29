<?php

/**
 * @file
 * Provides the default view for Similar entries module.
 */

/**
 * Implements hook_views_default_views().
 */
function similar_views_default_views() {
  $view = new view;
  $view->name = 'Similar Entries';
  $view->description = t('Provides links to similar content based on relevancy scores using in FULLTEXT searches.');
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Similar Entries';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->is_cacheable = '1';

  // Master display.
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Similar Entries';
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = 21600;
  $handler->display->display_options['cache']['output_lifespan'] = 21600;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = 8;

  // Sets the Similar entries sort handler.
  $handler->display->display_options['sorts']['similarity']['id'] = 'similarity';
  $handler->display->display_options['sorts']['similarity']['table'] = 'similar_entries';
  $handler->display->display_options['sorts']['similarity']['field'] = 'similarity';
  $handler->display->display_options['sorts']['similarity']['order'] = 'DESC';

  // Sets the Similar entries node ID argument.
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'similar_entries';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['override'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = 8;
  $handler->display->display_options['arguments']['nid']['validate_type'] = 'node';
  $handler->display->display_options['arguments']['nid']['validate_argument_type'] = 'nid';
  $handler->display->display_options['arguments']['nid']['validate_argument_nid_type'] = 'nid';
  $handler->display->display_options['arguments']['nid']['validate_fail'] = 'not found';

  // Sets a node title field that links to the node.
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;

  // Provides node->moderated and node->published filters.  
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  // Filter by similarity score.
  $handler->display->display_options['filters']['similarity']['id'] = 'similarity';
  $handler->display->display_options['filters']['similarity']['table'] = 'similar_entries';
  $handler->display->display_options['filters']['similarity']['field'] = 'similarity';
  $handler->display->display_options['filters']['similarity']['value'] = '1';
  $handler->display->display_options['filters']['similarity']['operator'] = '>';
  $handler->display->display_options['filters']['similarity']['group'] = 0;
  $handler->display->display_options['filters']['similarity']['expose']['operator'] = FALSE;

  // Add a block display.
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['block_caching'] = 4;

  return array($view->name => $view);
}
