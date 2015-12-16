<?php

/**
* @file
* Provides functions for use by the sports_scores module.
*/

/**
 * Given a sport, returns an array of games and scores
 * @param $sport
 */
function sports_scores_fetch_scores($sport) {
  $selected_parser = sports_scores_get_sports($sport);
  if (!is_array($selected_parser['url'])) {
    $selected_parser['url'] = array($selected_parser['url']);
  }
  $scores = array();
  foreach($selected_parser['url'] as $url) {
    $http_result = drupal_http_request($url);
    if ($http_result->code == 200) {
      $data = $http_result->data;
      $url_scores = call_user_func($selected_parser['parser'], $data, $selected_parser);
      $url_scores['sport'] = $sport;
      // If you already have games, just merge the games sub-array.
      if (isset($scores['games'])) {
        $scores['games'] = array_merge($scores['games'], $url_scores['games']);
      }
      else {
        $scores = array_merge($scores, $url_scores);
      }
    }
    else {
      watchdog('sports_scores', 'Fetch of %url resulted in HTTP code %code.', array('%url' => $url, '%code' => $http_result->code), WATCHDOG_WARNING);
    }
  }
  // Parsers may not return any games.
  if (!isset($scores['games'])) {
    $scores['games'] = array();
  }
  return $scores;
}


function sports_scores_load_db_scores($sport) {
  $query = "SELECT gid, gamestart, status_text, status_code, home_team, visiting_team, home_score, visiting_score, sport ";
  $query .= "from {sports_scores_games} WHERE sport = :sport";
  $result = db_query($query, array(':sport' => $sport))->fetchAll(PDO::FETCH_ASSOC);
  $scores = array('sport' => $sport, 'games' => array());
  foreach ($result as $g) {
    $scores['games'][$g['gid']] = get_game_array($g);
  }
  return $scores;
}


/*
 * Get a game array with all of the correct keys.
 */
function get_game_array($game = NULL) {
  $game = array(
    'gid' => isset($game) ? $game['gid'] : NULL,
    'sport' => isset($game) ? $game['sport'] : NULL,
    'home_team' => isset($game) ? $game['home_team'] : NULL,
    'visiting_team' => isset($game) ? $game['visiting_team'] : NULL,
    'home_score' => isset($game) ? $game['home_score'] : NULL,
    'visiting_score' => isset($game) ? $game['visiting_score'] : NULL,
    'gamestart' => isset($game) ? $game['gamestart'] : FALSE,
    'status_text' => isset($game) ? $game['status_text'] : NULL,
    'status_code' => isset($game) ? $game['status_code'] : NULL,
  );

  return $game;
}


