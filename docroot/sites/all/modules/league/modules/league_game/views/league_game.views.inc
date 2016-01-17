<?php

/**
 * @file
 * Providing extra functionality for the LeagueGame UI via views.
 */


/**
 * Implements hook_views_data()
 */
function league_game_views_data_alter(&$data) { 
  $data['league_game']['link_league_game'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the league_game.'),
      'handler' => 'league_game_handler_link_field',
    ),
  );
  $data['league_game']['edit_league_game'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the league_game.'),
      'handler' => 'league_game_handler_edit_link_field',
    ),
  );
  $data['league_game']['edit_result_league_game'] = array(
    'field' => array(
      'title' => t('Edit Result Link'),
      'help' => t('Provide a link to the edit result form for the league_game.'),
      'handler' => 'league_game_handler_edit_result_link_field',
    ),
  );
  $data['league_game']['delete_league_game'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the league_game.'),
      'handler' => 'league_game_handler_delete_link_field',
    ),
  );
  $data['league_game']['teams'] = array(
    'title' => t('Teams A and B'), 
    'help' => t('Provide a field for filtering for any game.'),
    // Accepts any team A or B as argument
    'argument' => array(
      'title' => t('Teams A or B'),
      'handler' => 'league_game_handler_argument_teams_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows league_games/league_game/%league_game_id/op
  $data['league_game']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this league_game.'),
      'handler' => 'league_game_handler_league_game_operations_field',
    ),
  );
}
