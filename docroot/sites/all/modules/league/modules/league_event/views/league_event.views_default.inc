<?php

/**
 * @file
 * Providing extra functionality for the LeagueGame UI via views.
 */

/**
 * Implements hook_views_default_views().
 */
function league_event_views_default_views() {
  $views = array();

$view = new view();
$view->name = 'league_events';
$view->description = 'List of league events';
$view->tag = 'League';
$view->base_table = 'league_event';
$view->human_name = 'League Events';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'League Events';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'label',
    'rendered' => 0,
    'rendered_strip' => 0,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'label' => 'label',
  'minute' => 'minute',
  'person_id' => 'person_id',
  'team_id' => 'team_id',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'label' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'minute' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'person_id' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'team_id' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Field: League Event Types: Label */
$handler->display->display_options['fields']['label']['id'] = 'label';
$handler->display->display_options['fields']['label']['table'] = 'league_event_type';
$handler->display->display_options['fields']['label']['field'] = 'label';
$handler->display->display_options['fields']['label']['label'] = 'Event';
$handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
/* Field: League Events: Points on event */
$handler->display->display_options['fields']['points']['id'] = 'points';
$handler->display->display_options['fields']['points']['table'] = 'league_event';
$handler->display->display_options['fields']['points']['field'] = 'points';
$handler->display->display_options['fields']['points']['group_type'] = 'count';
$handler->display->display_options['fields']['points']['label'] = 'Number';
$handler->display->display_options['fields']['points']['separator'] = '';
/* Sort criterion: League Events: Minute */
$handler->display->display_options['sorts']['minute']['id'] = 'minute';
$handler->display->display_options['sorts']['minute']['table'] = 'league_event';
$handler->display->display_options['sorts']['minute']['field'] = 'minute';
/* Sort criterion: Broken/missing handler */
$handler->display->display_options['sorts']['weight']['id'] = 'weight';
$handler->display->display_options['sorts']['weight']['table'] = 'league_event';
$handler->display->display_options['sorts']['weight']['field'] = 'weight';
/* Contextual filter: League Events: Person */
$handler->display->display_options['arguments']['person_id']['id'] = 'person_id';
$handler->display->display_options['arguments']['person_id']['table'] = 'league_event';
$handler->display->display_options['arguments']['person_id']['field'] = 'person_id';
$handler->display->display_options['arguments']['person_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['person_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['person_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['person_id']['summary_options']['items_per_page'] = '25';
/* Filter criterion: League Event Types: Active */
$handler->display->display_options['filters']['active']['id'] = 'active';
$handler->display->display_options['filters']['active']['table'] = 'league_event_type';
$handler->display->display_options['filters']['active']['field'] = 'active';
$handler->display->display_options['filters']['active']['value'] = '1';
/* Filter criterion: League Event Types: Public */
$handler->display->display_options['filters']['public']['id'] = 'public';
$handler->display->display_options['filters']['public']['table'] = 'league_event_type';
$handler->display->display_options['filters']['public']['field'] = 'public';
$handler->display->display_options['filters']['public']['value'] = '1';
/* Filter criterion: League Event Types: Published */
$handler->display->display_options['filters']['published']['id'] = 'published';
$handler->display->display_options['filters']['published']['table'] = 'league_event_type';
$handler->display->display_options['filters']['published']['field'] = 'published';
$handler->display->display_options['filters']['published']['value'] = '1';

/* Display: Player events */
$handler = $view->new_display('page', 'Player events', 'page');
$handler->display->display_options['display_description'] = 'List events for a single player';
$handler->display->display_options['defaults']['group_by'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'label' => 'label',
  'points' => 'points',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'label' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'points' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: League Event Types: Weight */
$handler->display->display_options['sorts']['weight']['id'] = 'weight';
$handler->display->display_options['sorts']['weight']['table'] = 'league_event_type';
$handler->display->display_options['sorts']['weight']['field'] = 'weight';
$handler->display->display_options['path'] = 'league_person/%/list_events';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'List events';
$handler->display->display_options['menu']['description'] = 'List of events for player';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

/* Display: Game events */
$handler = $view->new_display('page', 'Game events', 'page_1');
$handler->display->display_options['display_description'] = 'List events for a single game';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['grouping'] = array(
  0 => array(
    'field' => 'label',
    'rendered' => 0,
    'rendered_strip' => 1,
  ),
);
$handler->display->display_options['style_options']['columns'] = array(
  'label' => 'label',
  'minute' => 'minute',
  'person_id' => 'person_id',
  'team_id' => 'team_id',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'label' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'minute' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'person_id' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'team_id' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: League Event Types: Label */
$handler->display->display_options['fields']['label']['id'] = 'label';
$handler->display->display_options['fields']['label']['table'] = 'league_event_type';
$handler->display->display_options['fields']['label']['field'] = 'label';
$handler->display->display_options['fields']['label']['label'] = '';
$handler->display->display_options['fields']['label']['exclude'] = TRUE;
$handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
/* Field: League Events: Minute */
$handler->display->display_options['fields']['minute']['id'] = 'minute';
$handler->display->display_options['fields']['minute']['table'] = 'league_event';
$handler->display->display_options['fields']['minute']['field'] = 'minute';
$handler->display->display_options['fields']['minute']['separator'] = '';
/* Field: League Events: Team */
$handler->display->display_options['fields']['team_id']['id'] = 'team_id';
$handler->display->display_options['fields']['team_id']['table'] = 'league_event';
$handler->display->display_options['fields']['team_id']['field'] = 'team_id';
/* Field: League Events: Person */
$handler->display->display_options['fields']['person_id']['id'] = 'person_id';
$handler->display->display_options['fields']['person_id']['table'] = 'league_event';
$handler->display->display_options['fields']['person_id']['field'] = 'person_id';
$handler->display->display_options['defaults']['sorts'] = FALSE;
/* Sort criterion: League Events: Minute */
$handler->display->display_options['sorts']['minute']['id'] = 'minute';
$handler->display->display_options['sorts']['minute']['table'] = 'league_event';
$handler->display->display_options['sorts']['minute']['field'] = 'minute';
/* Sort criterion: League Event Types: Weight */
$handler->display->display_options['sorts']['weight']['id'] = 'weight';
$handler->display->display_options['sorts']['weight']['table'] = 'league_event_type';
$handler->display->display_options['sorts']['weight']['field'] = 'weight';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: League Events: Games */
$handler->display->display_options['arguments']['game_id']['id'] = 'game_id';
$handler->display->display_options['arguments']['game_id']['table'] = 'league_event';
$handler->display->display_options['arguments']['game_id']['field'] = 'game_id';
$handler->display->display_options['arguments']['game_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['game_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['game_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['game_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['path'] = 'league_game/%/list_events';
$handler->display->display_options['menu']['type'] = 'tab';
$handler->display->display_options['menu']['title'] = 'List events';
$handler->display->display_options['menu']['description'] = 'List of events for player';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['context'] = 0;
$handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;
  return $views;

}