function sports_scores_process_game_updates($old, $new) {

  $last_update = 0;
  $sport = '';
  foreach (array_keys($new['games']) as $gid) {

  // Setup some convenience variables.
    $oldgame = isset($old['games'][$gid]) ? $old['games'][$gid] : get_game_array();
    $newgame = $new['games'][$gid];

    /* Don't add or update games without a gametime.
     * Don't update final games already in db.
     * These will come from the espn feed.
     * CRON must be run once before a game goes final or
     * IT WILL NOT BE ADDED!
     */
    if ($newgame['gamestart'] == FALSE || $oldgame['status_code'] > 1) {
      continue;
    }

    // New game, add to database.
    elseif (!isset($old['games'][$gid])) {
      $query = "insert into {sports_scores_games} (gid, sport, gamestart, status_text, status_code, home_team, visiting_team, home_score, visiting_score, last_update) ";
      $query .= "values(:gid, :sport, :gamestart, :status_text, :status_code, :home_team, :visiting_team, :home_score, :visiting_score, :last_update)";
      $result = db_query($query, array(
        ':gid' => $newgame['gid'],
        ':sport' => $newgame['sport'],
        ':gamestart' => $newgame['gamestart'],
        ':status_text' => $newgame['status_text'],
        ':status_code' => $newgame['status_code'],
        ':home_team' => $newgame['home_team'],
        ':visiting_team' => $newgame['visiting_team'],
        ':home_score' => $newgame['home_score'],
        ':visiting_score' => $newgame['visiting_score'],
        ':last_update' => $newgame['last_update'],
      ));

      if ($newgame['status_code'] == 2) {
        // If game is already final, call trigger.
        trigger_sports_scores($newgame, 'game_score_final', $new['sport']);
      }
      elseif ($newgame['status_code'] == 1) {
        // If game is already in progress, call trigger.
        trigger_sports_scores($newgame, 'game_score_update', $new['sport']);
      }
    }

    // Game just went final.
    elseif ($newgame['status_code'] == 2 && ($oldgame['status_code'] != 2)) {
      // Game went final, update the progress fields and scores.
      $query = "UPDATE {sports_scores_games} set status_text=:status_text, status_code=:status_code, home_score=:home_score, visiting_score=:visiting_score, last_update=:last_update ";
      $query .= "where gid=:gid";
      $result = db_query($query, array(
        ':gid' => $newgame['gid'],
        ':status_text' => $newgame['status_text'],
        ':status_code' => $newgame['status_code'],
        ':home_score' => $newgame['home_score'],
        ':visiting_score' => $newgame['visiting_score'],
        ':last_update' => $newgame['last_update'],
      ));

      // Call trigger.
      trigger_sports_scores($newgame, 'game_score_final', $new['sport']);
    }

    // Game was just postponed.
    elseif ($newgame['status_code'] == 3 && ($oldgame['status_code'] != 3)) {
      $query = "UPDATE {sports_scores_games} set status_text=:status_text, status_code=:status_code last_update=:last_update where gid=:gid";
      $result = db_query($query, array(
        ':gid' => $newgame['gid'],
        ':status_text' => $newgame['status_text'],
        ':status_code' => $newgame['status_code'],
        ':last_update' => $newgame['last_update'],
      ));

      // Call trigger.
      // Not sure if postponed should be a FINAL.
      //trigger_sports_scores($newgame, 'game_score_final', $new['sport']);
    }

    // Score changed.
    elseif ($newgame['home_score'] != $oldgame['home_score']
      || $newgame['visiting_score'] != $oldgame['visiting_score']) {
      // Scoring update, update scores only.
      $query = "UPDATE {sports_scores_games} set home_score=:home_score, visiting_score=:visiting_score, last_update=:last_update ";
      $query .= "where gid=:gid";
      $result = db_query($query, array(
        ':gid' => $newgame['gid'],
        ':home_score' => $newgame['home_score'],
        ':visiting_score' => $newgame['visiting_score'],
        ':last_update' => $newgame['last_update'],
      ));

      // Call trigger.
      trigger_sports_scores($newgame, 'game_score_update', $new['sport']);
    }

    // Nothing changed, but still mark the last_update time.
    else {
      $query = "UPDATE {sports_scores_games} set last_update=:last_update where gid=:gid";
      $result = db_query($query, array(
        ':gid' => $newgame['gid'],
        ':last_update' => $newgame['last_update'],
      ));
    }

    // Save a copy of the last_update field for final reconciliation.
    if ($newgame['last_update'] > $last_update) {
      $last_update = $newgame['last_update'];
      $sport = $newgame['sport'];
    }
  }

  /* Delete any non-final games from this sport that have an old date.
   * They were rescheduled.
   */
  $result = db_select('sports_scores_games')
    ->fields('sports_scores_games', array('gid', 'home_team', 'visiting_team'))
    ->condition('sport', $sport)
    ->condition('status_code', 2, '<')
    ->condition('last_update', $last_update, '<')
    ->execute();
  foreach ($result as $game) {
    db_delete('sports_scores_games')->condition('gid', $game->gid)->execute();
    watchdog('sports_scores', 'Game rescheduled, removed old game for %visiting_team @ %home_team.',
      array('%visiting_team' => $game->visiting_team, '%home_team' => $game->home_team), WATCHDOG_WARNING);
  }
}


/*
 *
 * NOTE: Ths scorestrip.json feed does not contain enough game date info for future
 *       games to be added.  Until a game goes 'in-progress' or 'final' the game will not be added to db.
 *
 * Regular Season json has 14 fields.
 * http://www.nfl.com/liveupdate/scorestrip/scorestrip.json
 * [["Sun","1:00","Final",,"TB","21","ATL","27",,,"54840",,"REG9","2010"], ...]
 * array(
 *   0 => Day of week,
 *   1 => Time
 *   2 => Final, Pregame OR final overtime
 *   3 => UNKNOWN
 *   4 => Visiting Team Identifier
 *   5 => Visiting Team Score
 *   6 => Home Team Identifier
 *   7 => Home Team Score
 *   8 => UNKNOWN
 *   9 => UNKNOWN
 *  10 => Game Identifier
 *  11 => UNKNOWN
 *  12 => Week Identifier
 *  13 => Year
 *  Missing: Clock, Ball, Redzone, and game action.
 *
 *
 * Postseason json has 17 fields.
 * http://www.nfl.com/liveupdate/scorestrip/scorestrip.json
 * [["Sun","1:00","Final",,"TB","21","ATL","27",,,"54840",,"REG9","2010"], ...]
 * array(
 *   0 => Day of week,
 *   1 => Time
 *   2 => Final, Pregame OR final overtime
 *   3 => UNKNOWN
 *   4 => Visiting Team Full Name
 *   5 => Visiting Team Abbreviation
 *   6 => Visiting Team Score
 *   7 => Home Team Full Name
 *   8 => Home Team Abbreviation
 *   9 => Home Team Score
 *  10 => UNKNOWN
 *  11 => UNKNOWN
 *  12 => Game Identifier
 *  13 => UNKNOWN
 *  14 => TV Network
 *  15 => Week Identifier
 *  16 => Year
 *
 * Another possible format
 * http://www.nfl.com/liveupdate/scorestrip/ss.xml
 */
