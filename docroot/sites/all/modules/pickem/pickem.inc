<?php

/**
 * @file
 * includes little helpers functions.
*/

/**
 * Fetch a league object.
 * Also acts as a wildcard loader for hook_menu().
 *
 * @param $league_id
 *   A numeric id of the league to load.
 * @return
 *   A fully-loaded $league object upon successful league load or FALSE if league
 *   cannot be loaded.
 */
function league_load($league_id) {
  $valid = TRUE;
  $league = FALSE;
  if ( is_numeric($league_id) ) {
    $ret = get_leagues($league_id);
    if ( count($ret) > 0 ) {
      $tmp = each($ret);
      $league = $tmp['value'];
      $league->scoring_engine = get_scoring_engine($league);
    }
    else {
      $valid = FALSE;
    }
  }
  return $valid ? $league : FALSE;
}

/**
 * Fetch a team object.
 * Also acts as a wildcard loader for hook_menu().
 *
 * @param $team_id
 *   A numeric id of the team to load.
 * @return
 *   A fully-loaded $team object upon successful team load or FALSE if team
 *   cannot be loaded.
 */
function team_load($team_id) {
  $valid = TRUE;
  $team = FALSE;
  if ( is_numeric($team_id) ) {
    $ret = get_teams(array('team_id' => $team_id));
    if ( count($ret) > 0 ) {
      $tmp = each($ret);
      $team = $tmp['value'];
    }
    else {
      $valid = FALSE;
    }
  }
  return $valid ? $team : FALSE;
}

/**
 * Fetch a game object.
 * Also acts as a wildcard loader for hook_menu().
 *
 * @param $game_id
 *   A numeric id of the game to load.
 * @return
 *   A fully-loaded $game object upon successful game load or FALSE if game
 *   cannot be loaded.
 */
function game_load($game_id) {
  $valid = TRUE;
  $game = FALSE;
  if ( is_numeric($game_id) ) {
    $ret = get_games(array('game_id' => $game_id));
    if ( count($ret) > 0 ) {
      $tmp = each($ret);
      $game = $tmp['value'];
    }
    else {
      $valid = FALSE;
    }
  }
  return $valid ? $game : FALSE;
}

/**
 * Fetch a week object.
 * Also acts as a wildcard loader for hook_menu().
 *
 * @param $week_id
 *   A numeric id of the week to load.
 * @return
 *   A fully-loaded $week object upon successful week load or FALSE if week
 *   cannot be loaded.
 */
function week_load($week_id) {
  $valid = TRUE;
  $week = FALSE;
  if ( is_numeric($week_id) ) {
    $ret =  get_weeks(array('week_id' => $week_id));
    if ( count($ret) > 0 ) {
      $tmp = each($ret);
      $week = $tmp['value'];
    }
    else {
      $valid = FALSE;
    }
  }
  return $valid ? $week : FALSE;
}

/**
 * Fetch a gameset object.
 * Also acts as a wildcard loader for hook_menu().
 *
 * @param $gameset_id
 *   A numeric id of the gameset to load.
 * @return
 *   A fully-loaded $gameset object upon successful gameset load or FALSE if gameset
 *   cannot be loaded.
 */
function gameset_load($gameset_id) {
  $valid = TRUE;
  $gameset = FALSE;
  if ( is_numeric($gameset_id) ) {
    $ret = get_gamesets(array('gameset_id' => $gameset_id));
    if ( count($ret) > 0 ) {
      $tmp = each($ret);
      $gameset = $tmp['value'];
    }
    else {
      $valid = FALSE;
    }
  }
  return $valid ? $gameset : FALSE;
}

/**
 * Perform a check if the league has a particular week in its gameset.
 *
 * @param $league
 *   A league object.
 * @param $week
 *   A week object.
 * @return boolean
 *   TRUE if league contains week, FALSE otherwise.
 */
