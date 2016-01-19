<?php

/**
 * @file
 * Provides the default view for Similar Entries Popup module.
 */

/**
 * Implements hook_views_default_views().
 */
function similar_popup_views_default_views() {
  $view = new view();
  $view->name = 'similar_popup';
  $view->description = 'Provides links to similar content based on relevancy scores using in FULLTEXT searches.';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Similar Entries Popup';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Similar Entries';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'time';
  $handler->display->display_options['cache']['results_lifespan'] = 21600;
  $handler->display->display_options['cache']['output_lifespan'] = 21600;
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '3';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  /* Sort criterion: Similar Entries: Similarity score */
  $handler->display->display_options['sorts']['similarity']['id'] = 'similarity';
  $handler->display->display_options['sorts']['similarity']['table'] = 'similar_entries';
  $handler->display->display_options['sorts']['similarity']['field'] = 'similarity';
  /* Contextual filter: Similar Entries: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'similar_entries';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['override'] = TRUE;
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = 8;
  $handler->display->display_options['arguments']['nid']['specify_validation'] = TRUE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 0;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Similar Entries: Similarity score */
  $handler->display->display_options['filters']['similarity']['id'] = 'similarity';
  $handler->display->display_options['filters']['similarity']['table'] = 'similar_entries';
  $handler->display->display_options['filters']['similarity']['field'] = 'similarity';
  $handler->display->display_options['filters']['similarity']['value']['min'] = '1';
  $handler->display->display_options['filters']['similarity']['value']['max'] = '1';
  $handler->display->display_options['filters']['similarity']['group'] = 0;
  $handler->display->display_options['filters']['similarity']['expose']['operator'] = FALSE;
  $translatables['similar_popup'] = array(
    t('Master'),
    t('Similar Entries'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('All'),
  );

  return array($view->name => $view);
}
