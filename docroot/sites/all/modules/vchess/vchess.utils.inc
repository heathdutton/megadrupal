<?php

/**
 * @file
 * Utility functions for the vchess module.
 */

/**
 * Get a random user id (uid)
 *
 * @return
 *   A user id (uid)
 */
function vchess_random_uid() {
  $sql = "SELECT uid FROM {users} WHERE uid <> 0 ORDER BY RAND()";

  $result = db_query_range($sql, 0, 1);

  $record = $result->fetchAssoc();

  return $record['uid'];
}

/**
 * Create a user-friendly view of a time, e.g. "56 mins 34 secs"
 *
 * @param $secs
 *   Number of seconds
 *
 * @return
 *   A user-friendly string of the time, e.g. "2 days 23 hours 56 mins 34 secs"
 */
function vchess_user_friendly_time($secs) {
  $vals = array(
      TIME_UNITS_DAYS => $secs / 86400 % 7,
      TIME_UNITS_HOURS => $secs / 3600 % 24,
      TIME_UNITS_MINS => $secs / 60 % 60,
      TIME_UNITS_SECS => $secs % 60);

  $ret = array();

  $added = false;
  foreach ($vals as $k => $v) {
    if ($v > 0 || $added) {
      $added = true;
      $ret[] = $v . " " . $k;
    }
  }

  return join(' ', $ret);
}

/**
 * Check through all the current games to see if any have been lost on time
 *
 * If games are found which are lost on time, then the game is finished and
 * the player statistics are updated
 */
function vchess_check_for_lost_on_time_games() {
  $games = vchess_load_all_current_games();

  foreach ($games as $game) {
    if ($game->is_lost_on_time()) {
      vchess_update_player_stats($game);
    }
  }
}