function is_valid_week($league, $week) {
  $query = "SELECT w.wid from {pickem_leagues} l inner join {pickem_weeks} w on w.gsid=l.gsid WHERE lid=:lid and wid=:wid";
  $valid_wid = db_query($query, array(':lid' => $league->lid, ':wid' => $week->wid))->fetchField();
  if (isset($valid_wid)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function is_last_game_of_week($game, $reset = FALSE) {
  static $lastgames = array();

  // Game can be an array or an object.  Make it always an object.
  $game = (object)$game;

  $cachekey = $game->wid;
  if (empty($lastgames[$cachekey])) {
    $sql = "SELECT gid from {pickem_games} where wid=:wid order by gamestart desc, gid";
    $result = db_query_range($sql, 0, 1, array(':wid' => $game->wid))->fetchAll();
    $row = array_pop($result);
    $lastgames[$cachekey] = $row->gid;
  }
  return $game->gid == $lastgames[$cachekey] ? TRUE : FALSE;
}


function get_weeks( $filter = array('league_id' => NULL, 'gameset_id' => NULL, 'week_id' => NULL, 'playoffs' => NULL, 'started' => NULL) ) {

  $sql_where_list = array();
  if (isset($filter['gameset_id'])) {
    $sql_where_list[] = 'w.gsid=' . $filter['gameset_id'];
  }
  if (isset($filter['week_id'])) {
    $sql_where_list[] = 'w.wid=' . $filter['week_id'];
  }
  if (isset($filter['playoffs'])) {
    $sql_where_list[] = 'w.playoffs=' . $filter['playoffs'];
  }
  if (isset($filter['league_id'])) {
    $sql_where_list[] = 'l.lid=' . $filter['league_id'];
  }
  if (isset($filter['started'])) {
    $sql_where_list[] = 'w.first_gamestart <= ' . time();
  }

  $sql_where = '';
  if ( count($sql_where_list) > 0 ) {
    $sql_where = ' WHERE ' . implode(' and ', $sql_where_list);
  }

  // Only join league table if league_id is specified.
  if (isset($filter['league_id'])) {
    $query = "SELECT w.* from {pickem_weeks} w inner join {pickem_leagues} l on l.gsid=w.gsid " . $sql_where . " ORDER BY w.first_gamestart";
  }
  else {
    $query = "SELECT w.* from {pickem_weeks} w " . $sql_where . " ORDER BY w.first_gamestart";
  }

  $result = db_query($query);
  $weeks = array();
  foreach ($result as $record) {
    $week_started = 0;
    if (time() > $record->first_gamestart && $record->first_gamestart <> 0) {
      $week_started = 1;
    }
    $record->week_started = $week_started;

    $week_ended = 0;
    if (time() > $record->first_gamestart && $record->first_gamestart <> 0) {
      $week_ended = 1;
    }
    $record->week_ended = $week_ended;

    $weeks[$record->wid] = $record;
  }
  return $weeks;
}


function get_users($league=NULL, $not_in_league=0, $full_user_load=0) {

  if ( !isset($league) ) {
    $query = "SELECT u.* from {users} u where u.uid<>0 order by name";
    $result = db_query($query);
  }
  else {
    if ( $not_in_league == 0 ) {
      $query = "SELECT u.* from {users} u inner join {pickem_users} pu on pu.uid=u.uid where u.uid<>0 and u.status<>0 and pu.lid = :lid order by name";
    }
    else {
      $query = "SELECT * from {users} where uid not in (select uid from {pickem_users} where lid = :lid) and uid<>0 and status<>0 order by name";
    }
    $result = db_query($query, array(':lid' => $league->lid));
  }
  $users = array();
  foreach ($result as $record) {
    $users[$record->uid] = $record;
    if ( $full_user_load == 1 ) {
      // Do a full user_load, hooks and all, for RealName module compatibility.
      $users[$record->uid]->user_obj = user_load($record->uid);
    }
    else {
      // Create a user-like object.  This is more performant on large user lists than a full user_load().
      $users[$record->uid]->user_obj = new stdClass;
      $users[$record->uid]->user_obj->name = $record->name;
      $users[$record->uid]->user_obj->uid = $record->uid;
    }
  }
  return $users;
}

function get_leagues($league_id=0) {
  if ( $league_id == 0 ) {
    //all leagues
    $query = "SELECT * from {pickem_leagues} order by name";
    $result = db_query($query);
  }
  else {
    //one league
    $query = "SELECT * from {pickem_leagues} where lid=:lid order by name";
    $result = db_query($query, array(':lid' => $league_id));
  }
  $leagues = array();
  foreach ($result as $record) {
    $leagues[$record->lid] = $record;
  }

  return $leagues;
}
function get_my_leagues($gameset_id=NULL, $user_id=NULL) {

  // use the logged in user if no user_id is passed in.
  if ( is_null($user_id) ) {
    global $user;
    $user_id = $user->uid;
  }

  if ( is_null($gameset_id) ) {
    $query = "SELECT l.* from {pickem_leagues} l inner join {pickem_users} u on u.lid=l.lid where u.uid=:uid";
    $result = db_query($query, array(':uid' => $user_id));
  }
  else {
    $query = "SELECT l.* from {pickem_leagues} l inner join {pickem_users} u on u.lid=l.lid where u.uid=:uid and l.gsid=:gsid";
    $result = db_query($query, array(':uid' => $user_id, ':gsid' => $gameset_id));
  }
  $leagues = array();
  foreach ($result as $record) {
    $leagues[] = league_load($record->lid);
  }
  return $leagues;
}


/**
 * Perform a check if a user is a member of a particular league.
 *
 * @param $league
 *   A league object.
 * @param $account
 *   A user object.
 * @return boolean
 *   TRUE if current user is a league member, FALSE otherwise.
 */
function is_league_member($league, $account) {
  $league_member_id = db_query("SELECT l.lid from {pickem_leagues} l inner join {pickem_users} pu on pu.lid=l.lid where pu.lid=:lid and pu.uid=:uid", array(':lid' => $league->lid, ':uid' => $account->uid))->fetchField();
  if (!empty($league_member_id)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}


/**
 * Perform a check if the current user is a member of a particular league.
 *
 * @param $league
 *   A league object.
 * @return boolean
 *   TRUE if current user is a league member, FALSE otherwise.
 */
function is_my_league($league) {
  global $user;
  return is_league_member($league, $user);
}


function get_teams($filter = array('team_id' => NULL, 'gameset_id' => NULL)) {
  if (isset($filter['gameset_id'])) {
    $query = "SELECT * from {pickem_teams} where gsid=:gsid order by tm_abbrev, tm_name";
    $result = db_query($query, array(':gsid' => $filter['gameset_id']));
  }
  elseif (isset($filter['team_id'])) {
    $query = "SELECT * from {pickem_teams} where tid=:tid order by tm_abbrev, tm_name";
    $result = db_query($query, array(':tid' => $filter['team_id']));
  }
  else {
    $query = "SELECT * from {pickem_teams} order by tm_abbrev, tm_name";
    $result = db_query($query);
  }
  $teams = array();
  $module_path = drupal_get_path('module', 'pickem');
  foreach ($result as $record) {
    $record->short_name = ($record->tm_abbrev <> '') ? $record->tm_abbrev : $record->tm_name;
    $record->long_name = $record->tm_city . ' ' . $record->tm_name;
    if ( $record->tm_abbrev <> '' ) {
      $record->long_name .=  ' (' . $record->tm_abbrev . ')';
    }
    $teams[$record->tid] = $record;
  }
  return $teams;
}
function get_gamesets($filter = array('gameset_id' => NULL, 'gameset_name' => NULL, 'gameset_sport' => NULL)) {
  if (isset($filter['gameset_id'])) {
    $query = "SELECT * from {pickem_gamesets} where gsid=:gsid order by gs_name";
    $result = db_query($query, array(':gsid' => $filter['gameset_id']));
  }
  elseif (isset($filter['gameset_name'])) {
    $query = "SELECT * from {pickem_gamesets} where gs_name=:gs_name order by gs_name";
    $result = db_query($query, array(':gs_name' => $filter['gameset_name']));
  }
  elseif (isset($filter['gameset_sport'])) {
    $query = "SELECT * from {pickem_gamesets} where sport=:sport order by gs_name";
    $result = db_query($query, array(':sport' => $filter['gameset_sport']));
  }
  else {
    //all gamesets
    $query = "SELECT * from {pickem_gamesets} order by gs_name";
    $result = db_query($query);
  }

  $gamesets = array();
  foreach ($result as $record) {
    $gamesets[$record->gsid] = $record;
  }

  return $gamesets;
}


/**
 * Get a list of games.
 *
 * @param $filter
 *   A structured array of key-values to add to the where statement.
 * @param $options
 *   A structured array of key-values to add to the order by and group by statements.
 */
function get_games($filter = array('game_id' => NULL, 'week_id' => NULL, 'gameset_id' => NULL, 'team_id' => NULL, 'league_id' => NULL, 'gamestart' => NULL, 'playoffs' => NULL, 'week_started' => NULL), $options=array('by_week' => 0, 'order_by' => 'week_ended, g.gamestart, g.gid') ) {

  $now_time = time();
  $sql_where_list = array();
  if (isset($filter['game_id'])) {
    $sql_where_list[] = 'g.gid=' . $filter['game_id'];
  }
  if (isset($filter['week_id'])) {
    $sql_where_list[] = 'g.wid=' . $filter['week_id'];
  }
  if (isset($filter['gameset_id'])) {
    $sql_where_list[] = 'pw.gsid=' . $filter['gameset_id'];
  }
  if (isset($filter['league_id'])) {
    $sql_where_list[] = 'l.lid=' . $filter['league_id'];
  }
  if (isset($filter['team_id'])) {
    $sql_where_list[] = '(g.v_id=' . $filter['team_id'] . ' or g.h_id=' . $filter['team_id'] . ')';
  }
  if (isset($filter['gamestart'])) {
    $sql_where_list[] = 'g.gamestart=' . $filter['gamestart'];
  }
  if (isset($filter['playoffs'])) {
    $sql_where_list[] = 'pw.playoffs=' . $filter['playoffs'];
  }
  if (isset($filter['week_started'])) {
    $sql_where_list[] = 'pw.first_gamestart <=' . $now_time;
  }

  $order_by = 'week_ended, g.gamestart, g.gid';
  if (isset($options['order_by']) ) {
    $order_by = $options['order_by'];
  }


  $sql_where = '';
  if ( count($sql_where_list) > 0 ) {
    $sql_where = ' WHERE ' . implode(' and ', $sql_where_list);
  }

  $games = array();

  if (!isset($filter['league_id'])) {
    $query = "SELECT g.*, case when :now_time > g.gamestart then 1 else 0 end as 'game_started', case when :now_time > pw.first_gamestart then 1 else 0 end as 'week_started', case when :now_time > pw.last_gamestart then 1 else 0 end as 'week_ended', pw.first_gamestart, pw.last_gamestart, pw.wk_number FROM {pickem_games} g INNER JOIN {pickem_weeks} pw ON pw.wid=g.wid  " . $sql_where . " ORDER BY $order_by";
  }
  else {
    $query = "SELECT g.*, case when :now_time > g.gamestart then 1 else 0 end as 'game_started', case when :now_time > pw.first_gamestart then 1 else 0 end as 'week_started', case when :now_time > pw.last_gamestart then 1 else 0 end as 'week_ended', pw.first_gamestart, pw.last_gamestart, pw.wk_number FROM {pickem_games} g INNER JOIN {pickem_weeks} pw ON pw.wid=g.wid inner join {pickem_leagues} l on l.gsid=pw.gsid " . $sql_where . " ORDER BY $order_by";
  }

  $result = db_query($query, array(':now_time' => $now_time));
  foreach ($result as $record) {
    if (isset($options['by_week']) &&  $options['by_week'] == 1 ) {
      $games[$record->wid][$record->gid] = $record;
    }
    else {
      $games[$record->gid] = $record;
    }
  }
  return $games;
}

function get_picks($filter = array('league_id' => NULL, 'user_id' => NULL, 'week_id' => NULL) ) {

  if (isset($filter['league_id'])) {
    $sql_where_list[] = 'l.lid=' . $filter['league_id'];
  }
  if (isset($filter['user_id'])) {
    $sql_where_list[] = 'u.uid=' . $filter['user_id'];
  }
  if (isset($filter['week_id'])) {
    $sql_where_list[] = 'w.wid=' . $filter['week_id'];
  }

  $sql_where = '';
  if ( count($sql_where_list) > 0 ) {
    $sql_where = ' WHERE ' . implode(' and ', $sql_where_list);
  }

  $query = 'SELECT g.gid, g.wid, g.h_score, g.v_score, g.v_id, g.h_id, p.uid, p.winnerpick_id, p.pick_v_score, p.pick_h_score, p.pick_v_score, p.pick_h_score, p.confidence, g.gamestart, w.wk_points from {pickem_users} u inner join {pickem_picks} p on u.lid=p.lid and u.uid=p.uid inner join {pickem_games} g on p.gid = g.gid  inner join {pickem_weeks} w on w.wid=g.wid inner join {pickem_leagues} l on l.gsid=w.gsid and l.lid=u.lid '. $sql_where .' ORDER BY g.wid, g.gamestart, g.gid';
  $result = db_query($query);

  $picks = array();
  foreach ($result as $record) {
    $picks[$record->gid][$record->uid] = $record;

    // If the user did not pick the winner, but did pick the score, use the score values.
    if ( !isset($picks[$record->gid][$record->uid]->winnerpick_id) && isset($record->pick_v_score) && isset($record->pick_h_score) ) {
      $picks[$record->gid][$record->uid]->winnerpick_id = $record->pick_h_score > $record->pick_v_score ? $record->h_id : $record->v_id;
    }
  }

  return $picks;
}


function set_pick($user_id, $league, $pick, $context) {

  // If this comes from the user pick screen, check to see if the game has already started.
  if ($context == 'user') {
    $game = game_load($pick['gid']);
    if ($game->game_started == 1) {
      // Don't set any error messages here.  This function is called once for each game and
      // would set many duplicate messages.
      return FALSE;
    }
  }

  // Pick the winner with confidence points type league.
  if ($league->scoring_engine->features['with_confidence'] == 1) {

    db_merge('pickem_picks')
      ->key(array(
        'gid' => $pick['gid'],
        'uid' => $user_id,
        'lid' => $league->lid,
      ))
      ->fields(array(
        'winnerpick_id' => $pick['pick'] == '' ? NULL : $pick['pick'],
        'picktime' => time(),
        'confidence' => $pick['confidence'] == '' ? NULL : $pick['confidence'],
      ))
      ->execute();
  }

   // Pick the winner type league.
  else if (isset($pick['pick'])) {

    db_merge('pickem_picks')
      ->key(array(
        'gid' => $pick['gid'],
        'uid' => $user_id,
        'lid' => $league->lid,
      ))
      ->fields(array(
        'winnerpick_id' => $pick['pick'] == '' ? NULL : $pick['pick'],
        'picktime' => time(),
      ))
      ->execute();
  }

  // Pick the score type league.
  else {

    db_merge('pickem_picks')
      ->key(array(
        'gid' => $pick['gid'],
        'uid' => $user_id,
        'lid' => $league->lid,
      ))
      ->fields(array(
        'winnerpick_id' => NULL,
        'pick_v_score' => $pick['pick_v_score'] == '' ? NULL : $pick['pick_v_score'],
        'pick_h_score' => $pick['pick_h_score'] == '' ? NULL : $pick['pick_h_score'],
        'picktime' => time(),
      ))
      ->execute();
  }
  return TRUE;
}


function get_scoring_engine($league) {

  switch ($league->scoring_type) {
    case PICKEM_TYPE_WIN_LOSS:
      return new pickem_scoring_win_loss();
    case PICKEM_TYPE_WIN_LOSS_TIEBREAK:
      return new pickem_scoring_win_loss_tiebreak();
    case PICKEM_TYPE_SPREAD:
      return new pickem_scoring_point_spread();
    case PICKEM_TYPE_SCORE_DIFF:
      return new pickem_scoring_score_diff();
    case PICKEM_TYPE_CONFIDENCE:
      return new pickem_scoring_confidence();
  }
}


function get_scoring_engine_all() {
  $all_engines = array();
  $i = 0;
  for ($i = 0; $i < PICKEM_SCORING_ENGINE_COUNT; $i++) {
    $league = new stdClass;
    $league->scoring_type = $i;
    $eng = get_scoring_engine($league);
    $all_engines[$eng->key] = $eng;
  }
  return $all_engines;
}


/**
 * Fetch an array containing user point scores for the league.
 * Long running query, uses static variable cache and DB cache where it can.
 * DB cache should be cleared when game scores are updated.
 *
 * @param $league
 *   A $league object for the league in question.
 * @param $week_id
 *   A numeric week id (wid).
 * @param $reset
 *   A boolean for resetting the static variable containing cached return values.
 * @return
 *   A complex array, keyed with uid, containing the users point totals, overall and by week.
 */
function get_user_totals($league=NULL, $week_id=0, $reset = FALSE) {
  static $cache_totals;
  $cache_key = 'pickem_get_user_totals_' . $league->lid . '_' . $week_id;
  if (!isset($cache_totals[$cache_key]) || $reset) {
    if (!$reset && ($cache = cache_get($cache_key)) && !empty($cache->data)) {
      $totals = $cache->data;
    }
    else {

      $totals = array();

      // Initialized $totals array with current league users.
      $league_users = get_users($league, 0, variable_get('pickem_full_user_load', 0));
      foreach ($league_users as $u) {
        $totals[$u->uid] = array(
          'user_obj' => $u,
          'total' => 0,
          'name' => $u->name,
          'best_week_id' => -1,
          'worst_week_id' => -1,
          'picked_games' => 0,
          'ranking' => 0,
          'weeks' => array()
        );
      }

      // Initialize user-weeks.
      $games_by_week = get_games(array('league_id' => $league->lid, 'week_started' => 1), array('by_week' => 1));
      foreach ($league_users as $u) {
        foreach ($games_by_week as $wid => $games) {
          $totals[$u->uid]['weeks'][$wid]['total'] = 0;
          $totals[$u->uid]['weeks'][$wid]['weeks_games'] = count($games);
          $totals[$u->uid]['weeks'][$wid]['games'] = array();
          $totals[$u->uid]['weeks'][$wid]['picked_games'] = 0;
        }
      }

      if ($week_id == 0) {
        // All picked games.
        // Will be missing entries if they didn't pick.
        $query = "select g.gid, g.wid, g.h_score, g.v_score, g.v_id, g.v_spread, g.h_id, g.h_spread, p.uid, p.winnerpick_id, p.pick_v_score, p.pick_h_score, p.confidence, g.gamestart, w.wk_points from {pickem_picks} p inner join {pickem_games} g on p.gid = g.gid inner join {pickem_weeks} w on w.wid = g.wid where p.lid=:lid and w.first_gamestart < :now_time order by g.wid, g.gamestart, g.gid";
        $result = db_query($query, array(':lid' => $league->lid, ':now_time' => time()));
      }
      else {
        // All games for a certain week.
        // Will be missing entries if they didn't pick.
        $query = "select g.gid, g.wid, g.h_score, g.v_score, g.v_id, g.v_spread, g.h_id, g.h_spread, p.uid, p.winnerpick_id, p.pick_v_score, p.pick_h_score, p.confidence, g.gamestart, w.wk_points from {pickem_picks} p inner join {pickem_games} g on p.gid = g.gid inner join {pickem_weeks} w on w.wid = g.wid where p.lid=:lid and w.wid=:wid order by g.wid, g.gamestart, g.gid";
        $result = db_query($query, array(':lid' => $league->lid, ':wid' => $week_id));
      }

      foreach ($result as $record) {
        // If user is not in list, skip them.
        // They were in the league, left, and may return.
        // Also skip if the week hasn't started.
        if (!isset($totals[$record->uid]) || !isset($totals[$record->uid]['weeks'][$record->wid])) {
          continue;
        }

        // Calculate scores for each game picked.
        $league->scoring_engine->update_totals($totals, $record);
      }

      // Some scoring systems need to make modifications after ALL games scored.
      $league->scoring_engine->update_totals_final($totals);

      // Calculate user's best and worst weeks.
      $weekwinners = array();
      foreach ($totals as $uid => $u_object) {
        foreach ($u_object['weeks'] as $wid => $week_item) {
          if ( $totals[$uid]['best_week_id'] == -1 ) {
            $totals[$uid]['best_week_id'] = $wid;
            $totals[$uid]['worst_week_id'] = $wid;
          }
          if ( $week_item['total'] > $totals[$uid]['weeks'][$totals[$uid]['best_week_id']]['total'] ) {
            $totals[$uid]['best_week_id'] = $wid;
          }
          if ( $week_item['total'] < $totals[$uid]['weeks'][$totals[$uid]['worst_week_id']]['total'] ) {
            $totals[$uid]['worst_week_id'] = $wid;
          }
        }
      }

      // Calculate total ordinal ranking.
      uasort($totals, "sort_totals");

      $ordinal = 0;
      $last_ordinal = 0;
      $high_score = 0;
      $last_score = 100000;
      $last_uid = -1;
      foreach ($totals as $uid => $u_object) {
        $ordinal++;
        if ($u_object['total'] < $last_score) {
          $last_ordinal = $ordinal;
          $last_score = $u_object['total'];
          if ($ordinal == 1) {
            $high_score = $last_score;
          }
        }
        else if (isset($totals[$last_uid]['tiebreak_pts']) && isset($totals[$uid]['tiebreak_pts']) &&
          $totals[$uid]['tiebreak_pts'] > $totals[$last_uid]['tiebreak_pts']) {
          $last_ordinal = $ordinal;
        }
        $totals[$uid]['ranking'] = $last_ordinal;
        $totals[$uid]['pts_back'] = $high_score - $last_score;
        $last_uid = $uid;
      }
      cache_set($cache_key, $totals);
    }
    $cache_totals[$cache_key] = $totals;
  }

  return $cache_totals[$cache_key];
}
function sort_totals($a, $b) {
  if ($a['total'] > $b['total']) {
    return -1;
  }
  elseif ($a['total'] < $b['total']) {
      return 1;
  }
  else {
    // Use tie break if available.
    if (isset($a['tiebreak_pts']) && isset($b['tiebreak_pts'])) {
      if ($a['tiebreak_pts'] < $b['tiebreak_pts']) {
        return -1;
      }
      elseif ($a['tiebreak_pts'] > $b['tiebreak_pts']) {
          return 1;
      }
      else {
        return 0;
      }
    }

    if ($a['name'] < $b['name']) {
      return -1;
    }
    elseif ($a['name'] > $b['name']) {
        return 1;
    }
    else {
      return 0;
    }
  }
}

/**
 * Fetch an array containing current week details
 * Often called query, but it is fast enought to not require caching.
 *
 * @param $league
 *   A $league object for the league in question.
 * @param $nextweek
 *   A boolean whether or not to get the next week.
 * @return
 *   A stdClass object containing week info.
 */
function get_current_week($league, $nextweek = TRUE) {
  $now_time = time();
  if (!$league) {
    return;
  }
  if ($nextweek == TRUE) {
    // once you pass the firstgame start you are on the next week
    $result = db_query("SELECT *, first_gamestart - :now_time as diff FROM {pickem_weeks} w inner join {pickem_leagues} l on l.gsid=w.gsid where l.lid=:lid and first_gamestart > :now_time order by first_gamestart", array(':lid' => $league->lid, ':now_time' => $now_time));
  }
  else {
    // ONLY once you pass the firstgame are you on THIS week.
    $result = db_query("SELECT *, first_gamestart - :now_time as diff FROM {pickem_weeks} w inner join {pickem_leagues} l on l.gsid=w.gsid where l.lid=:lid and first_gamestart <= :now_time order by first_gamestart DESC", array(':lid' => $league->lid, ':now_time' => $now_time));
  }

  // There is only one row.
  foreach ($result as $row) {
    $row->days = ($row->diff) / 86400; // days: decimal number
    $row->firstgame_dow = date("l", $row->first_gamestart);
    return $row;
  }
  return FALSE;
}


function get_totals_by_week($league, $week_id = 0, $reset = FALSE) {

  $totals = get_user_totals($league, $week_id = 0, $reset = FALSE);
  $weeks = get_weeks(array('gameset_id' => $league->gsid));
  if (count($weeks) == 0) {
    return array();
  }
  $wk_summary = array();
  foreach ($weeks as $wid => $week) {
    if ( $week->week_started==1 ) {
      $max = 0;
      $min = 100;
      $week_total = 0;
      $count = 0;
      foreach ($totals as $uid => $tot) {
        if ( isset($tot['weeks'][$wid]['total']) ) {
          if ($tot['weeks'][$wid]['total'] > $max) {
            $max = $tot['weeks'][$wid]['total'];
          }
          if ( $tot['weeks'][$wid]['total'] < $min ) {
            $min = $tot['weeks'][$wid]['total'];
          }
          $week_total += $tot['weeks'][$wid]['total'];
          $count += 1;
        }
      }

      $wk_summary[$wid] = array(
        "name" => $week->wk_number,
        "max" => $max,
        "min" => $min,
        "count" => $count,
        "total" => $week_total,
      );
    }
  }
  return $wk_summary;
}


/**
 * Fetch an array containing team win-loss and other statistics.
 * Long running query, uses static variable cache and DB cache where it can.
 * DB cache should be cleared when game scores are updated.
 *
 * @param $league
 *   A $league object for the league in question.
 * @param $sort_by
 *   A string containing the sort item.
 * @param $week_id
 *   A numeric week id (wid).
 * @param $reset
 *   A boolean for resetting the static variable containing cached return values.
 * @return
 *   A complex array, keyed with team id(tid), containing the teams statistics, overall and by week.
 */
function get_team_totals($league, $sort_by='win_lose', $week_id=0, $reset = FALSE) {
  static $cache_totals;
  $cache_key = 'pickem_get_team_totals_' . $league->lid . '_' . $sort_by . '_' . $week_id;

  if (!isset($cache_totals[$cache_key]) || $reset) {
    if (!$reset && ($cache = cache_get($cache_key)) && !empty($cache->data)) {
      $totals = $cache->data;
    }
    else {
      $totals = array();
      $teams = get_teams(array('gameset_id' => $league->gsid));

      foreach ($teams as $tid => $team) {
        $totals[$tid] = array(
          'total_picks' => 0,
          'win_picks' => 0,
          'correct_picks' => 0,
          'pickability' => NULL,
          'pickability_w' => NULL,
          'wins' => 0,
          'abbrev' => $team->tm_abbrev,
          'times_picked' => 0,
          'picked_perc' => NULL,
        );
      }

      $now_time = time();
      if ( $week_id != 0 ) {
        $query = "select g.gid, g.wid, g.h_score, g.v_score, g.v_id, g.h_id, p.uid, p.winnerpick_id, g.gamestart,w.wk_points from {pickem_users} u inner join {pickem_picks} p on u.lid=p.lid and u.uid=p.uid inner join {pickem_games} g on p.gid = g.gid  inner join {pickem_weeks} w on w.wid=g.wid where p.lid = :lid and w.first_gamestart < :now_time and w.wid = :wid order by g.wid, g.gamestart, g.gid";
        $result = db_query($query, array(':lid' => $league->lid, ':wid' => $week_id, ':now_time' => $now_time));
      }
      else {
        $query = "select g.gid, g.wid, g.h_score, g.v_score, g.v_id, g.h_id, p.uid, p.winnerpick_id, g.gamestart,w.wk_points from {pickem_users} u inner join {pickem_picks} p on u.lid=p.lid and u.uid=p.uid inner join {pickem_games} g on p.gid = g.gid  inner join {pickem_weeks} w on w.wid=g.wid where p.lid = :lid and w.first_gamestart < :now_time order by g.wid, g.gamestart, g.gid";
        $result = db_query($query, array(':lid' => $league->lid, ':now_time' => $now_time));
      }
      foreach ($result as $record) {

        // If they have a blank pick, skip.
        if (isset($record->winnerpick_id)) {

          // Home Win, Correct
          if ($record->h_score > $record->v_score && $record->winnerpick_id == $record->h_id) {
            $totals[$record->v_id]['correct_picks'] += 1;
            $totals[$record->h_id]['correct_picks'] += 1;
            $totals[$record->h_id]['win_picks'] += 1;
            $totals[$record->h_id]['wins'] += 1;
          }
          // Visitor Win, Correct
          elseif ($record->v_score > $record->h_score && $record->winnerpick_id == $record->v_id) {
            $totals[$record->v_id]['correct_picks'] += 1;
            $totals[$record->v_id]['win_picks'] += 1;
            $totals[$record->h_id]['correct_picks'] += 1;
            $totals[$record->v_id]['wins'] += 1;
          }
          // Tie
          elseif ($record->v_score == $record->h_score && $record->h_score <> "") {
            $totals[$record->v_id]['correct_picks'] += 1;
            $totals[$record->h_id]['correct_picks'] += 1;
          }

          $totals[$record->v_id]['total_picks'] += 1;
          $totals[$record->h_id]['total_picks'] += 1;

          if ( $record->winnerpick_id == $record->h_id ) {
            $totals[$record->h_id]['times_picked'] += 1;
          }

          if ( $record->winnerpick_id == $record->v_id ) {
            $totals[$record->v_id]['times_picked'] += 1;
          }

          if ( $totals[$record->h_id]['total_picks'] != 0 ) {
            $totals[$record->h_id]['pickability'] = round($totals[$record->h_id]['correct_picks'] / $totals[$record->h_id]['total_picks'] * 100, 1);
            $totals[$record->h_id]['pickability_w'] = round($totals[$record->h_id]['win_picks'] / $totals[$record->h_id]['total_picks'] * 100, 1);
            $totals[$record->h_id]['picked_perc'] = round($totals[$record->h_id]['times_picked'] / $totals[$record->h_id]['total_picks'] * 100, 0);
          }
          if ( $totals[$record->v_id]['total_picks'] != 0 ) {
            $totals[$record->v_id]['pickability'] = round($totals[$record->v_id]['correct_picks'] / $totals[$record->v_id]['total_picks'] * 100, 1);
            $totals[$record->v_id]['pickability_w'] = round($totals[$record->v_id]['win_picks'] / $totals[$record->v_id]['total_picks'] * 100, 1);
            $totals[$record->v_id]['picked_perc'] = round($totals[$record->v_id]['times_picked'] / $totals[$record->v_id]['total_picks'] * 100, 0);
          }
        }
      }
      uasort($totals, "sort_$sort_by");
      cache_set($cache_key, $totals);
    }
    $cache_totals[$cache_key] = $totals;
  }

  return $cache_totals[$cache_key];
}


function sort_win_lose($a, $b) {
  if ( $a['pickability'] > $b['pickability'] || is_null($b['pickability']) ) {
    return -1;
  }
  elseif ( $a['pickability'] < $b['pickability'] || is_null($a['pickability']) ) {
      return 1;
  }
  else {
    if ( $a['abbrev'] < $b['abbrev'] ) {
      return -1;
    }
    elseif ($a['abbrev'] > $b['abbrev']) {
        return 1;
    }
    else {
      return 0;
    }
  }
}
function sort_to_win($a, $b) {
  if ( $a['pickability_w'] > $b['pickability_w'] || is_null($b['pickability_w']) ) {
    return -1;
  }
  elseif ($a['pickability_w'] < $b['pickability_w'] || is_null($a['pickability_w']) ) {
      return 1;
  }
  else {
    if ($a['abbrev'] < $b['abbrev']) {
      return -1;
    }
    elseif ($a['abbrev'] > $b['abbrev']) {
        return 1;
    }
    else {
      return 0;
    }
  }
}


function get_team_stats($league) {

  // team_stats will contain rolling totals for each week.
  $team_stats = array();

  // default values to initialize team data (gets copied)
  $team_def = array(
    'team' => NULL,
    'wins' => 0,
    'losses' => 0,
    'ties' => 0,
    'streak' => 0,
    'games_played' => 0,
    'pts_for' => 0,
    'pts_against' => 0,
    'win_percent' => NULL,

    'opp_win_sum' => 0,
    'scoring_sum' => 0,
    'ord' => 0
  );

  // $team_stats[0] is where the current totals are stored
  $teams = get_teams(array('gameset_id' => $league->gsid));
  foreach ($teams as $tid => $t) {
    // make sure both teams are initialized
    if ( !isset($team_stats[0][$tid]) ) {
      $team_stats[0][$tid] = $team_def;
      $team_stats[0][$tid]['team'] = $t;
    }
  }

  // loop through the games and get the data
  $games = get_games(array('gameset_id' => $league->gsid), array('order_by' => 'pw.first_gamestart,g.gamestart,g.gid'));
  foreach ($games as $gid => $game) {
    // skip unplayed games
    if ( ! isset($game->h_score) || ! isset($game->v_score) ) {
      continue;
    }


    /*
     * get the stats
     */

    // home win
    if ( $game->h_score > $game->v_score ) {
      $team_stats[0][$game->h_id]['wins']++;
      $team_stats[0][$game->v_id]['losses']++;

      // streak calc
      if ( $team_stats[0][$game->h_id]['streak'] >= 0 ) {
        $team_stats[0][$game->h_id]['streak']++;
      }
      else {
        $team_stats[0][$game->h_id]['streak'] = 1;
      }
      if ( $team_stats[0][$game->v_id]['streak'] <= 0 ) {
        $team_stats[0][$game->v_id]['streak']--;
      }
      else {
        $team_stats[0][$game->v_id]['streak'] = -1;
      }

    }

    // visitor win
    elseif ( $game->h_score < $game->v_score ) {
      $team_stats[0][$game->v_id]['wins']++;
      $team_stats[0][$game->h_id]['losses']++;

      // streak calc
      if ( $team_stats[0][$game->v_id]['streak'] >= 0 ) {
        $team_stats[0][$game->v_id]['streak']++;
      }
      else {
        $team_stats[0][$game->v_id]['streak'] = 1;
      }
      if ( $team_stats[0][$game->h_id]['streak'] <= 0 ) {
        $team_stats[0][$game->h_id]['streak']--;
      }
      else {
        $team_stats[0][$game->h_id]['streak'] = -1;
      }

    }

    // tie
    elseif ( $game->h_score == $game->v_score ) {
      $team_stats[0][$game->v_id]['ties']++;
      $team_stats[0][$game->h_id]['ties']++;

      // does not affect streak
    }

    $team_stats[0][$game->v_id]['games_played']++;
    $team_stats[0][$game->h_id]['games_played']++;

    $team_stats[0][$game->v_id]['pts_for'] += $game->v_score;
    $team_stats[0][$game->v_id]['pts_against'] += $game->h_score;

    $team_stats[0][$game->h_id]['pts_for'] += $game->h_score;
    $team_stats[0][$game->h_id]['pts_against'] += $game->v_score;

    // Win Percentage
    if ( $team_stats[0][$game->h_id]['wins'] + $team_stats[0][$game->h_id]['losses'] > 0 ) {
      $team_stats[0][$game->h_id]['win_percent'] = $team_stats[0][$game->h_id]['wins'] / ( $team_stats[0][$game->h_id]['wins'] + $team_stats[0][$game->h_id]['losses'] );
    }
    if ( $team_stats[0][$game->v_id]['wins'] + $team_stats[0][$game->v_id]['losses'] > 0 ) {
      $team_stats[0][$game->v_id]['win_percent'] = $team_stats[0][$game->v_id]['wins'] / ( $team_stats[0][$game->v_id]['wins'] + $team_stats[0][$game->v_id]['losses'] );
    }

    // copy into weeks stats
    $team_stats[$game->wid][$game->h_id] = $team_stats[0][$game->h_id];
    $team_stats[$game->wid][$game->v_id] = $team_stats[0][$game->v_id];

  }

  return $team_stats;
}


function build_picks_week_form($form, &$form_state, $league, $week, $account) {
  pickem_add_sport_css($league);
  $teams = get_teams(array('gameset_id' => $league->gsid));
  $games = get_games(array('week_id' => $week->wid));
  $picks = get_picks(array('league_id' => $league->lid, 'user_id' => $account->uid, 'week_id' => $week->wid));

  $form = array();
  $form['games'] = array('#tree' => TRUE);
  $last = count($games);
  $count = 0;

  $any_pickable_games = FALSE;
  if ($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 0) {
    $any_pickable_games = TRUE;
  }

  $confidence_list = array();
  $confidence_list[''] = '';
  for ($i=1; $i<=count($games); $i++) {
    $confidence_list[$i] = $i;
  }

  $stats = get_team_stats($league);

  foreach ($games as $gid => $game) {

    if ($league->pick_lock == PICKEM_PICK_LOCK_GAME && $game->game_started == 0) {
      $any_pickable_games = TRUE;
    }

    $count++;
    $form['games'][$gid]['v_id'] = array(
      '#type' => 'hidden',
      '#value' => $game->v_id
    );
    $form['games'][$gid]['h_id'] = array(
      '#type' => 'hidden',
      '#value' => $game->h_id
    );

    // Figure out the matchup operator.
    if ($game->neutral_venue == 1) {
      $matchup = "vs";
    }
    else {
      $matchup = "@";
    }

    // Build-up the team labels.
    if ( variable_get('pickem_prefer_long_team_names', 0) == 0 ) {
      $v_team_label = '<span class="logo-team-small logo-team-small-' . $teams[$game->v_id]->assetid . '">&nbsp;</span> ' . $teams[$game->v_id]->short_name;
      $h_team_label = '<span class="logo-team-small logo-team-small-' . $teams[$game->h_id]->assetid . '">&nbsp;</span> ' .  $matchup . ' ' . $teams[$game->h_id]->short_name;
    }
    else {
      $v_team_label = '<span class="logo-team-small logo-team-small-' . $teams[$game->v_id]->assetid . '">&nbsp;</span> ' .  $teams[$game->v_id]->long_name;
      $h_team_label = '<span class="logo-team-small logo-team-small-' . $teams[$game->h_id]->assetid . '">&nbsp;</span> ' .  $matchup . ' ' . $teams[$game->h_id]->long_name;
    }

    if ( variable_get('pickem_show_team_win_loss_record', 1) == 1 ) {
      $v_team_label .= "&nbsp;&nbsp;(" . $stats[0][$game->v_id]['wins'] . "-" . $stats[0][$game->v_id]['losses'] . "-" . $stats[0][$game->v_id]['ties'] . ")";
      $h_team_label .= "&nbsp;&nbsp;(" . $stats[0][$game->h_id]['wins'] . "-" . $stats[0][$game->h_id]['losses'] . "-" . $stats[0][$game->h_id]['ties'] . ")";
    }

    if ($league->scoring_engine->features['with_spread'] == 1 ) {
      $v_team_label .= '&nbsp;&nbsp;' . ($game->v_spread>0?'+' . $game->v_spread:$game->v_spread);
      $h_team_label .= '&nbsp;&nbsp;' . ($game->h_spread>0?'+' . $game->h_spread:$game->h_spread);
    }

    // Add single confidence level box
    if ($league->scoring_engine->features['with_confidence'] == 1) {
        $form['games'][$gid]['confidence'] = array(
          '#type' => 'select',
          '#options' => $confidence_list,
          '#default_value' => !empty($picks[$gid][$account->uid]->confidence) ? $picks[$gid][$account->uid]->confidence : '',
        );

      // Disable the input boxes depending on league pick_lock setting.
        if (($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) ||
            ($league->pick_lock == PICKEM_PICK_LOCK_GAME && $game->game_started == 1)) {
          $form['games'][$gid]['confidence']['#attributes']['readonly'] = 'readonly';
          $form['games'][$gid]['confidence']['#attributes']['disabled'] = 'disabled';
      }
    }


    // Add the pick score boxes.
    if ($league->scoring_engine->features['pick_score'] == 1 ||
      ($league->scoring_engine->features['with_last_weekly_score'] == 1 && is_last_game_of_week(array('gid' => $gid, 'wid' => $week->wid)))) {
      $form['games'][$gid]['pick_v_score'] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#maxlength' => 5,
        '#default_value' => isset($picks[$gid][$account->uid]->pick_v_score) ? $picks[$gid][$account->uid]->pick_v_score : '',
        '#field_suffix' => $v_team_label,
        '#prefix' => '<div class="pickem-team-score-diff-' . variable_get('pickem_prefer_long_team_names', 0) . '">'
      );
      $form['games'][$gid]['pick_h_score'] = array(
        '#type' => 'textfield',
        '#size' => 4,
        '#maxlength' => 5,
        '#default_value' => isset($picks[$gid][$account->uid]->pick_h_score) ? $picks[$gid][$account->uid]->pick_h_score : '',
        '#field_suffix' => $h_team_label,
        '#suffix' => '</div>'
      );

      // Disable the input boxes depending on league pick_lock setting.
      if (($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) ||
          ($league->pick_lock == PICKEM_PICK_LOCK_GAME && $game->game_started == 1)) {
        $form['games'][$gid]['pick_v_score']['#attributes']['readonly'] = 'readonly';
        $form['games'][$gid]['pick_v_score']['#attributes']['disabled'] = 'disabled';
        $form['games'][$gid]['pick_h_score']['#attributes']['readonly'] = 'readonly';
        $form['games'][$gid]['pick_h_score']['#attributes']['disabled'] = 'disabled';
      }
    }

    // Add the pick team boxes.
    else {
      $form['games'][$gid]['pick'] = array(
        '#type' => 'radios',
        '#default_value' => isset($picks[$gid][$account->uid]->winnerpick_id)?$picks[$gid][$account->uid]->winnerpick_id:'',
        '#attributes' => array('class' => array('pickem-team-radios-' . variable_get('pickem_prefer_long_team_names', 0))),
        '#options' => array($game->v_id => $v_team_label, $game->h_id => $h_team_label)
      );

      // Disable the radio boxes depending on league pick_lock setting.
      if (($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) ||
          ($league->pick_lock == PICKEM_PICK_LOCK_GAME && $game->game_started == 1)) {
        $form['games'][$gid]['pick']['#attributes']['readonly'] = 'readonly';
        $form['games'][$gid]['pick']['#attributes']['disabled'] = 'disabled';
      }

    }

    if (($league->pick_lock == PICKEM_PICK_LOCK_WEEK && $week->week_started == 1) ||
        ($league->pick_lock == PICKEM_PICK_LOCK_GAME && $game->game_started == 1)) {
      $form['games'][$gid]['pick_status'] = array(
        '#markup' => t('Locked')
      );
    }
    else {
      $form['games'][$gid]['pick_status'] = array(
        '#markup' => t('Pick')
      );
    }

    $form['games'][$gid]['gamestart'] = array(
      '#markup' => format_date($game->gamestart, 'medium')
    );
  }

  // If $account is entered in more than one league with the same scoring type,
  // add a checkbox to apply these picks to related leagues.
  $leagues = get_my_leagues($league->gsid, $account->uid);
  $apply_to_leagues = array();
  foreach ($leagues as $l) {
    if ($l->scoring_type == $league->scoring_type) {
      $apply_to_leagues[] = $l->name;
    }
  }

  // Add all leagues checkbox if the user belongs to any other league with the same gameset and scoring type.
  $form['apply_to_all_leagues'] = array(
    '#type' => 'checkbox',
    '#access' => ($any_pickable_games && count($apply_to_leagues) > 1),
    '#title' => t('Apply picks to all leagues that use the same game set and scoring system.'),
    '#description' => implode(', ', $apply_to_leagues),
    '#default_value' => 0
  );

  // Add the league id and week id so the theme function and the submit handler will have it.
  $form['league_id'] = array(
    '#type' => 'hidden',
    '#value' => $league->lid,
  );
  $form['week_id'] = array(
    '#type' => 'hidden',
    '#value' => $week->wid,
  );
  $form['account_id'] = array(
    '#type' => 'hidden',
    '#value' => $account->uid,
  );


  // Add the submit button, if there are pickable games.
  $form['submit'] = array(
    '#type' => 'submit',
    '#access' => $any_pickable_games,
    '#value' => t('Save Picks'),
    '#weight' => 5
  );

  return $form;
}


/**
 * Get the data from the pickem_user_settings table.
 *
  * @param $uid
 *   User ID.
* @param $default_league
 *   A league ID of the user's default league.
 * @param $email_reminders
 *   1=send email reminders, 2=don't send email reminders.
 */
function pickem_set_user_setting($uid, $default_league = NULL, $email_reminders = NULL) {
  db_merge('pickem_user_settings')
    ->key(array(
      'uid' => $uid,
    ))
    ->fields(array(
      'default_league' => $default_league == '' ? NULL : $default_league,
      'email_reminders' => $email_reminders == '' ? 0 : $email_reminders,
    ))
    ->execute();
}


/**
 * Set the data in the pickem_user_settings table.
 *
  * @param $uid
 *   User ID.
  * @return
 *   An associative array of the user setting data.
*/
function pickem_get_user_setting($uid) {
  $settings = db_select('pickem_user_settings', 's')
    ->fields('s', array('default_league', 'email_reminders'))
    ->condition('uid', $uid)
    ->execute()
    ->fetchAssoc();

  // Set default_league default to zero (ie no default league).
  if (!isset($settings['default_league'])) {
    $settings['default_league'] = 0;
  }
  // Set email_reminders default.
  if (!isset($settings['email_reminders'])) {
    $settings['email_reminders'] = 1;
  }
  return $settings;
}

function pickem_add_sport_css($league) {
  $gs = gameset_load($league->gsid);
  $sport = strtolower($gs->sport);
  drupal_add_css(drupal_get_path('module', 'pickem') . '/assets/' . $sport . '/pickem_' . $sport . '.css');
}
