<?php

/**
 * @file
 * Providing extra functionality for the League Event UI via views.
 */


/**
 * Implements hook_views_data()
 */

function league_event_views_data() {
  $data = array();
  $data['league_event']['table']['group'] = t('League Events');
  $data['league_event']['table']['base'] = array( 
    'field' => 'id', 
    'title' => t('League Events'), 
    'help' => t('LeagueEvents database table'),
  );
  $data['league_event']['table']['join'] = array( 
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
    'league_person' => array( 
      'handler' => 'views_join', // this is actually optional 
      'left_table' => 'league_person', // Because this is a direct link it could be left out. 
      'left_field' => 'league_person_id', 
      'field' => 'person_id',
    ),
    'league_event_type' => array( 
      'handler' => 'views_join', // this is actually optional 
      'left_table' => 'league_event_type', // Because this is a direct link it could be left out. 
      'left_field' => 'type', 
      'field' => 'type',
    ),
  );
  $data['league_event']['game_id'] = array(
    'title' => t('Games'), 
    'help' => t('The game of the event.'),
    'relationship' => array(
        'base' => 'league_game',
        'base field' => 'league_game_id',
        'handler' => 'views_handler_relationship',
        'label' => t('Game'),
      ),
    'field' => array(
       // This one is set only to stablish relationships
      'handler' => 'league_game_handler_field_league_game_id', 
      'click sortable' => FALSE, 
    ),
    'filter' => array(
      'handler' => 'league_game_handler_filter_league_game_id',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['league_event']['person_id'] = array(
    'title' => t('Person'), 
    'help' => t('The person of the event.'),
    'relationship' => array(
        'base' => 'league_person',
        'base field' => 'league_person_id',
        'handler' => 'views_handler_relationship',
        'label' => t('Person'),
      ),
    'field' => array(
       // This one is set only to stablish relationships
      'handler' => 'league_person_handler_field_league_person_id', 
      'click sortable' => FALSE, 
    ),
    'filter' => array(
      'handler' => 'league_person_handler_filter_league_person_id',
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
  );
  $data['league_event']['team_id'] = array(
    'title' => t('Team'), 
    'help' => t('The team of the event.'),
    'relationship' => array(
        'base' => 'node',
        'base field' => 'nid',
        'handler' => 'views_handler_relationship',
        'label' => t('Team'),
      ),
    'field' => array(
      'handler' => 'league_handler_field_league_team_id', 
      'click sortable' => FALSE, 
    ),
    'filter' => array(
      'handler' => 'league_handler_filter_league_team_id',
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
  );
  $data['league_event']['minute'] = array(
    'title' => t('Minute'), 
    'help' => t('The minute the event happenned.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE, 
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ), 
  );
  $data['league_event']['points'] = array(
    'title' => t('Points on event'), 
    'help' => t('Points earned on event.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_event']['type'] = array(
    'title' => t('Type of event'), 
    'help' => t('The type of this event.'),
    'field' => array(
      'handler' => 'league_event_handler_field_league_event_type', 
      'click sortable' => TRUE, 
    ),
    'filter' => array(
      'handler' => 'league_event_handler_filter_league_event_type',
    ),
  );
  $data['league_event']['comment'] = array(
    'title' => t('Comment'), 
    'help' => t('The date the content was last updated.'),
  );

  $data['league_event_type']['table']['group'] = t('League Event Types');
  $data['league_event_type']['table']['base'] = array( 
    'field' => 'id', 
    'title' => t('League Event Types'), 
    'help' => t('LeagueEvent Types database table'),
  );
  $data['league_event_type']['table']['join'] = array( 
    'league_event' => array( 
      'handler' => 'views_join', // this is actually optional 
      'left_table' => 'league_event', // Because this is a direct link it could be left out. 
      'left_field' => 'type', 
      'field' => 'type',
    ),
  );
  $data['league_event_type']['type'] = array(
    'title' => t('Type of event'), 
    'help' => t('The type of this event.'),
    'field' => array(
      'handler' => 'views_handler_field', 
      'click sortable' => TRUE, 
    ),
  );
  $data['league_event_type']['label'] = array(
    'title' => t('Label'), 
    'help' => t('Human readable label.'),
    'field' => array(
      'handler' => 'views_handler_field', 
      'click sortable' => TRUE, 
    ),
  );
  $data['league_event_type']['class_icon'] = array(
    'title' => t('Class Icon'), 
    'help' => t('Class defining icon for event.'),
    'field' => array(
      'handler' => 'views_handler_field', 
      'click sortable' => FALSE, 
    ),
  );
  $data['league_event_type']['public'] = array(
    'title' => t('Public'), 
    'help' => t('If it is a public event.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean', 
      'click sortable' => FALSE, 
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $data['league_event_type']['active'] = array(
    'title' => t('Active'), 
    'help' => t('If the event is active.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean', 
      'click sortable' => FALSE, 
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $data['league_event_type']['published'] = array(
    'title' => t('Published'), 
    'help' => t('If the event is published or not.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean', 
      'click sortable' => FALSE, 
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $data['league_event_type']['weight'] = array(
    'title' => t('Weight'), 
    'help' => t('The weight of the event type for sorting.'),
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