function sports_scores_parse_nfl_dot_com_result($json, $selected_parser) {
  $sport = $selected_parser['sport'];
  $rawscores = array();
  $scores = array();


  // Save the original time zone, and set timezone to feed timezone.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($selected_parser['feed_timezone']);

  // NFL.com has invalid json, this fixes it:
  while (strpos($json, ',,') > 0) {
    $json = str_replace(',,', ',null,', $json);
  }
  $rawscores = json_decode($json)->ss;
  $last_update = time();
  foreach ($rawscores as $gameid => $rawgame) {
    $game = get_game_array();
    $game['sport'] = $sport;
    $game['last_update'] = $last_update;

    // Raw feed data for debugging.
    $game['_rawgame'] = $rawgame;

    // Regular Season json.
    if (count($rawgame) == 14) {
      if (strpos(strtolower($rawgame[2]), 'final') !== FALSE) {
        $game['status_text'] = 'FINAL';
        $game['status_code'] = 2;
        $game['home_score'] = $rawgame[7];
        $game['visiting_score'] = $rawgame[5];
      }
      elseif (strpos(strtolower($rawgame[2]), 'pregame') !== FALSE) {
        $game['status_text'] = 'NOT STARTED';
        $game['status_code'] = 0;
      }
      else {
        $game['status_text'] = 'IN PROGRESS';
        $game['status_code'] = 1;
      }
      $game['home_team'] = $rawgame[6];
      $game['visiting_team'] = $rawgame[4];

      if ($game['status_code'] == 1 || $game['status_code'] == 2) {
        $dow = array('Mon' => 'Monday', 'Tue' => 'Tuesday', 'Wed' => 'Wednesday', 'Thu' => 'Thursday', 'Fri' => 'Friday', 'Sat' => 'Saturday', 'Sun' => 'Sunday');
        $game['gamestart'] = strtotime($dow[$rawgame[0]] . ' ' . $rawgame[1]);

        // If the game is final and game start time is in the future, then we've already
        // passed the correct start day.  Backup 7 days.
        if ( $game['gamestart'] > time() && $game['status_code'] == 2) {
          $game['gamestart'] = $game['gamestart'] - (7 * (60*60*24));
        }

        // Human readable game time for debugging.
        $game['_gamestartStr'] = date("F j, Y, g:i a", $game['gamestart']);

      }

      // Two games in one day for the same teams at the same venue would be treated as one game.
      $game['gid'] = md5($sport . ':' . $game['home_team'] . '@' . $game['visiting_team'] . ' ' . $game['gamestart']);
      $scores['last_fetch'] = time();
      $scores['games'][$game['gid']] = $game;
    }

    // Postseason json.
    else {
      if (strpos(strtolower($rawgame[2]), 'final') !== FALSE) {
        $game['status_text'] = 'FINAL';
        $game['status_code'] = 2;
        $game['home_score'] = $rawgame[9];
        $game['visiting_score'] = $rawgame[6];
      }
      elseif (strpos(strtolower($rawgame[2]), 'pregame') !== FALSE) {
        $game['status_text'] = 'NOT STARTED';
        $game['status_code'] = 0;
      }
      else {
        $game['status_text'] = 'IN PROGRESS';
        $game['status_code'] = 1;
      }
      $game['home_team'] = $rawgame[8];
      $game['visiting_team'] = $rawgame[5];

      if ($game['status_code'] == 1 || $game['status_code'] == 2) {
        $dow = array('Mon' => 'Monday', 'Tue' => 'Tuesday', 'Wed' => 'Wednesday', 'Thu' => 'Thursday', 'Fri' => 'Friday', 'Sat' => 'Saturday', 'Sun' => 'Sunday');
        // When the day has passed strtotime will flip to next week's date, so backup appropriately.
        $game['gamestart'] = strtotime($dow[$rawgame[0]] . ' ' . $rawgame[1] . "pm");

        // If the game is final and game start time is in the future, then we've already
        // passed the correct start day.  Backup 7 days.
        if ( $game['gamestart'] > time() && $game['status_code'] == 2) {
          $game['gamestart'] = $game['gamestart'] - (7 * (60*60*24));
        }

        // Human readable game time for debugging.
        $game['_gamestartStr'] = date("F j, Y, g:i a", $game['gamestart']);

      }

      // Two games in one day for the same teams at the same venue would be treated as one game.
      $game['gid'] = md5($sport . ':' . $game['home_team'] . '@' . $game['visiting_team'] . ' ' . $game['gamestart']);
      $scores['last_fetch'] = time();
      $scores['games'][$game['gid']] = $game;
    }
  }

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $scores;
}


