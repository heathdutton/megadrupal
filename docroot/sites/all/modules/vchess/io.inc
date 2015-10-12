<?php

/*
 * @file
 * This file contains functions relating to database i/o
 */


/**
 * Load array of game infos according to the given filter criteria. NULL for a
 * criteria means any value. $location must be set either 'opengames' or
 * 'archive'.
 * Always returns an array but it may be empty if no matching games were found
 * and entries are sorted most recent first. 
 */
function vchess_compareTimestamp($a, $b) {
  // Bof!  I dunno.  Let's return 0.  Hugh
  
//  if ($a['ts_last'] == $b['ts_last']) {
//    return 0;
//  }
//  else if ($a['ts_last'] < $b['ts_last']) {
//    return 1;
//  }
//  else {
//    return -1;
//  }
}

/**
 * Load a list of games for the given userid
 * 
 * @param
 *   $uid the userid of the user whose games we want
 *   
 * @return
 *   An array of in progress games
 */
function vchess_load_users_current_games($uid) {
  $game_list = array();
  
  $sql = "SELECT gid FROM {vchess_games} WHERE (white_uid = :uid OR black_uid = :uid) AND status = 'in progress'";
  $result = db_query($sql, array('uid' => $uid));

  foreach ($result as $data) {
    $gid = $data->gid;
    
    // Add a game to the list
    $game = new Game();
    $game->load($gid);
    $game_list[] = $game;
  }

  // NB: sort will destroy index key, therefore $game_list['gid'] is used
  // later instead.
  if (count($game_list) > 0) {
    usort($game_list, 'vchess_compareTimestamp');
  }
  return $game_list;
}

/**
 * Load a list of all current games
 *
 * @return
 *   An array of current (in progress) games
 */
function vchess_load_all_current_games() {
  $game_list = array();

  $sql = "SELECT gid FROM {vchess_games} WHERE status = '" . STATUS_IN_PROGRESS . "'";
  $result = db_query($sql);

  foreach ($result as $data) {
    $gid = $data->gid;

    // Add a game to the list
    $game = new Game();
    $game->load($gid);
    $game_list[] = $game;
  }

  // NB: sort will destroy index key, therefore $game_list['gid'] is used
  // later instead.
  if (count($game_list) > 0) {
    usort($game_list, 'vchess_compareTimestamp');
  }
  return $game_list;
}

/**
 * Load a list of all current games
 *
 * @return
 *   An array of current (in progress) games
 */
function vchess_load_challenges() {
  $game_list = array();

  $sql = "SELECT gid FROM {vchess_games} WHERE status = '" . STATUS_AWAITING_PLAYERS . "'" . 
    "ORDER BY time_started ASC";
  $result = db_query($sql);

  foreach ($result as $data) {
    $gid = $data->gid;

    // Add a game to the list
    $game = new Game();
    $game->load($gid);
    $game_list[] = $game;
  }

  // NB: sort will destroy index key, therefore $game_list['gid'] is used
  // later instead.
  if (count($game_list) > 0) {
    usort($game_list, 'vchess_compareTimestamp');
  }
  return $game_list;
}

