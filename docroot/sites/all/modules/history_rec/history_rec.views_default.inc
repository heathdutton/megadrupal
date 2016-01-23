<?php

/**
 * Implements hook_views_default_views().
 */
function history_rec_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'browsing_history_recommender_my_viewed_contents';
  $view->description = 'Display the list of nodes I have viewed.';
  $view->tag = 'recommender';
  $view->base_table = 'history_rec_data';
  $view->human_name = 'Browsing History Recommender: My Viewed Contents';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My Viewed Contents';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Recommender: Browsing History Recommender: item in preference table */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'history_rec_data';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Recommender: Browsing History Recommender: timestamp in preference table */
  $handler->display->display_options['sorts']['updated']['id'] = 'updated';
  $handler->display->display_options['sorts']['updated']['table'] = 'history_rec_data';
  $handler->display->display_options['sorts']['updated']['field'] = 'updated';
  $handler->display->display_options['sorts']['updated']['order'] = 'DESC';
  $handler->display->display_options['sorts']['updated']['granularity'] = 'minute';
  /* Contextual filter: Recommender: Browsing History Recommender: user in preference table */
  $handler->display->display_options['arguments']['euid']['id'] = 'euid';
  $handler->display->display_options['arguments']['euid']['table'] = 'history_rec_data';
  $handler->display->display_options['arguments']['euid']['field'] = 'euid';
  $handler->display->display_options['arguments']['euid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['euid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['euid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['euid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['euid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'nid';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;


  $view = new view();
  $view->name = 'browsing_history_recommender_similar_nodes';
  $view->description = 'Display "Users who viewed this node also viewed..."';
  $view->tag = 'recommender';
  $view->base_table = 'history_rec_item_similarity';
  $view->human_name = 'Browsing History Recommender: Similar Nodes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Users who viewed this also viewed';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Recommender: Browsing History Recommender: item 2 in item similarity table */
  $handler->display->display_options['relationships']['eid2']['id'] = 'eid2';
  $handler->display->display_options['relationships']['eid2']['table'] = 'history_rec_item_similarity';
  $handler->display->display_options['relationships']['eid2']['field'] = 'eid2';
  $handler->display->display_options['relationships']['eid2']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'eid2';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Recommender: Browsing History Recommender: score in item similarity table */
  $handler->display->display_options['sorts']['score']['id'] = 'score';
  $handler->display->display_options['sorts']['score']['table'] = 'history_rec_item_similarity';
  $handler->display->display_options['sorts']['score']['field'] = 'score';
  $handler->display->display_options['sorts']['score']['order'] = 'DESC';
  /* Contextual filter: Recommender: Browsing History Recommender: item 1 in item similarity table */
  $handler->display->display_options['arguments']['eid1']['id'] = 'eid1';
  $handler->display->display_options['arguments']['eid1']['table'] = 'history_rec_item_similarity';
  $handler->display->display_options['arguments']['eid1']['field'] = 'eid1';
  $handler->display->display_options['arguments']['eid1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['eid1']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['eid1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['eid1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['eid1']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'eid2';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Recommender: Browsing History Recommender: score in item similarity table */
  $handler->display->display_options['filters']['score']['id'] = 'score';
  $handler->display->display_options['filters']['score']['table'] = 'history_rec_item_similarity';
  $handler->display->display_options['filters']['score']['field'] = 'score';
  $handler->display->display_options['filters']['score']['operator'] = '>=';
  $handler->display->display_options['filters']['score']['value']['value'] = '0.1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;


  $view = new view();
  $view->name = 'browsing_history_recommender_my_recommendations';
  $view->description = 'Display the list of nodes I might be interested in based on my browsing history.';
  $view->tag = 'recommender';
  $view->base_table = 'history_rec_prediction';
  $view->human_name = 'Browsing History Recommender: My Recommendations';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My Recommendations';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Recommender: Browsing History Recommender: item in prediction table */
  $handler->display->display_options['relationships']['eid']['id'] = 'eid';
  $handler->display->display_options['relationships']['eid']['table'] = 'history_rec_prediction';
  $handler->display->display_options['relationships']['eid']['field'] = 'eid';
  $handler->display->display_options['relationships']['eid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'eid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Recommender: Browsing History Recommender: score in prediction table */
  $handler->display->display_options['sorts']['score']['id'] = 'score';
  $handler->display->display_options['sorts']['score']['table'] = 'history_rec_prediction';
  $handler->display->display_options['sorts']['score']['field'] = 'score';
  $handler->display->display_options['sorts']['score']['order'] = 'DESC';
  /* Contextual filter: Recommender: Browsing History Recommender: user in prediction table */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'history_rec_prediction';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'eid';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Recommender: Browsing History Recommender: score in prediction table */
  $handler->display->display_options['filters']['score']['id'] = 'score';
  $handler->display->display_options['filters']['score']['table'] = 'history_rec_prediction';
  $handler->display->display_options['filters']['score']['field'] = 'score';
  $handler->display->display_options['filters']['score']['operator'] = '>=';
  $handler->display->display_options['filters']['score']['value']['value'] = '0.1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;

  return $views;
}