/*
 * Need more info on this feed.
 * in_progress/final/gametime don't seem to capture everything.
 * No way to match up a final game with the correct gametime/gid.
 */
function sports_scores_parse_espn_dot_com_result($query_string, $selected_parser) {
  $sport = $selected_parser['sport'];
  $rawscores = array();
  $scores = array();

  // Save the original time zone, and set timezone to feed timezone.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($selected_parser['feed_timezone']);

  parse_str($query_string, $rawscores);
  $last_update = time();
  foreach ($rawscores as $k => $v) {
    $matches = array();
    $game = get_game_array();
    $game['sport'] = $sport;
    $game['last_update'] = $last_update;

    // Raw feed data for debugging.
    $game['_rawgame'] = "$k $v";

    if (strpos($k, '_s_stamp')) {
      $scores['last_fetch'] = time();
    }
    elseif (preg_match('/_s_left(\d+)/', $k, $matches) ) {
      // It's a game.  It's either FINAL, in progress, or scheduled
      // Remove ranking from team name ie '(4) Nebraska'
      $v = preg_replace('/\(\d+\) /', '', $v);
      if (preg_match('/^(.*) at (.*) \((.*) ET\)$/', $v, $matches)) {
        // It's scheduled
        $game = array_merge($game, array(
          'visiting_team' => $matches[1],
          'home_team' => $matches[2],
          'gametime' => $matches[3],
          'status_text' => 'NOT STARTED',
          'status_code' => 0,
        ));
      }
      elseif (preg_match('/^(.*) (\d+)   (.*) (\d+) \((FINAL.*)\)$/', $v, $matches)) {
        // It's final
        $game = array_merge($game, array(
          'visiting_team' => str_replace('^', '', $matches[1]),
          'visiting_score' => $matches[2],
          'home_team' => str_replace('^', '', $matches[3]),
          'home_score' => $matches[4],
          'gametime' => 'FINAL',
          'status_text' => 'FINAL',
          'status_code' => 2,
        ));
      }
      elseif (preg_match('/^(.*) at (.*) \((POSTPONED)\)$/', $v, $matches)) {
        // It's postponed
        $game = array_merge($game, array(
          'visiting_team' => str_replace('^', '', $matches[1]),
          'home_team' => str_replace('^', '', $matches[2]),
          'gametime' => 'POSTPONED',
          'status_text' => 'POSTPONED',
          'status_code' => 3,
        ));
      }
      elseif (preg_match('/^(.*) (\d+)   (.*) (\d+) \((.*)\)$/', $v, $matches)) {
        // In progress
        $game = array_merge($game, array(
          'visiting_team' => $matches[1],
          'visiting_score' => $matches[2],
          'home_team' => $matches[3],
          'home_score' => $matches[4],
          'gametime' => $matches[5],
          'status_text' => 'IN PROGRESS',
          'status_code' => 1,
        ));
      }
      else {
        drupal_set_message("no match");
        watchdog('sports_scores', 'Parse of \'%string\' failed.', array('%string' => $v), WATCHDOG_WARNING);
      }

      $game['gamestart'] = strtotime('today ' . $game['gametime']);
      if ($game['gamestart'] == FALSE) {
        $game['gamestart'] = get_latest_game_start_for_these_teams($game);
      }
      if ($game['gamestart'] != FALSE) {
        // Human readable game time for debugging.
        $game['_gamestartStr'] = date("F j, Y, g:i a", $game['gamestart']);
      }

      // This shouldn't be passed onward.
      unset($game['gametime']);

      $game['gid'] = md5($sport . ':' . $game['home_team'] . '@' . $game['visiting_team'] . ' ' . $game['gamestart']);
      $scores['games'][$game['gid']] = $game;
    }
  }

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $scores;
}


