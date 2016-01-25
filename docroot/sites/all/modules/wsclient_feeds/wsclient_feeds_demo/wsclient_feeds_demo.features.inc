<?php
/**
 * @file
 * wsclient_feeds_demo.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function wsclient_feeds_demo_ctools_plugin_api() {
  return array("version" => "1");
}

/**
 * Implements hook_node_info().
 */
function wsclient_feeds_demo_node_info() {
  $items = array(
    'game' => array(
      'name' => t('Game'),
      'base' => 'node_content',
      'description' => t('Description of a football game, who, where, when and results. Based on \'tGameInfo\' schema.'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
    'team' => array(
      'name' => t('Team'),
      'base' => 'node_content',
      'description' => t('A Football team'),
      'has_title' => '1',
      'title_label' => t('Name'),
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_default_wsclient_service().
 */
function wsclient_feeds_demo_default_wsclient_service() {
  $items = array();
  $items['football_results'] = entity_import('wsclient_service', '{
    "settings" : [],
    "operations" : {
      "AllPlayerNames" : {
        "label" : "AllPlayerNames",
        "result" : { "type" : "AllPlayerNamesResponse", "label" : "AllPlayerNamesResponse" },
        "parameter" : { "parameters" : { "type" : "AllPlayerNames" } }
      },
      "AllDefenders" : {
        "label" : "AllDefenders",
        "result" : { "type" : "AllDefendersResponse", "label" : "AllDefendersResponse" },
        "parameter" : { "parameters" : { "type" : "AllDefenders" } }
      },
      "AllGoalKeepers" : {
        "label" : "AllGoalKeepers",
        "result" : { "type" : "AllGoalKeepersResponse", "label" : "AllGoalKeepersResponse" },
        "parameter" : { "parameters" : { "type" : "AllGoalKeepers" } }
      },
      "AllForwards" : {
        "label" : "AllForwards",
        "result" : { "type" : "AllForwardsResponse", "label" : "AllForwardsResponse" },
        "parameter" : { "parameters" : { "type" : "AllForwards" } }
      },
      "AllMidFields" : {
        "label" : "AllMidFields",
        "result" : { "type" : "AllMidFieldsResponse", "label" : "AllMidFieldsResponse" },
        "parameter" : { "parameters" : { "type" : "AllMidFields" } }
      },
      "TopGoalScorers" : {
        "label" : "TopGoalScorers",
        "result" : { "type" : "TopGoalScorersResponse", "label" : "TopGoalScorersResponse" },
        "parameter" : { "parameters" : { "type" : "TopGoalScorers" } }
      },
      "TopSelectedGoalScorers" : {
        "label" : "TopSelectedGoalScorers",
        "result" : {
          "type" : "TopSelectedGoalScorersResponse",
          "label" : "TopSelectedGoalScorersResponse"
        },
        "parameter" : { "parameters" : { "type" : "TopSelectedGoalScorers" } }
      },
      "AllPlayersWithYellowOrRedCards" : {
        "label" : "AllPlayersWithYellowOrRedCards",
        "result" : {
          "type" : "AllPlayersWithYellowOrRedCardsResponse",
          "label" : "AllPlayersWithYellowOrRedCardsResponse"
        },
        "parameter" : { "parameters" : { "type" : "AllPlayersWithYellowOrRedCards" } }
      },
      "AllPlayersWithYellowCards" : {
        "label" : "AllPlayersWithYellowCards",
        "result" : {
          "type" : "AllPlayersWithYellowCardsResponse",
          "label" : "AllPlayersWithYellowCardsResponse"
        },
        "parameter" : { "parameters" : { "type" : "AllPlayersWithYellowCards" } }
      },
      "AllPlayersWithRedCards" : {
        "label" : "AllPlayersWithRedCards",
        "result" : {
          "type" : "AllPlayersWithRedCardsResponse",
          "label" : "AllPlayersWithRedCardsResponse"
        },
        "parameter" : { "parameters" : { "type" : "AllPlayersWithRedCards" } }
      },
      "AllCards" : {
        "label" : "AllCards",
        "result" : { "type" : "AllCardsResponse", "label" : "AllCardsResponse" },
        "parameter" : { "parameters" : { "type" : "AllCards" } }
      },
      "Cities" : {
        "label" : "Cities",
        "result" : { "type" : "CitiesResponse", "label" : "CitiesResponse" },
        "parameter" : { "parameters" : { "type" : "Cities" } }
      },
      "StadiumNames" : {
        "label" : "StadiumNames",
        "result" : { "type" : "StadiumNamesResponse", "label" : "StadiumNamesResponse" },
        "parameter" : { "parameters" : { "type" : "StadiumNames" } }
      },
      "StadiumURL" : {
        "label" : "StadiumURL",
        "result" : { "type" : "StadiumURLResponse", "label" : "StadiumURLResponse" },
        "parameter" : { "parameters" : { "type" : "StadiumURL" } }
      },
      "StadiumInfo" : {
        "label" : "StadiumInfo",
        "result" : { "type" : "StadiumInfoResponse", "label" : "StadiumInfoResponse" },
        "parameter" : { "parameters" : { "type" : "StadiumInfo" } }
      },
      "AllStadiumInfo" : {
        "label" : "AllStadiumInfo",
        "result" : { "type" : "AllStadiumInfoResponse", "label" : "AllStadiumInfoResponse" },
        "parameter" : { "parameters" : { "type" : "AllStadiumInfo" } }
      },
      "FullTeamInfo" : {
        "label" : "FullTeamInfo",
        "result" : { "type" : "FullTeamInfoResponse", "label" : "FullTeamInfoResponse" },
        "parameter" : { "parameters" : { "type" : "FullTeamInfo" } }
      },
      "Teams" : {
        "label" : "Teams",
        "result" : { "type" : "TeamsResponse", "label" : "TeamsResponse" },
        "parameter" : { "parameters" : { "type" : "Teams" } }
      },
      "GroupCount" : {
        "label" : "GroupCount",
        "result" : { "type" : "GroupCountResponse", "label" : "GroupCountResponse" },
        "parameter" : { "parameters" : { "type" : "GroupCount" } }
      },
      "Groups" : {
        "label" : "Groups",
        "result" : { "type" : "GroupsResponse", "label" : "GroupsResponse" },
        "parameter" : { "parameters" : { "type" : "Groups" } }
      },
      "GroupCompetitors" : {
        "label" : "GroupCompetitors",
        "result" : {
          "type" : "GroupCompetitorsResponse",
          "label" : "GroupCompetitorsResponse"
        },
        "parameter" : { "parameters" : { "type" : "GroupCompetitors" } }
      },
      "AllGroupCompetitors" : {
        "label" : "AllGroupCompetitors",
        "result" : {
          "type" : "AllGroupCompetitorsResponse",
          "label" : "AllGroupCompetitorsResponse"
        },
        "parameter" : { "parameters" : { "type" : "AllGroupCompetitors" } }
      },
      "GoalsScored" : {
        "label" : "GoalsScored",
        "result" : { "type" : "GoalsScoredResponse", "label" : "GoalsScoredResponse" },
        "parameter" : { "parameters" : { "type" : "GoalsScored" } }
      },
      "GameInfo" : {
        "label" : "GameInfo",
        "result" : { "type" : "GameInfoResponse", "label" : "GameInfoResponse" },
        "parameter" : { "parameters" : { "type" : "GameInfo" } }
      },
      "AllGames" : {
        "label" : "AllGames",
        "result" : { "type" : "AllGamesResponse", "label" : "AllGamesResponse" },
        "parameter" : { "parameters" : { "type" : "AllGames" } }
      },
      "CountryNames" : {
        "label" : "CountryNames",
        "result" : { "type" : "CountryNamesResponse", "label" : "CountryNamesResponse" },
        "parameter" : { "parameters" : { "type" : "CountryNames" } }
      },
      "GamesPlayed" : {
        "label" : "GamesPlayed",
        "result" : { "type" : "GamesPlayedResponse", "label" : "GamesPlayedResponse" },
        "parameter" : { "parameters" : { "type" : "GamesPlayed" } }
      },
      "GamesPerCity" : {
        "label" : "GamesPerCity",
        "result" : { "type" : "GamesPerCityResponse", "label" : "GamesPerCityResponse" },
        "parameter" : { "parameters" : { "type" : "GamesPerCity" } }
      },
      "YellowCardsTotal" : {
        "label" : "YellowCardsTotal",
        "result" : {
          "type" : "YellowCardsTotalResponse",
          "label" : "YellowCardsTotalResponse"
        },
        "parameter" : { "parameters" : { "type" : "YellowCardsTotal" } }
      },
      "RedCardsTotal" : {
        "label" : "RedCardsTotal",
        "result" : { "type" : "RedCardsTotalResponse", "label" : "RedCardsTotalResponse" },
        "parameter" : { "parameters" : { "type" : "RedCardsTotal" } }
      },
      "YellowAndRedCardsTotal" : {
        "label" : "YellowAndRedCardsTotal",
        "result" : {
          "type" : "YellowAndRedCardsTotalResponse",
          "label" : "YellowAndRedCardsTotalResponse"
        },
        "parameter" : { "parameters" : { "type" : "YellowAndRedCardsTotal" } }
      },
      "GameResultCodes" : {
        "label" : "GameResultCodes",
        "result" : {
          "type" : "GameResultCodesResponse",
          "label" : "GameResultCodesResponse"
        },
        "parameter" : { "parameters" : { "type" : "GameResultCodes" } }
      },
      "Coaches" : {
        "label" : "Coaches",
        "result" : { "type" : "CoachesResponse", "label" : "CoachesResponse" },
        "parameter" : { "parameters" : { "type" : "Coaches" } }
      },
      "PlayedAtWorldCup" : {
        "label" : "PlayedAtWorldCup",
        "result" : {
          "type" : "PlayedAtWorldCupResponse",
          "label" : "PlayedAtWorldCupResponse"
        },
        "parameter" : { "parameters" : { "type" : "PlayedAtWorldCup" } }
      },
      "TeamsCompeteList" : {
        "label" : "TeamsCompeteList",
        "result" : {
          "type" : "TeamsCompeteListResponse",
          "label" : "TeamsCompeteListResponse"
        },
        "parameter" : { "parameters" : { "type" : "TeamsCompeteList" } }
      },
      "NextGame" : {
        "label" : "NextGame",
        "result" : { "type" : "NextGameResponse", "label" : "NextGameResponse" },
        "parameter" : { "parameters" : { "type" : "NextGame" } }
      },
      "DateOfFirstGame" : {
        "label" : "DateOfFirstGame",
        "result" : {
          "type" : "DateOfFirstGameResponse",
          "label" : "DateOfFirstGameResponse"
        },
        "parameter" : { "parameters" : { "type" : "DateOfFirstGame" } }
      },
      "DateOfLastGame" : {
        "label" : "DateOfLastGame",
        "result" : { "type" : "DateOfLastGameResponse", "label" : "DateOfLastGameResponse" },
        "parameter" : { "parameters" : { "type" : "DateOfLastGame" } }
      },
      "DateLastGroupGame" : {
        "label" : "DateLastGroupGame",
        "result" : {
          "type" : "DateLastGroupGameResponse",
          "label" : "DateLastGroupGameResponse"
        },
        "parameter" : { "parameters" : { "type" : "DateLastGroupGame" } }
      },
      "TournamentInfo" : {
        "label" : "TournamentInfo",
        "result" : { "type" : "TournamentInfoResponse", "label" : "TournamentInfoResponse" },
        "parameter" : { "parameters" : { "type" : "TournamentInfo" } }
      },
      "NumberOfGames" : {
        "label" : "NumberOfGames",
        "result" : { "type" : "NumberOfGamesResponse", "label" : "NumberOfGamesResponse" },
        "parameter" : { "parameters" : { "type" : "NumberOfGames" } }
      },
      "SignupsPerDate" : {
        "label" : "SignupsPerDate",
        "result" : { "type" : "SignupsPerDateResponse", "label" : "SignupsPerDateResponse" },
        "parameter" : { "parameters" : { "type" : "SignupsPerDate" } }
      }
    },
    "datatypes" : {
      "tPlayerNames" : {
        "label" : "tPlayerNames",
        "property info" : {
          "iId" : { "type" : "integer" },
          "sName" : { "type" : "text" },
          "sCountryName" : { "type" : "text" },
          "sCountryFlag" : { "type" : "text" },
          "sCountryFlagLarge" : { "type" : "text" }
        }
      },
      "tTopGoalScorer" : {
        "label" : "tTopGoalScorer",
        "property info" : {
          "sName" : { "type" : "text" },
          "iGoals" : { "type" : "integer" },
          "sCountry" : { "type" : "text" },
          "sFlag" : { "type" : "text" },
          "sFlagLarge" : { "type" : "text" }
        }
      },
      "tTopSelectedGoalScorer" : {
        "label" : "tTopSelectedGoalScorer",
        "property info" : {
          "sName" : { "type" : "text" },
          "iSelected" : { "type" : "integer" },
          "Countries" : { "type" : "list\\u003CtCountrySelectedTopScorer\\u003E" }
        }
      },
      "tCountrySelectedTopScorer" : {
        "label" : "tCountrySelectedTopScorer",
        "property info" : { "sName" : { "type" : "text" }, "iCount" : { "type" : "integer" } }
      },
      "tPlayersWithCards" : {
        "label" : "tPlayersWithCards",
        "property info" : {
          "sName" : { "type" : "text" },
          "iYellowCards" : { "type" : "integer" },
          "iRedCards" : { "type" : "integer" },
          "sTeamName" : { "type" : "text" },
          "sTeamFlag" : { "type" : "text" },
          "sTeamFlagLarge" : { "type" : "text" }
        }
      },
      "tCardInfo" : {
        "label" : "tCardInfo",
        "property info" : {
          "dGame" : { "type" : "date" },
          "iMinute" : { "type" : "integer" },
          "sGameTeam1" : { "type" : "text" },
          "sGameTeam2" : { "type" : "text" },
          "sPlayerName" : { "type" : "text" },
          "bYellowCard" : { "type" : "boolean" },
          "bRedCard" : { "type" : "boolean" },
          "sGameTeam1Flag" : { "type" : "text" },
          "sGameTeam1FlagLarge" : { "type" : "text" },
          "sGameTeam2Flag" : { "type" : "text" },
          "sGameTeam2FlagLarge" : { "type" : "text" },
          "sPlayerFlag" : { "type" : "text" },
          "sPlayerFlagLarge" : { "type" : "text" }
        }
      },
      "tStadiumInfo" : {
        "label" : "tStadiumInfo",
        "property info" : {
          "sStadiumName" : { "type" : "text" },
          "iSeatsCapacity" : { "type" : "integer" },
          "sCityName" : { "type" : "text" },
          "sWikipediaURL" : { "type" : "text" },
          "sGoogleMapsURL" : { "type" : "text" }
        }
      },
      "tFullTeamInfo" : {
        "label" : "tFullTeamInfo",
        "property info" : {
          "sName" : { "type" : "text" },
          "sCountryFlag" : { "type" : "text" },
          "sCountryFlagLarge" : { "type" : "text" },
          "sCoach" : { "type" : "text" },
          "iCompeted" : { "type" : "integer" },
          "sGoalKeepers" : { "type" : "list\\u003Cstring\\u003E" },
          "sDefenders" : { "type" : "list\\u003Cstring\\u003E" },
          "sMidFields" : { "type" : "list\\u003Cstring\\u003E" },
          "sForwards" : { "type" : "list\\u003Cstring\\u003E" }
        }
      },
      "tTeamInfo" : {
        "label" : "tTeamInfo",
        "property info" : {
          "iId" : { "type" : "integer" },
          "sName" : { "type" : "text" },
          "sCountryFlag" : { "type" : "text" },
          "sWikipediaURL" : { "type" : "text" },
          "sCountryFlagLarge" : { "type" : "text" }
        }
      },
      "tGroupInfo" : {
        "label" : "tGroupInfo",
        "property info" : { "sCode" : { "type" : "text" }, "sDescription" : { "type" : "text" } }
      },
      "tGroupsCompetitors" : {
        "label" : "tGroupsCompetitors",
        "property info" : {
          "GroupInfo" : { "type" : "tGroupInfo" },
          "TeamsInGroup" : { "type" : "list\\u003CtTeamInfo\\u003E" }
        }
      },
      "tGoal" : {
        "label" : "tGoal",
        "property info" : {
          "dGame" : { "type" : "date" },
          "iMinute" : { "type" : "integer" },
          "sPlayerName" : { "type" : "text" },
          "sTeamName" : { "type" : "text" },
          "sTeamFlag" : { "type" : "text" },
          "sTeamFlagLarge" : { "type" : "text" }
        }
      },
      "tGameInfo" : {
        "label" : "tGameInfo",
        "property info" : {
          "iId" : { "type" : "integer" },
          "sDescription" : { "type" : "text" },
          "dPlayDate" : { "type" : "date" },
          "tPlayTime" : { "type" : "time" },
          "StadiumInfo" : { "type" : "tStadiumInfo" },
          "Team1" : { "type" : "tTeamInfo" },
          "Team2" : { "type" : "tTeamInfo" },
          "sResult" : { "type" : "text" },
          "sScore" : { "type" : "text" },
          "iYellowCards" : { "type" : "integer" },
          "iRedCards" : { "type" : "integer" },
          "Cards" : { "type" : "list\\u003CtGameCard\\u003E" },
          "Goals" : { "type" : "list\\u003CtGoal\\u003E" },
          "bChampion" : { "type" : "boolean" }
        }
      },
      "tGameCard" : {
        "label" : "tGameCard",
        "property info" : {
          "dGame" : { "type" : "date" },
          "iMinute" : { "type" : "integer" },
          "sPlayerName" : { "type" : "text" },
          "bYellowCard" : { "type" : "boolean" },
          "bRedCard" : { "type" : "boolean" },
          "sTeamName" : { "type" : "text" },
          "sTeamFlag" : { "type" : "text" },
          "sTeamFlagLarge" : { "type" : "text" }
        }
      },
      "tCountryInfo" : {
        "label" : "tCountryInfo",
        "property info" : {
          "iId" : { "type" : "integer" },
          "sName" : { "type" : "text" },
          "sIsoCode" : { "type" : "text" },
          "iPersons" : { "type" : "integer" }
        }
      },
      "tCards" : {
        "label" : "tCards",
        "property info" : { "iYellow" : { "type" : "integer" }, "iRed" : { "type" : "integer" } }
      },
      "tGameResultCode" : {
        "label" : "tGameResultCode",
        "property info" : { "sCode" : { "type" : "text" }, "sDescription" : { "type" : "text" } }
      },
      "tCoaches" : {
        "label" : "tCoaches",
        "property info" : { "sName" : { "type" : "text" }, "TeamInfo" : { "type" : "tTeamInfo" } }
      },
      "tTeamCompete" : {
        "label" : "tTeamCompete",
        "property info" : {
          "iCompeted" : { "type" : "integer" },
          "iWon" : { "type" : "integer" },
          "CoachInfo" : { "type" : "tCoaches" }
        }
      },
      "tTournamentInfo" : {
        "label" : "tTournamentInfo",
        "property info" : {
          "dFirstGame" : { "type" : "date" },
          "dLastGame" : { "type" : "date" },
          "dLastGroupDateGame" : { "type" : "date" },
          "iGames" : { "type" : "integer" }
        }
      },
      "tSignupCount" : {
        "label" : "tSignupCount",
        "property info" : {
          "dSignup" : { "type" : "date" },
          "iCount" : { "type" : "integer" },
          "iTotal" : { "type" : "integer" },
          "iAverage" : { "type" : "integer" }
        }
      },
      "ArrayOftCountrySelectedTopScorer" : {
        "label" : "ArrayOftCountrySelectedTopScorer",
        "property info" : { "tCountrySelectedTopScorer" : { "type" : "tCountrySelectedTopScorer" } }
      },
      "ArrayOfString" : {
        "label" : "ArrayOfString",
        "property info" : { "string" : { "type" : "text" } }
      },
      "ArrayOftTeamInfo" : {
        "label" : "ArrayOftTeamInfo",
        "property info" : { "tTeamInfo" : { "type" : "tTeamInfo" } }
      },
      "ArrayOftGameCard" : {
        "label" : "ArrayOftGameCard",
        "property info" : { "tGameCard" : { "type" : "tGameCard" } }
      },
      "ArrayOftGoal" : {
        "label" : "ArrayOftGoal",
        "property info" : { "tGoal" : { "type" : "tGoal" } }
      },
      "ArrayOftPlayerNames" : {
        "label" : "ArrayOftPlayerNames",
        "property info" : { "tPlayerNames" : { "type" : "tPlayerNames" } }
      },
      "ArrayOftTopGoalScorer" : {
        "label" : "ArrayOftTopGoalScorer",
        "property info" : { "tTopGoalScorer" : { "type" : "tTopGoalScorer" } }
      },
      "ArrayOftTopSelectedGoalScorer" : {
        "label" : "ArrayOftTopSelectedGoalScorer",
        "property info" : { "tTopSelectedGoalScorer" : { "type" : "tTopSelectedGoalScorer" } }
      },
      "ArrayOftPlayersWithCards" : {
        "label" : "ArrayOftPlayersWithCards",
        "property info" : { "tPlayersWithCards" : { "type" : "tPlayersWithCards" } }
      },
      "ArrayOftCardInfo" : {
        "label" : "ArrayOftCardInfo",
        "property info" : { "tCardInfo" : { "type" : "tCardInfo" } }
      },
      "ArrayOftStadiumInfo" : {
        "label" : "ArrayOftStadiumInfo",
        "property info" : { "tStadiumInfo" : { "type" : "tStadiumInfo" } }
      },
      "ArrayOftGroupInfo" : {
        "label" : "ArrayOftGroupInfo",
        "property info" : { "tGroupInfo" : { "type" : "tGroupInfo" } }
      },
      "ArrayOftGroupsCompetitors" : {
        "label" : "ArrayOftGroupsCompetitors",
        "property info" : { "tGroupsCompetitors" : { "type" : "tGroupsCompetitors" } }
      },
      "ArrayOftGameInfo" : {
        "label" : "ArrayOftGameInfo",
        "property info" : { "tGameInfo" : { "type" : "tGameInfo" } }
      },
      "ArrayOftCountryInfo" : {
        "label" : "ArrayOftCountryInfo",
        "property info" : { "tCountryInfo" : { "type" : "tCountryInfo" } }
      },
      "ArrayOftGameResultCode" : {
        "label" : "ArrayOftGameResultCode",
        "property info" : { "tGameResultCode" : { "type" : "tGameResultCode" } }
      },
      "ArrayOftCoaches" : {
        "label" : "ArrayOftCoaches",
        "property info" : { "tCoaches" : { "type" : "tCoaches" } }
      },
      "ArrayOftTeamCompete" : {
        "label" : "ArrayOftTeamCompete",
        "property info" : { "tTeamCompete" : { "type" : "tTeamCompete" } }
      },
      "ArrayOftSignupCount" : {
        "label" : "ArrayOftSignupCount",
        "property info" : { "tSignupCount" : { "type" : "tSignupCount" } }
      },
      "AllPlayerNames" : {
        "label" : "AllPlayerNames",
        "property info" : { "bSelected" : { "type" : "boolean" } }
      },
      "AllPlayerNamesResponse" : {
        "label" : "AllPlayerNamesResponse",
        "property info" : { "AllPlayerNamesResult" : { "type" : "list\\u003CtPlayerNames\\u003E" } }
      },
      "AllDefenders" : {
        "label" : "AllDefenders",
        "property info" : { "sCountryName" : { "type" : "text" } }
      },
      "AllDefendersResponse" : {
        "label" : "AllDefendersResponse",
        "property info" : { "AllDefendersResult" : { "type" : "list\\u003Cstring\\u003E" } }
      },
      "AllGoalKeepers" : {
        "label" : "AllGoalKeepers",
        "property info" : { "sCountryName" : { "type" : "text" } }
      },
      "AllGoalKeepersResponse" : {
        "label" : "AllGoalKeepersResponse",
        "property info" : { "AllGoalKeepersResult" : { "type" : "list\\u003Cstring\\u003E" } }
      },
      "AllForwards" : {
        "label" : "AllForwards",
        "property info" : { "sCountryName" : { "type" : "text" } }
      },
      "AllForwardsResponse" : {
        "label" : "AllForwardsResponse",
        "property info" : { "AllForwardsResult" : { "type" : "list\\u003Cstring\\u003E" } }
      },
      "AllMidFields" : {
        "label" : "AllMidFields",
        "property info" : { "sCountryName" : { "type" : "text" } }
      },
      "AllMidFieldsResponse" : {
        "label" : "AllMidFieldsResponse",
        "property info" : { "AllMidFieldsResult" : { "type" : "list\\u003Cstring\\u003E" } }
      },
      "TopGoalScorers" : {
        "label" : "TopGoalScorers",
        "property info" : { "iTopN" : { "type" : "integer" } }
      },
      "TopGoalScorersResponse" : {
        "label" : "TopGoalScorersResponse",
        "property info" : { "TopGoalScorersResult" : { "type" : "list\\u003CtTopGoalScorer\\u003E" } }
      },
      "TopSelectedGoalScorers" : {
        "label" : "TopSelectedGoalScorers",
        "property info" : { "iTopN" : { "type" : "integer" } }
      },
      "TopSelectedGoalScorersResponse" : {
        "label" : "TopSelectedGoalScorersResponse",
        "property info" : { "TopSelectedGoalScorersResult" : { "type" : "list\\u003CtTopSelectedGoalScorer\\u003E" } }
      },
      "AllPlayersWithYellowOrRedCards" : {
        "label" : "AllPlayersWithYellowOrRedCards",
        "property info" : {
          "bSortedByName" : { "type" : "boolean" },
          "bSortedByYellowCards" : { "type" : "boolean" },
          "bSortedByRedCards" : { "type" : "boolean" }
        }
      },
      "AllPlayersWithYellowOrRedCardsResponse" : {
        "label" : "AllPlayersWithYellowOrRedCardsResponse",
        "property info" : { "AllPlayersWithYellowOrRedCardsResult" : { "type" : "list\\u003CtPlayersWithCards\\u003E" } }
      },
      "AllPlayersWithYellowCards" : {
        "label" : "AllPlayersWithYellowCards",
        "property info" : {
          "bSortedByName" : { "type" : "boolean" },
          "bSortedByYellowCards" : { "type" : "boolean" }
        }
      },
      "AllPlayersWithYellowCardsResponse" : {
        "label" : "AllPlayersWithYellowCardsResponse",
        "property info" : { "AllPlayersWithYellowCardsResult" : { "type" : "list\\u003CtPlayersWithCards\\u003E" } }
      },
      "AllPlayersWithRedCards" : {
        "label" : "AllPlayersWithRedCards",
        "property info" : {
          "bSortedByName" : { "type" : "boolean" },
          "bSortedByRedCards" : { "type" : "boolean" }
        }
      },
      "AllPlayersWithRedCardsResponse" : {
        "label" : "AllPlayersWithRedCardsResponse",
        "property info" : { "AllPlayersWithRedCardsResult" : { "type" : "list\\u003CtPlayersWithCards\\u003E" } }
      },
      "AllCards" : { "label" : "AllCards", "property info" : [] },
      "AllCardsResponse" : {
        "label" : "AllCardsResponse",
        "property info" : { "AllCardsResult" : { "type" : "list\\u003CtCardInfo\\u003E" } }
      },
      "Cities" : { "label" : "Cities", "property info" : [] },
      "CitiesResponse" : {
        "label" : "CitiesResponse",
        "property info" : { "CitiesResult" : { "type" : "list\\u003Cstring\\u003E" } }
      },
      "StadiumNames" : { "label" : "StadiumNames", "property info" : [] },
      "StadiumNamesResponse" : {
        "label" : "StadiumNamesResponse",
        "property info" : { "StadiumNamesResult" : { "type" : "list\\u003Cstring\\u003E" } }
      },
      "StadiumURL" : {
        "label" : "StadiumURL",
        "property info" : { "sStadiumName" : { "type" : "text" } }
      },
      "StadiumURLResponse" : {
        "label" : "StadiumURLResponse",
        "property info" : { "StadiumURLResult" : { "type" : "text" } }
      },
      "StadiumInfo" : {
        "label" : "StadiumInfo",
        "property info" : { "sStadiumName" : { "type" : "text" } }
      },
      "StadiumInfoResponse" : {
        "label" : "StadiumInfoResponse",
        "property info" : { "StadiumInfoResult" : { "type" : "tStadiumInfo" } }
      },
      "AllStadiumInfo" : { "label" : "AllStadiumInfo", "property info" : [] },
      "AllStadiumInfoResponse" : {
        "label" : "AllStadiumInfoResponse",
        "property info" : { "AllStadiumInfoResult" : { "type" : "list\\u003CtStadiumInfo\\u003E" } }
      },
      "FullTeamInfo" : {
        "label" : "FullTeamInfo",
        "property info" : { "sTeamName" : { "type" : "text" } }
      },
      "FullTeamInfoResponse" : {
        "label" : "FullTeamInfoResponse",
        "property info" : { "FullTeamInfoResult" : { "type" : "tFullTeamInfo" } }
      },
      "Teams" : { "label" : "Teams", "property info" : [] },
      "TeamsResponse" : {
        "label" : "TeamsResponse",
        "property info" : { "TeamsResult" : { "type" : "list\\u003CtTeamInfo\\u003E" } }
      },
      "GroupCount" : { "label" : "GroupCount", "property info" : [] },
      "GroupCountResponse" : {
        "label" : "GroupCountResponse",
        "property info" : { "GroupCountResult" : { "type" : "integer" } }
      },
      "Groups" : { "label" : "Groups", "property info" : [] },
      "GroupsResponse" : {
        "label" : "GroupsResponse",
        "property info" : { "GroupsResult" : { "type" : "list\\u003CtGroupInfo\\u003E" } }
      },
      "GroupCompetitors" : {
        "label" : "GroupCompetitors",
        "property info" : { "sPoule" : { "type" : "text" } }
      },
      "GroupCompetitorsResponse" : {
        "label" : "GroupCompetitorsResponse",
        "property info" : { "GroupCompetitorsResult" : { "type" : "list\\u003CtTeamInfo\\u003E" } }
      },
      "AllGroupCompetitors" : { "label" : "AllGroupCompetitors", "property info" : [] },
      "AllGroupCompetitorsResponse" : {
        "label" : "AllGroupCompetitorsResponse",
        "property info" : { "AllGroupCompetitorsResult" : { "type" : "list\\u003CtGroupsCompetitors\\u003E" } }
      },
      "GoalsScored" : {
        "label" : "GoalsScored",
        "property info" : { "iGameId" : { "type" : "integer" } }
      },
      "GoalsScoredResponse" : {
        "label" : "GoalsScoredResponse",
        "property info" : { "GoalsScoredResult" : { "type" : "list\\u003CtGoal\\u003E" } }
      },
      "GameInfo" : {
        "label" : "GameInfo",
        "property info" : { "iGameId" : { "type" : "integer" } }
      },
      "GameInfoResponse" : {
        "label" : "GameInfoResponse",
        "property info" : { "GameInfoResult" : { "type" : "tGameInfo" } }
      },
      "AllGames" : { "label" : "AllGames", "property info" : [] },
      "AllGamesResponse" : {
        "label" : "AllGamesResponse",
        "property info" : { "AllGamesResult" : { "type" : "list\\u003CtGameInfo\\u003E" } }
      },
      "CountryNames" : {
        "label" : "CountryNames",
        "property info" : { "bWithCompetitors" : { "type" : "boolean" } }
      },
      "CountryNamesResponse" : {
        "label" : "CountryNamesResponse",
        "property info" : { "CountryNamesResult" : { "type" : "list\\u003CtCountryInfo\\u003E" } }
      },
      "GamesPlayed" : { "label" : "GamesPlayed", "property info" : [] },
      "GamesPlayedResponse" : {
        "label" : "GamesPlayedResponse",
        "property info" : { "GamesPlayedResult" : { "type" : "integer" } }
      },
      "GamesPerCity" : {
        "label" : "GamesPerCity",
        "property info" : { "sCityName" : { "type" : "text" } }
      },
      "GamesPerCityResponse" : {
        "label" : "GamesPerCityResponse",
        "property info" : { "GamesPerCityResult" : { "type" : "list\\u003CtGameInfo\\u003E" } }
      },
      "YellowCardsTotal" : { "label" : "YellowCardsTotal", "property info" : [] },
      "YellowCardsTotalResponse" : {
        "label" : "YellowCardsTotalResponse",
        "property info" : { "YellowCardsTotalResult" : { "type" : "integer" } }
      },
      "RedCardsTotal" : { "label" : "RedCardsTotal", "property info" : [] },
      "RedCardsTotalResponse" : {
        "label" : "RedCardsTotalResponse",
        "property info" : { "RedCardsTotalResult" : { "type" : "integer" } }
      },
      "YellowAndRedCardsTotal" : { "label" : "YellowAndRedCardsTotal", "property info" : [] },
      "YellowAndRedCardsTotalResponse" : {
        "label" : "YellowAndRedCardsTotalResponse",
        "property info" : { "YellowAndRedCardsTotalResult" : { "type" : "tCards" } }
      },
      "GameResultCodes" : { "label" : "GameResultCodes", "property info" : [] },
      "GameResultCodesResponse" : {
        "label" : "GameResultCodesResponse",
        "property info" : { "GameResultCodesResult" : { "type" : "list\\u003CtGameResultCode\\u003E" } }
      },
      "Coaches" : { "label" : "Coaches", "property info" : [] },
      "CoachesResponse" : {
        "label" : "CoachesResponse",
        "property info" : { "CoachesResult" : { "type" : "list\\u003CtCoaches\\u003E" } }
      },
      "PlayedAtWorldCup" : {
        "label" : "PlayedAtWorldCup",
        "property info" : { "sTeamName" : { "type" : "text" } }
      },
      "PlayedAtWorldCupResponse" : {
        "label" : "PlayedAtWorldCupResponse",
        "property info" : { "PlayedAtWorldCupResult" : { "type" : "integer" } }
      },
      "TeamsCompeteList" : { "label" : "TeamsCompeteList", "property info" : [] },
      "TeamsCompeteListResponse" : {
        "label" : "TeamsCompeteListResponse",
        "property info" : { "TeamsCompeteListResult" : { "type" : "list\\u003CtTeamCompete\\u003E" } }
      },
      "NextGame" : { "label" : "NextGame", "property info" : [] },
      "NextGameResponse" : {
        "label" : "NextGameResponse",
        "property info" : { "NextGameResult" : { "type" : "tGameInfo" } }
      },
      "DateOfFirstGame" : { "label" : "DateOfFirstGame", "property info" : [] },
      "DateOfFirstGameResponse" : {
        "label" : "DateOfFirstGameResponse",
        "property info" : { "DateOfFirstGameResult" : { "type" : "date" } }
      },
      "DateOfLastGame" : { "label" : "DateOfLastGame", "property info" : [] },
      "DateOfLastGameResponse" : {
        "label" : "DateOfLastGameResponse",
        "property info" : { "DateOfLastGameResult" : { "type" : "date" } }
      },
      "DateLastGroupGame" : { "label" : "DateLastGroupGame", "property info" : [] },
      "DateLastGroupGameResponse" : {
        "label" : "DateLastGroupGameResponse",
        "property info" : { "DateLastGroupGameResult" : { "type" : "date" } }
      },
      "TournamentInfo" : { "label" : "TournamentInfo", "property info" : [] },
      "TournamentInfoResponse" : {
        "label" : "TournamentInfoResponse",
        "property info" : { "TournamentInfoResult" : { "type" : "tTournamentInfo" } }
      },
      "NumberOfGames" : { "label" : "NumberOfGames", "property info" : [] },
      "NumberOfGamesResponse" : {
        "label" : "NumberOfGamesResponse",
        "property info" : { "NumberOfGamesResult" : { "type" : "integer" } }
      },
      "SignupsPerDate" : { "label" : "SignupsPerDate", "property info" : [] },
      "SignupsPerDateResponse" : {
        "label" : "SignupsPerDateResponse",
        "property info" : { "SignupsPerDateResult" : { "type" : "list\\u003CtSignupCount\\u003E" } }
      }
    },
    "global_parameters" : [],
    "name" : "football_results",
    "label" : "Football Results",
    "url" : "http:\\/\\/footballpool.dataaccess.eu\\/data\\/info.wso?WSDL",
    "type" : "soap",
    "authentication" : null
  }');
  return $items;
}
