<?php

/**
 * @file
 * Providing extra functionality for the LeagueGame UI via views.
 */

/**
 * Implements hook_views_default_views().
 */
function league_game_views_default_views() {
  $views = array();

$view = new view();
$view->name = 'league_games';
$view->description = 'An administrative list of League Games';
$view->tag = 'league';
$view->base_table = 'league_game';
$view->human_name = 'League Games';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'League Games';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = '50';
$handler->display->display_options['style_plugin'] = 'table';
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['label'] = 'Empty ';
$handler->display->display_options['empty']['area']['content'] = 'No league_games have been created yet';
/* Field: LeagueGame: Leaguegame ID */
$handler->display->display_options['fields']['league_game_id']['id'] = 'league_game_id';
$handler->display->display_options['fields']['league_game_id']['table'] = 'league_game';
$handler->display->display_options['fields']['league_game_id']['field'] = 'league_game_id';
$handler->display->display_options['fields']['league_game_id']['label'] = 'ID';
/* Field: LeagueGame: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'league_game';
$handler->display->display_options['fields']['type']['field'] = 'type';
/* Field: LeagueGame: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'league_game';
$handler->display->display_options['fields']['name']['field'] = 'name';
/* Field: LeagueGame: Stadium */
$handler->display->display_options['fields']['stadium']['id'] = 'stadium';
$handler->display->display_options['fields']['stadium']['table'] = 'league_game';
$handler->display->display_options['fields']['stadium']['field'] = 'stadium';
/* Field: LeagueGame: Matchday */
$handler->display->display_options['fields']['matchday']['id'] = 'matchday';
$handler->display->display_options['fields']['matchday']['table'] = 'league_game';
$handler->display->display_options['fields']['matchday']['field'] = 'matchday';
/* Field: LeagueGame: Team B */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'league_game';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['label'] = 'Status';
/* Field: LeagueGame: Team A */
$handler->display->display_options['fields']['team_a']['id'] = 'team_a';
$handler->display->display_options['fields']['team_a']['table'] = 'league_game';
$handler->display->display_options['fields']['team_a']['field'] = 'team_a';
/* Field: LeagueGame: Score_a */
$handler->display->display_options['fields']['score_a']['id'] = 'score_a';
$handler->display->display_options['fields']['score_a']['table'] = 'league_game';
$handler->display->display_options['fields']['score_a']['field'] = 'score_a';
$handler->display->display_options['fields']['score_a']['label'] = 'Score A';
/* Field: LeagueGame: Score_b */
$handler->display->display_options['fields']['score_b']['id'] = 'score_b';
$handler->display->display_options['fields']['score_b']['table'] = 'league_game';
$handler->display->display_options['fields']['score_b']['field'] = 'score_b';
$handler->display->display_options['fields']['score_b']['label'] = 'Score B';
/* Field: LeagueGame: Team B */
$handler->display->display_options['fields']['team_b']['id'] = 'team_b';
$handler->display->display_options['fields']['team_b']['table'] = 'league_game';
$handler->display->display_options['fields']['team_b']['field'] = 'team_b';
/* Field: LeagueGame: Link */
$handler->display->display_options['fields']['link_league_game']['id'] = 'link_league_game';
$handler->display->display_options['fields']['link_league_game']['table'] = 'league_game';
$handler->display->display_options['fields']['link_league_game']['field'] = 'link_league_game';
/* Field: LeagueGame: Edit Link */
$handler->display->display_options['fields']['edit_league_game']['id'] = 'edit_league_game';
$handler->display->display_options['fields']['edit_league_game']['table'] = 'league_game';
$handler->display->display_options['fields']['edit_league_game']['field'] = 'edit_league_game';
$handler->display->display_options['fields']['edit_league_game']['label'] = 'Edit';
/* Field: LeagueGame: Edit Result Link */
$handler->display->display_options['fields']['edit_result_league_game']['id'] = 'edit_result_league_game';
$handler->display->display_options['fields']['edit_result_league_game']['table'] = 'league_game';
$handler->display->display_options['fields']['edit_result_league_game']['field'] = 'edit_result_league_game';
$handler->display->display_options['fields']['edit_result_league_game']['label'] = 'Edit result';
/* Field: LeagueGame: Delete Link */
$handler->display->display_options['fields']['delete_league_game']['id'] = 'delete_league_game';
$handler->display->display_options['fields']['delete_league_game']['table'] = 'league_game';
$handler->display->display_options['fields']['delete_league_game']['field'] = 'delete_league_game';
$handler->display->display_options['fields']['delete_league_game']['label'] = 'Delete';
/* Filter criterion: LeagueGame: Status */
$handler->display->display_options['filters']['status_1']['id'] = 'status_1';
$handler->display->display_options['filters']['status_1']['table'] = 'league_game';
$handler->display->display_options['filters']['status_1']['field'] = 'status';
$handler->display->display_options['filters']['status_1']['group'] = 1;
$handler->display->display_options['filters']['status_1']['exposed'] = TRUE;
$handler->display->display_options['filters']['status_1']['expose']['operator_id'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['label'] = 'Status';
$handler->display->display_options['filters']['status_1']['expose']['operator'] = 'status_1_op';
$handler->display->display_options['filters']['status_1']['expose']['identifier'] = 'status_1';
$handler->display->display_options['filters']['status_1']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);
/* Filter criterion: LeagueGame: Type */
$handler->display->display_options['filters']['type_2']['id'] = 'type_2';
$handler->display->display_options['filters']['type_2']['table'] = 'league_game';
$handler->display->display_options['filters']['type_2']['field'] = 'type';
$handler->display->display_options['filters']['type_2']['group'] = 1;
$handler->display->display_options['filters']['type_2']['exposed'] = TRUE;
$handler->display->display_options['filters']['type_2']['expose']['operator_id'] = 'type_2_op';
$handler->display->display_options['filters']['type_2']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type_2']['expose']['operator'] = 'type_2_op';
$handler->display->display_options['filters']['type_2']['expose']['identifier'] = 'type_2';
$handler->display->display_options['filters']['type_2']['expose']['remember_roles'] = array(
  2 => '2',
  1 => 0,
  3 => 0,
);

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'league_persons_admin_page');
$handler->display->display_options['path'] = 'admin/content/league_games/list';
$handler->display->display_options['menu']['type'] = 'default tab';
$handler->display->display_options['menu']['title'] = 'List';
$handler->display->display_options['menu']['weight'] = '-10';
$handler->display->display_options['tab_options']['type'] = 'tab';
$handler->display->display_options['tab_options']['title'] = 'League Game';
$handler->display->display_options['tab_options']['description'] = 'Manage league_games';
$handler->display->display_options['tab_options']['weight'] = '0';
$handler->display->display_options['tab_options']['name'] = 'management';

  $views[] = $view;
  return $views;

}