/*
 * Return the latest non-final gamestart for these two teams.
 * This is required because the espn datafeed doesn't have the gamestart
 * after the game begins.
 */
function get_latest_game_start_for_these_teams($game) {
  $query = "SELECT gamestart from {sports_scores_games} WHERE sport = :sport and status_code not in (2,3) and home_team=:home_team and visiting_team=:visiting_team order by gamestart DESC";
  $result = db_query($query, array(':sport' => $game['sport'], ':home_team' => $game['home_team'], ':visiting_team' => $game['visiting_team']))->fetchAll(PDO::FETCH_ASSOC);
  foreach ($result as $g) {
    return $g['gamestart'];
  }
  return FALSE;
}


/*
 * Soccer feed discovered from: http://sports.espn.go.com/espn/bottomline/init
 */
function sports_scores_parse_espn_soccernet_result($query_string, $selected_parser) {
  $sport = $selected_parser['sport'];
  $rawscores = array();
  $scores = array();

  // Save the original time zone, and set timezone to feed timezone.
  $orig_timezone = date_default_timezone_get();
  date_default_timezone_set($selected_parser['feed_timezone']);

  parse_str($query_string, $rawscores);
  $last_update = time();
  foreach ($rawscores as $k => $v) {
    $matches = array();
    $game = get_game_array();
    $game['sport'] = $sport;
    $game['last_update'] = $last_update;

    // Raw feed data for debugging.
    $game['_rawgame'] = "$k $v";

    if (strpos($k, '_s_stamp')) {
      $scores['last_fetch'] = time();
    }

    // It's a game.
    elseif (preg_match('/_s_left(\d+)/', $k, $matches) ) {
      // Remove ranking from team name ie '(4) Madrid'
      $v = preg_replace('/\(\d+\) /', '', $v);
      // Replace html entities for accented characters to normal utf8 chars.
      $v = decode_entities($v);

      // Verified
      if (preg_match('/^(.*) v (.*) \((.*) ET\)$/', $v, $matches)) {
        // It's scheduled
        $game = array_merge($game, array(
          'visiting_team' => $matches[1],
          'home_team' => $matches[2],
          'gametime' => $matches[3],
          'status_text' => 'NOT STARTED',
          'status_code' => 0,
        ));
      }

      // Verified
      elseif (preg_match('/^(.*) (\d+) - (\d+) (.*) \((Full-time.*)\)$/', $v, $matches)) {
        // It's final
        $game = array_merge($game, array(
          'visiting_team' => str_replace('^', '', $matches[1]),
          'visiting_score' => $matches[2],
          'home_score' => $matches[3],
          'home_team' => str_replace('^', '', $matches[4]),
          'gametime' => 'Full-time',
          'status_text' => 'Full-time',
          'status_code' => 2,
        ));
      }

      // Not yet seen.
      elseif (preg_match('/^(.*) at (.*) \((POSTPONED)\)$/', $v, $matches)) {
        // It's postponed
        $game = array_merge($game, array(
          'visiting_team' => str_replace('^', '', $matches[1]),
          'home_team' => str_replace('^', '', $matches[2]),
          'gametime' => 'POSTPONED',
          'status_text' => 'POSTPONED',
          'status_code' => 3,
        ));
      }

      // Not yet seen.
      elseif (preg_match('/^(.*) (\d+) - (\d+) (.*) \((.*)\)$/', $v, $matches)) {
        // In progress
        $game = array_merge($game, array(
          'visiting_team' => $matches[1],
          'visiting_score' => $matches[2],
          'home_team' => $matches[3],
          'home_score' => $matches[4],
          'gametime' => $matches[5],
          'status_text' => 'IN PROGRESS',
          'status_code' => 1,
        ));
      }
      else {
        drupal_set_message("no match");
        watchdog('sports_scores', 'Parse of \'%string\' failed.', array('%string' => $v), WATCHDOG_WARNING);
      }

      $game['gamestart'] = strtotime('today ' . $game['gametime']);
      if ($game['gamestart'] == FALSE) {
        $game['gamestart'] = get_latest_game_start_for_these_teams($game);
      }
      if ($game['gamestart'] != FALSE) {
        // Human readable game time for debugging.
        $game['_gamestartStr'] = date("F j, Y, g:i a", $game['gamestart']);
      }

      // This shouldn't be passed onward.
      unset($game['gametime']);

      $game['gid'] = md5($sport . ':' . $game['home_team'] . '@' . $game['visiting_team'] . ' ' . $game['gamestart']);
      $scores['games'][$game['gid']] = $game;
    }
  }

  // Restore original time zone.
  date_default_timezone_set($orig_timezone);

  return $scores;
}
