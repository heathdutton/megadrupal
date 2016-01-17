<?php

/**
 * Implements hook_views_default_views().
 */
function fivestar_rec_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'fivestar_recommender_users_who_liked_this_also_liked';
  $view->description = 'This view shows a list of nodes recommendations on node\'s page based on users votes.';
  $view->tag = 'recommender';
  $view->base_table = 'fivestar_rec_similarity';
  $view->human_name = 'Fivestar Recommender: Users Who Liked This Also Liked';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Users Who Liked This Also Liked';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Recommender: Fivestar Recommender: item 2 in item similarity table */
  $handler->display->display_options['relationships']['eid2']['id'] = 'eid2';
  $handler->display->display_options['relationships']['eid2']['table'] = 'fivestar_rec_similarity';
  $handler->display->display_options['relationships']['eid2']['field'] = 'eid2';
  $handler->display->display_options['relationships']['eid2']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'eid2';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Recommender: Fivestar Recommender: score in item similarity table */
  $handler->display->display_options['sorts']['score']['id'] = 'score';
  $handler->display->display_options['sorts']['score']['table'] = 'fivestar_rec_similarity';
  $handler->display->display_options['sorts']['score']['field'] = 'score';
  $handler->display->display_options['sorts']['score']['order'] = 'DESC';
  /* Contextual filter: Recommender: Fivestar Recommender: item 1 in item similarity table */
  $handler->display->display_options['arguments']['eid1']['id'] = 'eid1';
  $handler->display->display_options['arguments']['eid1']['table'] = 'fivestar_rec_similarity';
  $handler->display->display_options['arguments']['eid1']['field'] = 'eid1';
  $handler->display->display_options['arguments']['eid1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['eid1']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['eid1']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['eid1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['eid1']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Recommender: Fivestar Recommender: score in item similarity table */
  $handler->display->display_options['filters']['score']['id'] = 'score';
  $handler->display->display_options['filters']['score']['table'] = 'fivestar_rec_similarity';
  $handler->display->display_options['filters']['score']['field'] = 'score';
  $handler->display->display_options['filters']['score']['operator'] = '>=';
  $handler->display->display_options['filters']['score']['value']['value'] = '0.1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'fivestar_recommender_recommended_for_you';
  $view->description = 'Shows a list of nodes to the current user based on the user\'s votes.';
  $view->tag = 'recommender';
  $view->base_table = 'fivestar_rec_prediction';
  $view->human_name = 'Fivestar Recommender: Recommended For You';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recommended For You';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Recommender: Fivestar Recommender: item in prediction table */
  $handler->display->display_options['relationships']['eid']['id'] = 'eid';
  $handler->display->display_options['relationships']['eid']['table'] = 'fivestar_rec_prediction';
  $handler->display->display_options['relationships']['eid']['field'] = 'eid';
  $handler->display->display_options['relationships']['eid']['required'] = TRUE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'eid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Recommender: Fivestar Recommender: score in prediction table */
  $handler->display->display_options['sorts']['score']['id'] = 'score';
  $handler->display->display_options['sorts']['score']['table'] = 'fivestar_rec_prediction';
  $handler->display->display_options['sorts']['score']['field'] = 'score';
  $handler->display->display_options['sorts']['score']['order'] = 'DESC';
  $handler->display->display_options['sorts']['score']['expose']['label'] = 'Fivestar Recommender: score in prediction table';
  /* Contextual filter: Recommender: Fivestar Recommender: user in prediction table */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'fivestar_rec_prediction';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Recommender: Fivestar Recommender: score in prediction table */
  $handler->display->display_options['filters']['score']['id'] = 'score';
  $handler->display->display_options['filters']['score']['table'] = 'fivestar_rec_prediction';
  $handler->display->display_options['filters']['score']['field'] = 'score';
  $handler->display->display_options['filters']['score']['operator'] = '>=';
  $handler->display->display_options['filters']['score']['value']['value'] = '50';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'eid';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $views[$view->name] = $view;

  return $views;
}
