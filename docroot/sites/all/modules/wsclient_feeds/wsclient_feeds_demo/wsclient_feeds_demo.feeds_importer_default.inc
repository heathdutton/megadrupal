<?php
/**
 * @file
 * wsclient_feeds_demo.feeds_importer_default.inc
 */

/**
 * Implements hook_feeds_importer_default().
 */
function wsclient_feeds_demo_feeds_importer_default() {
  $export = array();

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'full_team_info';
  $feeds_importer->config = array(
    'name' => 'Get Full Team Info',
    'description' => 'Loads up each teams player roster, per country',
    'fetcher' => array(
      'plugin_key' => 'FeedsWSClientFetcher',
      'config' => array(
        'service_operation' => 'football_results:FullTeamInfo',
        'operation_arguments' => array(),
        'raw' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsWSClientParser',
      'config' => array(
        'context' => '|FullTeamInfoResult',
        'sources' => array(),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsUniqueNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'sName',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'sName',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'sGoalKeepers|string',
            'target' => 'field_goalkeepers',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'sDefenders|string',
            'target' => 'field_defenders',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'sMidFields|string',
            'target' => 'field_midfields',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'sForwards|string',
            'target' => 'field_forwards',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'bundle' => 'team',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['full_team_info'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'get_games';
  $feeds_importer->config = array(
    'name' => 'Get Football Games',
    'description' => 'Fetch summaries of all games played',
    'fetcher' => array(
      'plugin_key' => 'FeedsWSClientFetcher',
      'config' => array(
        'service_operation' => 'football_results:AllGames',
        'operation_arguments' => array(),
        'raw' => 0,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsWSClientParser',
      'config' => array(
        'context' => '|AllGamesResult|tGameInfo',
        'sources' => array(),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'sDescription',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'iId',
            'target' => 'guid',
            'unique' => 1,
          ),
          2 => array(
            'source' => 'iId',
            'target' => 'field_id',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'dPlayDate',
            'target' => 'field_playdate:start',
            'unique' => FALSE,
          ),
          4 => array(
            'source' => 'Team1|sName',
            'target' => 'entityreference_lookup:field_team1',
            'unique' => FALSE,
          ),
          5 => array(
            'source' => 'Team2|sName',
            'target' => 'entityreference_lookup:field_team2',
            'unique' => FALSE,
          ),
          6 => array(
            'source' => 'sResult',
            'target' => 'field_result',
            'unique' => FALSE,
          ),
          7 => array(
            'source' => 'StadiumInfo|sStadiumName',
            'target' => 'field_venue',
            'unique' => FALSE,
          ),
          8 => array(
            'source' => 'sScore',
            'target' => 'field_score',
            'unique' => FALSE,
          ),
        ),
        'update_existing' => '2',
        'input_format' => 'plain_text',
        'bundle' => 'game',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['get_games'] = $feeds_importer;

  $feeds_importer = new stdClass();
  $feeds_importer->disabled = FALSE; /* Edit this to true to make a default feeds_importer disabled initially */
  $feeds_importer->api_version = 1;
  $feeds_importer->id = 'get_teams';
  $feeds_importer->config = array(
    'name' => 'Get Football Teams',
    'description' => 'Fetch a list of teams from the web service',
    'fetcher' => array(
      'plugin_key' => 'FeedsWSClientFetcher',
      'config' => array(
        'service_operation' => 'football_results:Teams',
        'operation_arguments' => array(),
        'raw' => FALSE,
      ),
    ),
    'parser' => array(
      'plugin_key' => 'FeedsWSClientParser',
      'config' => array(
        'context' => '|TeamsResult|tTeamInfo',
        'sources' => array(),
      ),
    ),
    'processor' => array(
      'plugin_key' => 'FeedsNodeProcessor',
      'config' => array(
        'expire' => '-1',
        'author' => 0,
        'mappings' => array(
          0 => array(
            'source' => 'sName',
            'target' => 'title',
            'unique' => FALSE,
          ),
          1 => array(
            'source' => 'sCountryFlagLarge',
            'target' => 'field_countryflaglarge:uri',
            'unique' => FALSE,
          ),
          2 => array(
            'source' => 'iId',
            'target' => 'field_id',
            'unique' => FALSE,
          ),
          3 => array(
            'source' => 'sName',
            'target' => 'guid',
            'unique' => 1,
          ),
        ),
        'update_existing' => '0',
        'input_format' => 'plain_text',
        'bundle' => 'team',
      ),
    ),
    'content_type' => '',
    'update' => 0,
    'import_period' => '-1',
    'expire_period' => 3600,
    'import_on_create' => 1,
    'process_in_background' => 0,
  );
  $export['get_teams'] = $feeds_importer;

  return $export;
}
