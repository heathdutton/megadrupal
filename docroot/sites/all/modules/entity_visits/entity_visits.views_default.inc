<?php

/**
 * @file
 * entity_visits.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function entity_visits_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'user_visits';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'User Visits';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Who\'s viewed your user';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'list';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: User: Visitor of Entity */
  $handler->display->display_options['relationships']['entity_visitor']['id'] = 'entity_visitor';
  $handler->display->display_options['relationships']['entity_visitor']['table'] = 'users';
  $handler->display->display_options['relationships']['entity_visitor']['field'] = 'entity_visitor';
  $handler->display->display_options['relationships']['entity_visitor']['required'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Contextual filter: Entity Visits: Viewed Entity */
  $handler->display->display_options['arguments']['eid']['id'] = 'eid';
  $handler->display->display_options['arguments']['eid']['table'] = 'entity_visits';
  $handler->display->display_options['arguments']['eid']['field'] = 'eid';
  $handler->display->display_options['arguments']['eid']['relationship'] = 'entity_visitor';
  $handler->display->display_options['arguments']['eid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['eid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['eid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['eid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['eid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Entity Visits: Date */
  $handler->display->display_options['filters']['date']['id'] = 'date';
  $handler->display->display_options['filters']['date']['table'] = 'entity_visits';
  $handler->display->display_options['filters']['date']['field'] = 'date';
  $handler->display->display_options['filters']['date']['relationship'] = 'entity_visitor';
  $handler->display->display_options['filters']['date']['operator'] = '>=';
  $handler->display->display_options['filters']['date']['value']['value'] = '-7 days';
  $handler->display->display_options['filters']['date']['value']['type'] = 'offset';

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $translatables['user_visits'] = array(
    t('Master'),
    t('Who\'s viewed your user'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Entity Visits'),
    t('All'),
    t('Block'),
  );

  $export['user_visits'] = $view;

  return $export;
}
