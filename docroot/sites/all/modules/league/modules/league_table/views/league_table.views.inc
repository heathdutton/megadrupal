<?php

/**
 * @file
 * Gnerate positioning tables via views.
 */


/**
 * Implements hook_views_data()
 */

function league_table_views_data() {
  $data = array();

  $data['league_table']['table']['group'] = t('League Table');

  $data['league_table']['table']['base'] = array( 
    'field' => 'id', 
    'title' => t('Tables'), 
    'help' => t('League Table database table'),
  );
 $data['league_table']['table']['join'] = array( 
    'node' => array( 
      'handler' => 'views_join', // this is actually optional 
      'left_table' => 'node', // Because this is a direct link it could be left out. 
      'left_field' => 'nid', 
      'field' => 'team_id',
    ),
    'league_game' => array( 
      'handler' => 'views_join', // this is actually optional 
      'left_table' => 'league_game', // Because this is a direct link it could be left out. 
      'left_field' => 'league_game_id', 
      'field' => 'game_id',
    ),
  );
  $data['league_table']['id'] = array(
    'title' => t('Table ID'),
    'help' => '',
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );
  $data['league_table']['game_id'] = array(
    'title' => t('Games'), 
    'help' => t('The games to calculate table.'),
    'relationship' => array(
        'base' => 'league_game',
        'base field' => 'league_game_id',
        'handler' => 'views_handler_relationship',
        'label' => t('Game'),
      ),
    'field' => array(
       // This one is set only to stablish relationships
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_table']['team_id'] = array(
    'title' => t('Team'), 
    'help' => t('The teams on the table.'),
    'relationship' => array(
        'base' => 'node',
        'base field' => 'nid',
        'handler' => 'views_handler_relationship',
        'label' => t('Team'),
      ),
    'field' => array(
      'handler' => 'views_handler_field_node', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_table']['played'] = array(
    'title' => t('Match played'), 
    'help' => t('If the match was played add one.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_table']['win'] = array(
    'title' => t('Match winned'), 
    'help' => t('If the match was winned.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_table']['tie'] = array(
    'title' => t('Match tied'), 
    'help' => t('If the match was tie.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_table']['loose'] = array(
    'title' => t('Match loosed'), 
    'help' => t('If the match was loosed.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_table']['goals'] = array(
    'title' => t('Goals'), 
    'help' => t('Goals in favor minus goals against.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE, 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
  );
  $data['league_table']['favor'] = array(
    'title' => t('Goals in favor'), 
    'help' => t('Goals scored.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE, 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
  );
  $data['league_table']['against'] = array(
    'title' => t('Goals against'), 
    'help' => t('Goals scored against.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE, 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
  );
  $data['league_table']['points'] = array(
    'title' => t('Points for game'), 
    'help' => t('Points this game adds.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE, 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
  );
  
  return $data;
}

/**
 * Implements hook_views_default_views().
 */
function league_table_views_default_views() {
$view = new view();
$view->name = 'positioning_table';
$view->description = 'A generic positioning table';
$view->tag = 'league';
$view->base_table = 'league_table';
$view->human_name = 'Positioning Table';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Positioning Table';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'table';
/* Relationship: League Table: Games */
$handler->display->display_options['relationships']['game_id']['id'] = 'game_id';
$handler->display->display_options['relationships']['game_id']['table'] = 'league_table';
$handler->display->display_options['relationships']['game_id']['field'] = 'game_id';
/* Relationship: League Table: Team */
$handler->display->display_options['relationships']['team_id']['id'] = 'team_id';
$handler->display->display_options['relationships']['team_id']['table'] = 'league_table';
$handler->display->display_options['relationships']['team_id']['field'] = 'team_id';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['relationship'] = 'team_id';
$handler->display->display_options['fields']['title']['label'] = 'Team';
/* Field: SUM(League Table: Match played) */
$handler->display->display_options['fields']['played']['id'] = 'played';
$handler->display->display_options['fields']['played']['table'] = 'league_table';
$handler->display->display_options['fields']['played']['field'] = 'played';
$handler->display->display_options['fields']['played']['group_type'] = 'sum';
$handler->display->display_options['fields']['played']['label'] = 'MP';
/* Field: SUM(League Table: Match winned) */
$handler->display->display_options['fields']['win']['id'] = 'win';
$handler->display->display_options['fields']['win']['table'] = 'league_table';
$handler->display->display_options['fields']['win']['field'] = 'win';
$handler->display->display_options['fields']['win']['group_type'] = 'sum';
$handler->display->display_options['fields']['win']['label'] = 'MW';
/* Field: SUM(League Table: Match tied) */
$handler->display->display_options['fields']['tie']['id'] = 'tie';
$handler->display->display_options['fields']['tie']['table'] = 'league_table';
$handler->display->display_options['fields']['tie']['field'] = 'tie';
$handler->display->display_options['fields']['tie']['group_type'] = 'sum';
$handler->display->display_options['fields']['tie']['label'] = 'MT';
/* Field: SUM(League Table: Match loosed) */
$handler->display->display_options['fields']['loose']['id'] = 'loose';
$handler->display->display_options['fields']['loose']['table'] = 'league_table';
$handler->display->display_options['fields']['loose']['field'] = 'loose';
$handler->display->display_options['fields']['loose']['group_type'] = 'sum';
$handler->display->display_options['fields']['loose']['label'] = 'ML';
/* Field: SUM(League Table: Goals) */
$handler->display->display_options['fields']['goals']['id'] = 'goals';
$handler->display->display_options['fields']['goals']['table'] = 'league_table';
$handler->display->display_options['fields']['goals']['field'] = 'goals';
$handler->display->display_options['fields']['goals']['group_type'] = 'sum';
/* Field: SUM(League Table: Goals in favor) */
$handler->display->display_options['fields']['favor']['id'] = 'favor';
$handler->display->display_options['fields']['favor']['table'] = 'league_table';
$handler->display->display_options['fields']['favor']['field'] = 'favor';
$handler->display->display_options['fields']['favor']['group_type'] = 'sum';
$handler->display->display_options['fields']['favor']['label'] = 'GF';
/* Field: SUM(League Table: Goals against) */
$handler->display->display_options['fields']['against']['id'] = 'against';
$handler->display->display_options['fields']['against']['table'] = 'league_table';
$handler->display->display_options['fields']['against']['field'] = 'against';
$handler->display->display_options['fields']['against']['group_type'] = 'sum';
$handler->display->display_options['fields']['against']['label'] = 'GA';
/* Field: SUM(League Table: Points for game) */
$handler->display->display_options['fields']['points']['id'] = 'points';
$handler->display->display_options['fields']['points']['table'] = 'league_table';
$handler->display->display_options['fields']['points']['field'] = 'points';
$handler->display->display_options['fields']['points']['group_type'] = 'sum';
$handler->display->display_options['fields']['points']['label'] = 'PTS';
/* Sort criterion: SUM(League Table: Points for game) */
$handler->display->display_options['sorts']['points']['id'] = 'points';
$handler->display->display_options['sorts']['points']['table'] = 'league_table';
$handler->display->display_options['sorts']['points']['field'] = 'points';
$handler->display->display_options['sorts']['points']['group_type'] = 'sum';
$handler->display->display_options['sorts']['points']['order'] = 'DESC';
/* Sort criterion: SUM(League Table: Goals) */
$handler->display->display_options['sorts']['goals']['id'] = 'goals';
$handler->display->display_options['sorts']['goals']['table'] = 'league_table';
$handler->display->display_options['sorts']['goals']['field'] = 'goals';
$handler->display->display_options['sorts']['goals']['group_type'] = 'sum';
$handler->display->display_options['sorts']['goals']['order'] = 'DESC';
/* Sort criterion: SUM(League Table: Goals in favor) */
$handler->display->display_options['sorts']['favor']['id'] = 'favor';
$handler->display->display_options['sorts']['favor']['table'] = 'league_table';
$handler->display->display_options['sorts']['favor']['field'] = 'favor';
$handler->display->display_options['sorts']['favor']['group_type'] = 'sum';
$handler->display->display_options['sorts']['favor']['order'] = 'DESC';
/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block');
$handler->display->display_options['block_description'] = 'Positioning Table';

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'positioning_table';

  $views[$view->name] = $view;
  return $views;

}
