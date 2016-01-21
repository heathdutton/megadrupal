<?php

/**
 * Implements hook_views_default_views().
 */
function similarreferences_views_default_views() {
  /*
   * View 'similarreferences'
   */
  $view = new view();
  $view->name = 'similarreferences';
  $view->description = 'Similar By References';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Similar By References';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Related By Reference';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Similar By References: Similarity in Content */
  $handler->display->display_options['fields']['similarreferences']['id'] = 'similarreferences';
  $handler->display->display_options['fields']['similarreferences']['table'] = 'node';
  $handler->display->display_options['fields']['similarreferences']['field'] = 'similarreferences';
  $handler->display->display_options['fields']['similarreferences']['count_type'] = '1';
  /* Sort criterion: Similar By References: Similarity in Content */
  $handler->display->display_options['sorts']['similarreferences']['id'] = 'similarreferences';
  $handler->display->display_options['sorts']['similarreferences']['table'] = 'node';
  $handler->display->display_options['sorts']['similarreferences']['field'] = 'similarreferences';
  /* Contextual filter: Similar By References: Entityreferences in Content */
  $handler->display->display_options['arguments']['similarreferences_argument']['id'] = 'similarreferences_argument';
  $handler->display->display_options['arguments']['similarreferences_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['similarreferences_argument']['field'] = 'similarreferences_argument';
  $handler->display->display_options['arguments']['similarreferences_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['similarreferences_argument']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['similarreferences_argument']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['similarreferences_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['similarreferences_argument']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['similarreferences_argument']['similarreferences']['field_names'] = array();
  $handler->display->display_options['arguments']['similarreferences_argument']['similarreferences']['include_args'] = 0;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  
  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['block_caching'] = '4';
  $views[$view->name] = $view;

  return $views;
}
